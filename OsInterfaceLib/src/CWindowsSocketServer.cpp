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

#ifdef USE_WINDOWS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <io.h>
#include "../inc/IPort.h"
#include "../inc/CBasePort.h"
#include "../inc/CPort.h"
#include "../inc/IHardware.h"
#include "../inc/ISoftware.h"
#include "../inc/ILogger.h"
#include "../inc/IDevice.h"
#include "../inc/IHandler.h"
#include "../inc/CBaseHandler.h"
#include "../inc/ISocketServer.h"
#include "../inc/CBaseBlock.h"
#include "../inc/CBaseSocketServer.h"
#include "../inc/CMapToPointer.h"
#include "../inc/CGenericDevice.h"
#include "../inc/CGenericHardware.h"
#include "../inc/CGenericSoftware.h"
#include "../inc/hardware_configure.h"
#include "../inc/CFile.h"

#include "../inc/CSocketServer.h"

#include <signal.h>  // SIGALRM
#include <errno.h>  // ENOMEM
#include <winsock.h>  // ENOMEM



#ifndef __FUNCTION__
#define __FUNCTION__ __FUNCTION__
#endif

namespace hardware {

    void CHandlerSocketServerClient::closeClientConnection(void) {
        if (socket != NULL && numClient >= 0) {
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
            case WSAEACCES: err = ISocketServer::socket_server_eaccess_error;
                break;
            case WSAEINVAL:
            case WSAEAFNOSUPPORT: err = ISocketServer::socket_server_no_protocol_error;
                break;
            case WSAEMFILE: err = socket_server_no_more_file_descriptor_error;
                break;
            case WSAENOBUFS: err = socket_server_no_more_resource_error;
                break;
            case WSATYPE_NOT_FOUND:
            case WSAEPROTONOSUPPORT: err = socket_server_no_supported_by_protocol_error;
                break;
            case WSAEBADF: err = socket_server_no_valid_descriptor;
                break;
            case WSAENOPROTOOPT:
            case WSAEFAULT: err = socket_server_no_valid_option;
                break;
            default:
            case WSAENOTSOCK: err = socket_server_no_valid_socket;
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
                            res = setsockopt(mHandle, setting.options[i].level, setting.options[i].option_name, (const char *) &setting.options[i].value, (sizeof (int)));
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
                        char error[10];
                        int len = 0;
                        memset(error, 0, sizeof (error));
                        getsockopt(mHandle, SOL_SOCKET, SO_ERROR, (char *) &error, &len);
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
                LOG(logger, "%s:%d: invalid socket.domain\n", __FUNCTION__, __LINE__);
                res = (int) ISocketServer::server_socket_invalid_domain;
            }
        }
        return res;
    }

    int CSocketServer::Open(const char*ipcname, const ISocketServer::s_Setting & newsetting, int maxclients) {
        ::device::CBaseBlock::Initialize();
        LOG(logger, "%s:%d:Invalid socket.domain\n", __FUNCTION__, __LINE__);
        int res = (int) ISocketServer::server_socket_invalid_domain;
        return res;
    }

    int CSocketServer::WaitForClients(IHandler *pService, unsigned int & client_address) {
        int res, f_d;
        res = -1;
        client_address = 0;
        if (device != NULL) {
            if (clients < maxClients) {
                struct sockaddr_in client;
                int clientlen = sizeof (client);
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
                    res = software->CreateThread((void(*)()) handlerClients, pServC, maxClientThreadID[clients].mth_thread);
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

                    //LOG(logger, "CSocketServer::%s %08X : dead_client offset=%d \n", __FUNCTION__, this, i );
                    dead_client = i;

                    LOG(logger, "%s:%d: %08X : Close Service Thread : %d \n", __FUNCTION__, __LINE__, this, maxClientThreadID[dead_client]);

                    if (maxClientThreadID[dead_client].fileDesc != -1)
                        closesocket(maxClientThreadID[dead_client].fileDesc);
                    maxClientThreadID[dead_client].fileDesc = -1;
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
                res = shutdown(mHandle, 2);
                if (res == 0) {
                    res = closesocket(mHandle);
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
                            closesocket(maxClientThreadID[i].fileDesc);
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
                software->DestroyThread(maxClientThreadID[i].mth_thread);
                RemoveClient(i);
                break;
            }
        }
        return res;
    }

    int CSocketServer::Write(unsigned char *out, size_t size, size_t & writed, size_t timeout) {
        int res = (int) IPort::io_port_generic_error;
        writed = 0;
        fd_set select_write_fd;
        struct timeval select_timeout;
        FD_ZERO(&select_write_fd);
        FD_SET(mHandle, &select_write_fd);
        select_timeout.tv_sec = (timeout / 1000);
        select_timeout.tv_usec = (timeout % 1000) * 1000;
        if (timeout != 0) {
            res = select(mHandle + 1, NULL, &select_write_fd, NULL, &select_timeout);
        } else {
            res = select(mHandle + 1, NULL, &select_write_fd, NULL, NULL);
        }
        if (res <= 0) {
            FD_CLR(mHandle, &select_write_fd);
            return (int) IPort::io_port_write_select_error;
        }
        if (res == 1) {
            if (FD_ISSET(mHandle, &select_write_fd) != 0) {
                res = send(mHandle, (const char *) out, size, 0);
                //printf("WRITE:HANDLE : %d : RES = %d\n",handle,res);
                if (res < 0) {
                    res = (int) IPort::io_port_write_io_error;
                    FD_CLR(mHandle, &select_write_fd);
                    return res;
                } else
                    if (res == 0) {
                    res = (int) IPort::io_port_write_timeout_error;
                    FD_CLR(mHandle, &select_write_fd);
                    return res;
                } else
                    writed = res;
                if (writed == size) {
                    res = 0;
                } else {
                    res = (int) IPort::io_port_partial_write_error;
                }
            } else {
                res = (int) IPort::io_port_write_select_error;
            }
        }
        FD_CLR(mHandle, &select_write_fd);
        return res;
    }

    int CSocketServer::Read(unsigned char *in, size_t size, size_t & readed, size_t timeout) {
        int res = IPort::io_port_generic_error;
        readed = 0;
        fd_set select_read_fd;
        struct timeval select_timeout;
        //peek to check connection
        res = recv(mHandle, (char *) in, size, MSG_PEEK);
        if (res == 0) {
            //connection closed by client
            res = IPort::io_port_handle_invalid;
        } else {
            FD_ZERO(&select_read_fd);
            FD_SET(mHandle, &select_read_fd);
            select_timeout.tv_sec = (timeout / 1000);
            select_timeout.tv_usec = (timeout % 1000) * 1000;
            if (timeout != 0) {
                res = select(mHandle + 1, &select_read_fd, NULL, NULL, &select_timeout);
            } else {
                res = select(mHandle + 1, &select_read_fd, NULL, NULL, NULL);
            }
            if (res <= 0) {
                FD_CLR(mHandle, &select_read_fd);
                return (int) IPort::io_port_read_select_error;
            }
            if (res == 1) {
                if (FD_ISSET(mHandle, &select_read_fd) != 0) {
                    res = recv(mHandle, (char *) in, size, 0);
                    if (res < 0) {
                        res = (int) IPort::io_port_read_io_error;
                        FD_CLR(mHandle, &select_read_fd);
                        return res;
                    } else
                        if (res == 0) {
                        if (timeout != 0)
                            res = (int) IPort::io_port_read_timeout_error;
                        else
                            res = (int) IPort::io_port_handle_invalid;
                        FD_CLR(mHandle, &select_read_fd);
                        return res;
                    } else {
                        readed = res;
                    }
                    if (readed == size) {
                        res = 0;
                    } else {
                        res = (int) IPort::io_port_partial_read_error;
                    }
                } else {
                    res = (int) IPort::io_port_read_select_error;
                }
            }
            FD_CLR(mHandle, &select_read_fd);
        }
        return res;
    }

    int CSocketServer::sliceWrite(unsigned char *out, size_t size, size_t slice, size_t timeout) {
        int res = IPort::io_port_generic_error;
        size_t writed;
        size_t base = 0;
        while (size != 0) {
            res = this->Write(&out[base], (size > slice ? slice : size), writed, timeout);
            if (res != 0)
                break;
            size -= writed;
            base += writed;
        }
        return res;
    }

    int CSocketServer::completeRead(unsigned char * buff, size_t size, size_t slice, size_t timeout) {
        int result = IPort::io_port_generic_error;
        size_t readed = 0;
        size_t total_readed = 0;
        size_t max_tick, tick, cur_size;
        size_t ticktime = 0;
        if (timeout != 0) {
            ticktime = timeout / 10;
            if (ticktime == 0)
                ticktime = 10;
        }
        cur_size = size;
        max_tick = (size / slice) + 1;
        if (max_tick < 5) max_tick = 5;
        for (tick = 0; tick < max_tick; tick++) {
            result = this->Read(&buff[total_readed], cur_size, readed, ticktime);
            if (result == IPort::io_port_partial_read_error || result == 0) {
                total_readed += readed;
                if (total_readed == size) {
                    result = 0;
                    break;
                } else {
                    cur_size -= readed;
                }
            } else
                if (result == IPort::io_port_read_timeout_error) {
                continue;
            } else
                break;
        }
        return result;
    }
}


#endif
