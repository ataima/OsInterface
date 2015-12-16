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
#include "../inc/ISocketClient.h"
#include "../inc/CBaseSocketClient.h"
#include "../inc/CGenericDevice.h"
#include "../inc/CDefaultDevice.h"
#include "../inc/IBlock.h"
#include "../inc/CBaseBlock.h"
#include "../inc/CSocketClient.h"
#include "../inc/CMapToPointer.h"
#include "../inc/CGenericHardware.h"
#include "../inc/CGenericSoftware.h"
#include "../inc/hardware_configure.h"


#include <signal.h>  // SIGALRM
#include <errno.h>  // ENOMEM
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/un.h>



namespace hardware {

    CSocketClient::CSocketClient(int fd)
    : CBaseSocketClient(0, 0), CPort(fd) {
        m_mutex = -1;
    }

    CSocketClient::~CSocketClient() {
        mHandle = 0;
        m_mutex = -1;
    }

    int CSocketClient::CurrentError(void) {
        ISocketClient::s_Error err = ISocketClient::socket_client_generic_error;
        switch (errno) {
            case EACCES: err = ISocketClient::socket_client_eaccess_error;
                break;
            case EINVAL:err = ISocketClient::socket_client_no_protocol_error;
                break;
            case EMFILE: err = ISocketClient::socket_client_no_more_file_descriptor_error;
                break;
            case ENOMEM:
            case ENOBUFS: err = ISocketClient::socket_client_no_more_resource_error;
                break;
            case EPROTONOSUPPORT: err = ISocketClient::socket_client_no_supported_by_protocol_error;
                break;
            case EBADF: err = ISocketClient::socket_client_no_valid_descriptor;
                break;
            case ENOPROTOOPT:
            case EFAULT: err = ISocketClient::socket_client_no_valid_option;
                break;
            case ENOTSOCK: err = ISocketClient::socket_client_no_valid_socket;
                break;
            case ECONNREFUSED: err = ISocketClient::socket_client_refused_connection;
                break;
            case EADDRINUSE: err = ISocketClient::socket_client_address_already_in_use;
                break;
            case EADDRNOTAVAIL: err = ISocketClient::socket_client_socket_not_avaiable;
                break;
            case EAFNOSUPPORT: err = ISocketClient::socket_client_address_not_supported;
                break;
            case EAGAIN: err = ISocketClient::socket_client_insufficient_caches;
                break;
            case EALREADY: err = ISocketClient::socket_client_already_try_to_connect;
                break;
                break;
            case EINPROGRESS: err = ISocketClient::socket_client_connection_in_progress;
                break;
            case EISCONN: err = ISocketClient::socket_client_already_open_error;
                break;
            case EINTR: err = ISocketClient::socket_client_interrupted_by_signal;
                break;
            case ENETUNREACH: err = ISocketClient::socket_client_network_unavaiable;
                break;
            case EPROTOTYPE: err = ISocketClient::socket_client_no_feature_supported;
                break;
            case ETIMEDOUT: err = ISocketClient::socket_client_timeout;
                break;
        }
        return (int) (err);
    }

    int CSocketClient::Open(int addr, int nPort, const ISocketClient::s_Setting & _setting) {
        ::device::CBaseBlock::Initialize();
        int res = -1;
        res = software->CreateBinaryMutex(m_mutex, false);
        if (res != 0) {
            res = (int) ISocketClient::socket_client_createmutex_fail;
            LOG(logger, "%s:%dfail on Create Mutex : %d\n", __FUNCTION__, __LINE__, res);
        } else {
            if (_setting.domain != PF_UNIX) {
                software->MutexLock(m_mutex);
                address = addr;
                numPort = nPort;
                struct sockaddr_in netaddr; // describe clients we want to bind to
                memcpy(&setting, &_setting, sizeof (ISocketClient::s_Setting));
                mHandle = socket(setting.domain, setting.type, setting.protocol);
                if (mHandle > 0) {
                    if (setting.num_opt != 0) {
                        int i;
                        for (i = 0; i < setting.num_opt; i++) {
                            res = setsockopt(mHandle, setting.options[i].level, setting.options[i].option_name, &setting.options[i].value, socklen_t(sizeof (int)));
                            if (res != 0) {
                                res = CurrentError();
                                LOG(logger, "%s:%d:fail on setsockopt : %d (M:%s)\n", __FUNCTION__, __LINE__, res, strerror(errno));
                                software->MutexUnlock(m_mutex);
                                Close();
                                return res;
                            }
                        }
                    }
                    netaddr.sin_family = AF_INET;
                    netaddr.sin_port = htons(numPort);
                    netaddr.sin_addr.s_addr = htonl(address);
                    memset(netaddr.sin_zero, 0, sizeof (netaddr.sin_zero));
                    res = connect(mHandle, (struct sockaddr *) &netaddr, sizeof (netaddr));
                    if (res != 0) {
                        res = CurrentError();
                        LOG(logger, "%s:%d: fail on connect : %d (M:%s)\n", __FUNCTION__, __LINE__, res, strerror(errno));
                        software->MutexUnlock(m_mutex);
                        Close();
                        return res;
                    }
                    software->MutexUnlock(m_mutex);
                } else {
                    res = CurrentError();
                    LOG(logger, "%s:%d: fail on socket : %d (M:%s)\n", __FUNCTION__, __LINE__, res, strerror(errno));
                }
            } else {
                LOG(logger, "%s:%d:invalid socket.domain\n", __FUNCTION__, __LINE__);
                res = (int) ISocketClient::socket_client_invalid_domain;
            }
        }
        return res;
    }

