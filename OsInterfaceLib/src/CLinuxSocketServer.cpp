////////////////////////////////////////////////////////////////////////////////
//    Copyright (C) 2011  Angelo Coppi (coppi dot angelo at virgilio dot it )
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////
// Author : Angelo Coppi (coppi dot angelo at virgilio dot it )
// History:        
// Created on 12 Nov 2011
////////////////////////////////////////////////////////////////////////////////

#ifdef USE_LINUX
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <map>
#include "../inc/IPort.h"
#include "../inc/CBasePort.h"
#include "../inc/CPort.h"
#include "../inc/IHardware.h"
#include "../inc/ISoftware.h"
#include "../inc/ILogger.h"
#include "../inc/IDevice.h"
#include "../inc/IHandler.h"
#include "../inc/CPort.h"
#include "../inc/CBaseHandler.h"
#include "../inc/ISocketServer.h"
#include "../inc/CBaseBlock.h"
#include "../inc/CBaseSocketServer.h"
#include "../inc/CSocketServer.h"
#include "../inc/CMapToPointer.h"
#include "../inc/CGenericDevice.h"
#include "../inc/CGenericHardware.h"
#include "../inc/CGenericSoftware.h"
#include "../inc/hardware_configure.h"

#include <signal.h>  // SIGALRM
#include <errno.h>  // ENOMEM
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>


namespace hardware {

    void CHandlerSocketServerClient::closeClientConnection(void) {
        if (socket != NULL && numClient >= 0) {
            close(fileDesc);
            socket->RemoveClient(numClient);
            numClient = -1;
        }
        fileDesc = -1;
    }

    static void * handlerClients(void *param) {
        int res = -1;
        CHandlerSocketServerClient * pHandler = utility::obj_cast<CHandlerSocketServerClient *>(param);
        if (pHandler != NULL) {
            res = pHandler->hook();
            pHandler->closeClientConnection();
        }
        return (void *) res;
    }

    CSocketServer::CSocketServer(int fd)
    : CBaseSocketServer(0, 0), CPort(fd) {
        clients = maxClients = mHandle = 0;
        maxClientThreadID = NULL;
        m_mutex = -1;
        device = NULL;
        logger = NULL;
        software = NULL;
    }

    CSocketServer::~CSocketServer() {
		if (m_mutex != -1)
			Close();
        address = numPort = clients = maxClients = mHandle = 0;
        if (maxClientThreadID != NULL) {
            delete[] maxClientThreadID;
        }
    }

    int CSocketServer::CurrentError(void) {
        ISocketServer::s_Error err = ISocketServer::socket_server_generic_error;
        switch (errno) {
            case EACCES: err = ISocketServer::socket_server_eaccess_error;
                break;
            case EINVAL:
            case EAFNOSUPPORT: err = ISocketServer::socket_server_no_protocol_error;
                break;
            case EMFILE: err = socket_server_no_more_file_descriptor_error;
                break;
            case ENOMEM:
            case ENOBUFS: err = socket_server_no_more_resource_error;
                break;
            case EPROTONOSUPPORT: err = socket_server_no_supported_by_protocol_error;
                break;
            case EBADF: err = socket_server_no_valid_descriptor;
                break;
            case ENOPROTOOPT:
            case EFAULT: err = socket_server_no_valid_option;
                break;
            default:
            case ENOTSOCK: err = socket_server_no_valid_socket;
                break;
        }
        return (int) (err);
    }

