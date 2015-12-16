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
#include <io.h>
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
#include <Winsock.h>

// link with Ws2_32.lib
#pragma comment(lib,"Ws2_32.lib")

namespace hardware {

    CSocketClient::CSocketClient(int fd)
    : CBaseSocketClient(0, 0), CPort(fd) {
        m_mutex = -1;
    }

    CSocketClient::~CSocketClient() {
		if (mHandle != 0)
			Close();
    }

    int CSocketClient::CurrentError(void) {
        ISocketClient::s_Error err = ISocketClient::socket_client_generic_error;
        switch (errno) {
            case WSANOTINITIALISED: err = ISocketClient::socket_client_eaccess_error;
                break;
            case WSAEINVAL:err = ISocketClient::socket_client_no_protocol_error;
                break;
            case WSAEMFILE: err = ISocketClient::socket_client_no_more_file_descriptor_error;
                break;
            case WSAENOBUFS: err = ISocketClient::socket_client_no_more_resource_error;
                break;
            case WSAEPROTONOSUPPORT: err = ISocketClient::socket_client_no_supported_by_protocol_error;
                break;
            case WSAEBADF: err = ISocketClient::socket_client_no_valid_descriptor;
                break;
            case WSAENOPROTOOPT:
            case WSAEFAULT: err = ISocketClient::socket_client_no_valid_option;
                break;
            case WSAENOTSOCK: err = ISocketClient::socket_client_no_valid_socket;
                break;
            case WSAECONNREFUSED: err = ISocketClient::socket_client_refused_connection;
                break;
            case WSAEADDRINUSE: err = ISocketClient::socket_client_address_already_in_use;
                break;
            case WSAEADDRNOTAVAIL: err = ISocketClient::socket_client_socket_not_avaiable;
                break;
            case WSAEAFNOSUPPORT: err = ISocketClient::socket_client_address_not_supported;
                break;
            case WSAEALREADY: err = ISocketClient::socket_client_already_try_to_connect;
                break;
            case WSAEINPROGRESS: err = ISocketClient::socket_client_connection_in_progress;
                break;
            case WSAEISCONN: err = ISocketClient::socket_client_already_open_error;
                break;
            case WSAEINTR: err = ISocketClient::socket_client_interrupted_by_signal;
                break;
            case WSAENETUNREACH: err = ISocketClient::socket_client_network_unavaiable;
                break;
            case WSAEPROTOTYPE: err = ISocketClient::socket_client_no_feature_supported;
                break;
            case WSAETIMEDOUT: err = ISocketClient::socket_client_timeout;
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
            LOG(logger, "%s:%d:fail on Create Mutex : %d\n", __FUNCTION__, __LINE__, res);
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
                            res = setsockopt(mHandle, setting.options[i].level, setting.options[i].option_name, (const char *) &setting.options[i].value, sizeof (int));
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
                        LOG(logger, "%s:%d:fail on connect : %d (M:%s)\n", __FUNCTION__, __LINE__, res, strerror(errno));
                        software->MutexUnlock(m_mutex);
                        Close();
                        return res;
                    }
                } else {
                    res = CurrentError();
                    LOG(logger, "%s:%d:fail on socket : %d (M:%s)\n", __FUNCTION__, __LINE__, res, strerror(errno));
                }
                software->MutexUnlock(m_mutex);
            } else {
                LOG(logger, "%s:%d:invalid socket.domain\n", __FUNCTION__, __LINE__);
                res = (int) ISocketClient::socket_client_invalid_domain;
            }
        }
        return res;
    }

    int CSocketClient::Open(const char *ipcname, const ISocketClient::s_Setting & _setting) {
        int res = -1;
        LOG(logger, "%s:%d: invalid socket.domain\n", __FUNCTION__, __LINE__);
        res = (int) ISocketClient::socket_client_invalid_domain;
        return res;
    }

    int CSocketClient::Close(void) {
        int res = 0;
        if (mHandle != 0) {
            res = shutdown(mHandle, 2);
            if (res == 0) {
                res = closesocket(mHandle);
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



    ///////////////////

    int CSocketClient::Write(unsigned char *out, size_t size, size_t & writed, size_t timeout) {
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

    int CSocketClient::Read(unsigned char *in, size_t size, size_t & readed, size_t timeout) {
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

    int CSocketClient::sliceWrite(unsigned char *out, size_t size, size_t slice, size_t timeout) {
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

    int CSocketClient::completeRead(unsigned char * buff, size_t size, size_t slice, size_t timeout) {
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



    // true 1 = OK  else broken connection

    bool CSocketClient::CheckConnection(void) {
        if (this->isValidFileDescriptor() == true) {
            char buf[32];
            int res = recv(mHandle, buf, 16, MSG_PEEK);
            LOG(logger, "%s:%d: check return : %d", __FUNCTION__, __FILE__, res);
            if (res > 0)
                return true;
        }
        return false;
    }

	bool CSocketClient::isValidFileDescriptor(void){
		if (mHandle!= NULL) {
			unsigned char out[] = "0123456789";
			size_t writed;
			if (this->Write(out, 0, writed, 100) == IPort::io_port_write_timeout_error /* send return 0 */)
				return true;
		}
		return false;
	}

}



#endif