    int CSocketClient::Open(const char *ipcname, const ISocketClient::s_Setting & _setting) {
        ::device::CBaseBlock::Initialize();
        int res = -1;
        res = software->CreateBinaryMutex(m_mutex, false);
        if (res != 0) {
            res = (int) ISocketClient::socket_client_createmutex_fail;
            LOG(logger, "%s:%d:fail on Create Mutex : %d\n", __FUNCTION__, __LINE__, res);
        } else {
            if (_setting.domain == PF_UNIX) {
                software->MutexLock(m_mutex);
                address = 0;
                numPort = 0;
                struct sockaddr_un netaddr; // describe clients we want to bind to
                memcpy(&setting, &_setting, sizeof (ISocketClient::s_Setting));
                mHandle = socket(setting.domain, setting.type, setting.protocol);
                if (mHandle > 0) {
                    if (setting.num_opt != 0) {
                        int i;
                        for (i = 0; i < setting.num_opt; i++) {
                            res = setsockopt(mHandle, setting.options[i].level, setting.options[i].option_name, &setting.options[i].value, socklen_t(sizeof (int)));
                            if (res != 0) {
                                res = CurrentError();
                                LOG(logger, "%s:%d:fail on setsockopt : %d (M:%s)\n", __FUNCTION__, __LINE__, res, strerror(errno));
                                software->MutexUnlock(m_mutex);
                                Close();
                                return res;
                            }
                        }
                    }
                    netaddr.sun_family = AF_UNIX;
                    strncpy(netaddr.sun_path, ipcname, sizeof (netaddr.sun_path));
                    res = connect(mHandle, (struct sockaddr *) &netaddr, sizeof (netaddr));
                    if (res != 0) {
                        res = CurrentError();
                        LOG(logger, "%s:%d:fail on connect : %d (M:%s)\n", __FUNCTION__, __LINE__, res, strerror(errno));
                        software->MutexUnlock(m_mutex);
                        Close();
                        return res;
                    }
                    software->MutexUnlock(m_mutex);
                } else {
                    res = CurrentError();
                    LOG(logger, "%s:%d:fail on socket : %d (M:%s)\n", __FUNCTION__, __LINE__, res, strerror(errno));
                }
            } else {
                LOG(logger, "%s:%d:invalid socket.domain\n", __FUNCTION__, __LINE__);
                res = (int) ISocketClient::socket_client_invalid_domain;
            }
        }
        return res;
    }

    int CSocketClient::Close(void) {
        int res = 0;
        if (mHandle != 0) {
            res = shutdown(mHandle, SHUT_RDWR);
            if (res == 0) {
                res = close(mHandle);
                mHandle = 0;
                if (res == -1) {
                    LOG(logger, "%s:%d:fail %d (M:%s)\n", __FUNCTION__, __LINE__, errno, strerror(errno));
                }
            }
        }
		if (VALID_HANDLE(m_mutex) && software!=NULL)
		{
			software->DestroyMutex(m_mutex);
			m_mutex = 0;
		}
        return res;
    }

    // true 1 = OK  else broken connection

    bool CSocketClient::CheckConnection(void) {
        if (this->isValidFileDescriptor() == true) {
            unsigned char buf;
            int res = recv(mHandle, &buf, 1, MSG_PEEK );
            LOG(logger, "%s:%d: check return : %d\n", __FUNCTION__, __LINE__, res);
            if (res > 0)
                return true;
        }
        return false;
    }

}




#endif