    int CSocketServer::Open(int addr, int nPort, const ISocketServer::s_Setting & newsetting, int maxclients) {
        ::device::CBaseBlock::Initialize();
        int res = -1;
        if (software->CreateBinaryMutex(m_mutex, false) != 0) {
            LOG(logger, "%s:%d: fail on Create Mutex : %d\n", __FUNCTION__, __LINE__, res);
            res = (int) ISocketServer::server_socket_createmutex_fail;
        } else {
            if (newsetting.domain != PF_UNIX) {
                if (software->MutexLock(m_mutex) != 0) {
                    LOG(logger, "%s:%d: MutexLock Fail : \n", __FUNCTION__, __LINE__);
                    res = (int) ISocketServer::server_socket_lockmutex_fail;
                }
                //LOG(logger, "CSocketServer::Open MUTEX LOCKED %d\n", m_mutex);
                address = addr;
                numPort = nPort;
                struct sockaddr_in netaddr; // describe clients we want to bind to
                memcpy(&setting, &newsetting, sizeof (s_Setting));
                mHandle = socket(setting.domain, setting.type, setting.protocol);
                if (mHandle > 0) {
                    if (setting.num_opt != 0) {
                        int i;
                        for (i = 0; i < setting.num_opt; i++) {
                            res = setsockopt(mHandle, setting.options[i].level, setting.options[i].option_name, &setting.options[i].value, socklen_t(sizeof (int)));
                            if (res != 0) {
                                res = CurrentError();
                                LOG(logger, "%s:%d: %08X : fail on setsockopt : %d (M:%s)\n", __FUNCTION__, __LINE__, this, res, strerror(errno));
                                if (software->MutexUnlock(m_mutex) != 0) {
                                    LOG(logger, "%s:%d: %08X : MutexLock Fail : \n", __FUNCTION__, __LINE__, this);
                                    res = (int) ISocketServer::server_socket_lockmutex_fail;
                                }
                                //LOG(logger, "CSocketServer::Open MUTEX UNLOCKED 1 %d\n", m_mutex);
                                Close();
                                return res;
                            }
                        }
                    }
                    netaddr.sin_family = AF_INET;
                    netaddr.sin_port = htons(numPort);
                    netaddr.sin_addr.s_addr = htonl(address);
                    memset(netaddr.sin_zero, 0, sizeof (netaddr.sin_zero));
                    res = bind(mHandle, (struct sockaddr *) &netaddr, sizeof (netaddr));
                    if (res != 0) {
                        res = CurrentError();
                        LOG(logger, "%s:%d: %08X : fail on bind : %d (M:%s)\n", __FUNCTION__, __LINE__, this, res, strerror(errno));
                        if (software->MutexUnlock(m_mutex) != 0) {
                            LOG(logger, "%s:%d: %08X : MutexLock Fail : \n", __FUNCTION__, __LINE__, this);
                            res = (int) ISocketServer::server_socket_lockmutex_fail;
                        }
                        //LOG(logger, "CSocketServer::Open MUTEX UNLOCKED 2%d\n", m_mutex);
                        Close();
                        return res;
                    } else {
                        res = listen(mHandle, maxclients);
                        if (res != 0) {
                            res = CurrentError();
                            LOG(logger, "%s:%d: %08X : fail on listen : %d (M:%s)\n", __FUNCTION__, __LINE__, this, res, strerror(errno));
                            if (software->MutexUnlock(m_mutex) != 0) {
                                LOG(logger, "%s:%d: %08X : MutexLock Fail : \n", __FUNCTION__, __LINE__, this);
                                res = (int) ISocketServer::server_socket_lockmutex_fail;
                            }
                            //LOG(logger, "CSocketServer::Open MUTEX UNLOCKED 3%d\n", m_mutex);
                            Close();
                            return res;
                        }
                        maxClients = maxclients;
                        maxClientThreadID = new running_client[maxClients];
                        memset(maxClientThreadID, 0, sizeof (running_client) * maxclients);
                        /* try getsockopt SO_ERROR to reset errors*/
                        int error[10];
                        socklen_t len = 0;
                        bzero(error, sizeof (error));
                        getsockopt(mHandle, SOL_SOCKET, SO_ERROR, (void *) &error, &len);
                        if (error[0] != 0) {
                            LOG(logger, "%s:%d: %08X : WARNING : getsockopt : %d:%d (M:%s)\n", __FUNCTION__, __LINE__, this, error[0], len, strerror(errno));
                        }
                    }
                } else {
                    res = CurrentError();
                    LOG(logger, "%s:%d: %08X : fail on socket : %d (M:%s)\n", __FUNCTION__, __LINE__, this, res, strerror(errno));
                }

                if (software->MutexUnlock(m_mutex) != 0) {
                    LOG(logger, "%s:%d: %08X : MutexLock Fail : \n", __FUNCTION__, __LINE__, this);
                    res = (int) ISocketServer::server_socket_lockmutex_fail;
                }
                //LOG(logger, "CSocketServer::Open MUTEX UNLOCKED 4 %d\n", m_mutex);
            } else {
                LOG(logger, "%s:%d invalid socket.domain\n", __FUNCTION__, __LINE__);
                res = (int) ISocketServer::server_socket_invalid_domain;
            }
        }
        return res;
    }

    int CSocketServer::Open(const char*ipcname, const ISocketServer::s_Setting & newsetting, int maxclients) {
        ::device::CBaseBlock::Initialize();
        int res = -1;
        if (software->CreateBinaryMutex(m_mutex, false) != 0) {
            LOG(logger, "%s:%d: fail on Create Mutex : %d\n", __FUNCTION__, __LINE__, res);
            res = (int) ISocketServer::server_socket_createmutex_fail;
        } else {
            if (newsetting.domain == PF_UNIX) {
                if (software->MutexLock(m_mutex) != 0) {
                    LOG(logger, "%s:%d: MutexLock Fail : \n", __FUNCTION__, __LINE__);
                    res = (int) ISocketServer::server_socket_lockmutex_fail;
                }
                //LOG(logger, "CSocketServer::Open MUTEX LOCKED %d\n", m_mutex);
                address = 0;
                numPort = 0;
                struct sockaddr_un netaddr; // describe clients we want to bind to
                memcpy(&setting, &newsetting, sizeof (s_Setting));
                mHandle = socket(setting.domain, setting.type, setting.protocol);
                if (mHandle > 0) {
                    if (setting.num_opt != 0) {
                        int i;
                        for (i = 0; i < setting.num_opt; i++) {
                            res = setsockopt(mHandle, setting.options[i].level, setting.options[i].option_name, &setting.options[i].value, socklen_t(sizeof (int)));
                            if (res != 0) {
                                res = CurrentError();
                                LOG(logger, "%s:%d: %08X : fail on setsockopt : %d (M:%s)\n", __FUNCTION__, __LINE__, this, res, strerror(errno));
                                if (software->MutexUnlock(m_mutex) != 0) {
                                    LOG(logger, "%s:%d: %08X : MutexLock Fail : \n", __FUNCTION__, __LINE__, this);
                                    res = (int) ISocketServer::server_socket_lockmutex_fail;
                                }
                                //LOG(logger, "CSocketServer::Open MUTEX UNLOCKED 1 %d\n", m_mutex);
                                Close();
                                return res;
                            }
                        }
                    }
                    netaddr.sun_family = AF_UNIX;
                    strncpy(netaddr.sun_path, ipcname, sizeof (netaddr.sun_path));
                    remove(ipcname);
                    res = bind(mHandle, (struct sockaddr *) &netaddr, sizeof (netaddr));
                    if (res != 0) {
                        res = CurrentError();
                        LOG(logger, "%s:%d: %08X : fail on bind : %d (M:%s)\n", __FUNCTION__, __LINE__, this, res, strerror(errno));
                        if (software->MutexUnlock(m_mutex) != 0) {
                            LOG(logger, "%s:%d: %08X : MutexLock Fail : \n", __FUNCTION__, __LINE__, this);
                            res = (int) ISocketServer::server_socket_lockmutex_fail;
                        }
                        //LOG(logger, "CSocketServer::Open MUTEX UNLOCKED 2%d\n", m_mutex);
                        Close();
                        return res;
                    } else {
                        res = listen(mHandle, maxclients);
                        if (res != 0) {
                            res = CurrentError();
                            LOG(logger, "%s:%d: %08X : fail on listen : %d (M:%s)\n", __FUNCTION__, __LINE__, this, res, strerror(errno));
                            if (software->MutexUnlock(m_mutex) != 0) {
                                LOG(logger, "%s:%d: %08X : MutexLock Fail : \n", __FUNCTION__, __LINE__, this);
                                res = (int) ISocketServer::server_socket_lockmutex_fail;
                            }
                            //LOG(logger, "CSocketServer::Open MUTEX UNLOCKED 3%d\n", m_mutex);
                            Close();
                            return res;
                        }
                        chmod(ipcname, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
                        maxClients = maxclients;
                        maxClientThreadID = new running_client[maxClients];
                        memset(maxClientThreadID, 0, sizeof (running_client) * maxclients);
                        /* try getsockopt SO_ERROR to reset errors*/
                        int error[10];
                        socklen_t len = 0;
                        bzero(error, sizeof (error));
                        getsockopt(mHandle, SOL_SOCKET, SO_ERROR, (void *) &error, &len);
                        if (error[0] != 0) {
                            LOG(logger, "%s:%d: %08X : WARNING : getsockopt : %d:%d (M:%s)\n", __FUNCTION__, __LINE__, this, error[0], len, strerror(errno));
                        }
                    }
                } else {
                    res = CurrentError();
                    LOG(logger, "%s:%d: %08X : fail on socket : %d (M:%s)\n", __FUNCTION__, __LINE__, this, res, strerror(errno));
                }

                if (software->MutexUnlock(m_mutex) != 0) {
                    LOG(logger, "%s:%d: %08X : MutexLock Fail : \n", __FUNCTION__, __LINE__, this);
                    res = (int) ISocketServer::server_socket_lockmutex_fail;
                }
                //LOG(logger, "CSocketServer::Open MUTEX UNLOCKED 4 %d\n", m_mutex);
            } else {
                LOG(logger, "%s:%d invalid socket.domain\n", __FUNCTION__, __LINE__);
                res = (int) ISocketServer::server_socket_invalid_domain;
            }
        }
        return res;
    }

    int CSocketServer::WaitForClients(IHandler *pService, unsigned int & client_address) {
        int res, f_d;
        res = -1;
        client_address = 0;
        if (device != NULL) {
            if (clients < maxClients) {
                struct sockaddr_in client;
                socklen_t clientlen = sizeof (client);
                res = f_d = 0;
                memset(&client, 0, clientlen);
                f_d = accept(mHandle, (struct sockaddr *) &client, &clientlen);
                if (f_d <= 0) {
                    res = CurrentError();
                    LOG(logger, "%s:%d: %08X : accept fail : (M:%s)\n", __FUNCTION__, __LINE__, this, strerror(errno));
                } else {
                    if (software->MutexLock(m_mutex) != 0) {
                        LOG(logger, "%s:%d: %08X : MutexLock Fail : \n", __FUNCTION__, __LINE__, this);
                        res = (int) ISocketServer::server_socket_lockmutex_fail;
                    }
                    //LOG(logger, "CSocketServer::WaitForClients MUTEX LOCKED %d\n", m_mutex);
                    LOG(logger, "%s:%d: %08X : %d  accept client : %s - file id =%d \n", __FUNCTION__, __LINE__, this, mHandle, inet_ntoa(client.sin_addr), f_d);
                    client_address = client.sin_addr.s_addr;
                    if (mode == ISocketServer::mode_client_exlusive_ip) {
                        // only a client with the same IP address
                        checkClientAtIp(client_address);
                    }
                    CHandlerSocketServerClient * pServC = utility::obj_cast<CHandlerSocketServerClient *>(pService);
                    pServC->setSoftware(software);
                    pServC->setLogger(logger);
                    pServC->setServer(this);
                    pServC->setNumClient(clients);
                    pServC->setFileD(f_d);
                    pServC->setIPaddress(client_address);
                    res = software->CreateThread((void (*)()) handlerClients, pServC, maxClientThreadID[clients].mth_thread);
                    if (res == 0) {
                        pServC->setNumClient(maxClientThreadID[clients].mth_thread);
                        maxClientThreadID[clients].fileDesc = f_d;
                        maxClientThreadID[clients].ipaddress = client_address;
                        LOG(logger, "%s:%d: %08X : Service  Thread created ID:%d C:%d : IP(0x%08X:0x%08X)\n", __FUNCTION__, __LINE__, this, maxClientThreadID[clients].mth_thread, clients, client_address, pServC);
                        clients++;
                    } else {
                        LOG(logger, "%s:%d: %08X :  cannot create service thread\n", __FUNCTION__, __LINE__, this);
                        res = (int) ISocketServer::server_socket_no_service_thread;
                    }
                    if (software->MutexUnlock(m_mutex) != 0) {
                        LOG(logger, "%s:%d: %08X : MutexLock Fail : \n", __FUNCTION__, __LINE__, this);
                        res = (int) ISocketServer::server_socket_lockmutex_fail;
                    }
                    //LOG(logger, "CSocketServer::WaitForClients MUTEX UNLOCKED %d\n", m_mutex);
                }
            } else
                res = (int) ISocketServer::socket_server_max_clients;
        }
        return res;
    }

    int CSocketServer::AvaiableClients(void) {
        return maxClients - clients;
    }

    int CSocketServer::RemoveClient(int dead_client) {
        int res = -1;
        if (device != NULL && software != NULL) {
            if (software->MutexLock(m_mutex) != 0) {
                LOG(logger, "%s:%d: %08X : MutexLock Fail : \n", __FUNCTION__, __LINE__, this);
                res = (int) ISocketServer::server_socket_lockmutex_fail;
            } else {
                //LOG(logger, "CSocketServer::RemoveClient MUTEX LOCKED %d\n", m_mutex);

                if (clients != 0 && VALID_HANDLE(dead_client)) {
                    int i;
                    //FIND num_client deaded 
                    for (i = 0; i < maxClients; i++) {
                        if (maxClientThreadID[i].mth_thread == dead_client)
                            break;
                    }

                    //LOG(logger, "CLinuxSocketServer::%s %08X : dead_client offset=%d \n", __FUNCTION__, this, i );
                    dead_client = i;

                    LOG(logger, "%s:%d: %08X : Close Service Thread : %d \n", __FUNCTION__, __LINE__, this, maxClientThreadID[dead_client]);
                    close(maxClientThreadID[dead_client].fileDesc);
                    software->SleepThread(200);
                    if (software->DestroyThread(maxClientThreadID[dead_client].mth_thread) != 0) {
                        LOG(logger, "%s:%d: %08X : cannot terminate %d thread\n", __FUNCTION__, __LINE__, this, maxClientThreadID[dead_client]);
                        res = (int) ISocketServer::server_socket_destroy_service_thread;
                    }
                    for (i = dead_client; i < clients && i < maxClients - 1; i++) {
                        memcpy(&maxClientThreadID[i], &maxClientThreadID[i + 1], sizeof (running_client));
                    }
                    memset(&maxClientThreadID[clients], 0, sizeof (running_client));
                    clients--;
                    res = 0;

                }
                if (software->MutexUnlock(m_mutex) != 0) {
                    LOG(logger, "%s:%d: %08X : MutexLock Fail : \n", __FUNCTION__, __LINE__, this);
                    res = (int) ISocketServer::server_socket_lockmutex_fail;
                }
                // LOG(logger, "CSocketServer::RemoveClient MUTEX UNLOCKED %d\n", m_mutex);
            }
        }
        return res;
    }

    int CSocketServer::Close(void) {
        int res = -1;
        if (device != NULL && software != NULL) {
            if (software->MutexLock(m_mutex) != 0) {
                LOG(logger, "%s:%d: %08X : MutexLock Fail : \n", __FUNCTION__, __LINE__, this);
                res = (int) ISocketServer::server_socket_lockmutex_fail;
            }
            //LOG(logger, "CSocketServer::Close MUTEX LOCKED %d\n", m_mutex);
            if (mHandle != 0) {
                res = shutdown(mHandle, SHUT_RDWR);
                if (res == 0) {
                    res = close(mHandle);
                    mHandle = 0;
                    if (res == -1) {
                        LOG(logger, "%s:%d: %08X : close fail %d (M:%s)\n", __FUNCTION__, __LINE__, this, errno, strerror(errno));
                        res = (int) ISocketServer::server_socket_close_socket_error;
                    }
                } else {
                    LOG(logger, "%s:%d: %08X : shuntdown fail %d (M:%s)\n", __FUNCTION__, __LINE__, this, errno, strerror(errno));
                    res = (int) ISocketServer::server_socket_shuntdown_socket_error;
                }
                if (clients != 0) {
                    int i;
                    for (i = 0; i < clients; i++) {
                        if (maxClientThreadID[i].mth_thread != -1) {
                            close(maxClientThreadID[i].fileDesc);
                            maxClientThreadID[i].fileDesc = 0;
                            software->SleepThread(500);
                            if (software->DestroyThread(maxClientThreadID[i].mth_thread) != 0) {
                                LOG(logger, "%s:%d: %08X  cannot terminate %d thread\n", __FUNCTION__, __LINE__, this, i);
                                res = (int) ISocketServer::server_socket_destroy_service_thread;
                            }
                        }
                    }
                }
            }
            if (software->MutexUnlock(m_mutex) != 0) {
                LOG(logger, "%s:%d: %08X : MutexUnLock Fail : \n", __FUNCTION__, __LINE__, this);
                res = (int) ISocketServer::server_socket_lockmutex_fail;
            }
            //LOG(logger, "CSocketServer::Close MUTEX UNLOCKED %d\n", m_mutex);
            if (software->DestroyMutex(m_mutex) != 0) {
                LOG(logger, "%s:%d: %08X : DestroyMutex Fail : \n", __FUNCTION__, __LINE__, this);
                res = (int) ISocketServer::server_socket_destroymutex_fail;
            }
            m_mutex = -1;
        }
        return res;
    }

    int CSocketServer::checkClientAtIp(int ip) {
        int res = -1;
        int i = 0;
        for (i = 0; i < clients; i++) {
            if (maxClientThreadID[i].ipaddress == ip) {
                //already exist;
                close(maxClientThreadID[i].fileDesc);
                software->DestroyThread(maxClientThreadID[i].mth_thread);
                RemoveClient(i);
                break;
            }
        }
        return res;
    }


}


#endif
