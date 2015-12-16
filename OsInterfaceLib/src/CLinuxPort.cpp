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
#include "../inc/IPort.h"
#include "../inc/CBasePort.h"
#include "../inc/CPort.h"



#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>


namespace hardware {

    int CPort::Write(unsigned char *out, size_t size, size_t & writed, size_t timeout) {
        return CPort::Write(mHandle, out, size, writed, timeout);
    }

    int CPort::Read(unsigned char *in, size_t size, size_t & readed, size_t timeout) {
        return CPort::Read(mHandle, in, size, readed, timeout);
    }

    int CPort::WriteAndRead(unsigned char *out, unsigned char *in, unsigned int size, unsigned int & readed, unsigned int timeout) {
        return 0;
    }

    int CPort::Write(int handle, unsigned char *out, size_t size, size_t & writed, size_t timeout) {
        int res = (int) IPort::io_port_generic_error;
        writed = 0;
        fd_set select_write_fd;
        struct timeval select_timeout;
        FD_ZERO(&select_write_fd);
        FD_SET(handle, &select_write_fd);
        select_timeout.tv_sec = (timeout / 1000);
        select_timeout.tv_usec = (timeout % 1000)*1000;
        if (timeout != 0) {
            res = select(handle + 1, NULL, &select_write_fd, NULL, &select_timeout);
        } else {
            res = select(handle + 1, NULL, &select_write_fd, NULL, NULL);
        }
        if (res <= 0) {
            FD_CLR(handle, &select_write_fd);
            return (int) IPort::io_port_write_select_error;
        }
        if (res == 1) {
            if (FD_ISSET(handle, &select_write_fd) != 0) {
                res = write(handle, out, size);
                //printf("WRITE:HANDLE : %d : RES = %d\n",handle,res);
                if (res < 0) {
                    res = (int) IPort::io_port_write_io_error;
                    FD_CLR(handle, &select_write_fd);
                    return res;
                } else
                    if (res == 0) {
                    res = (int) IPort::io_port_write_timeout_error;
                    FD_CLR(handle, &select_write_fd);
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
        FD_CLR(handle, &select_write_fd);
        return res;
    }

    int CPort::Read(int handle, unsigned char *in, size_t size, size_t & readed, size_t timeout) {
        int res = IPort::io_port_generic_error;
        readed = 0;
        fd_set select_read_fd;
        struct timeval select_timeout;
        FD_ZERO(&select_read_fd);
        FD_SET(handle, &select_read_fd);
        select_timeout.tv_sec = (timeout / 1000);
        select_timeout.tv_usec = (timeout % 1000) * 1000;
        if (timeout != 0) {
            res = select(handle + 1, &select_read_fd, NULL, NULL, &select_timeout);
        } else {
            res = select(handle + 1, &select_read_fd, NULL, NULL, NULL);
        }
        if (res <= 0) {
            FD_CLR(handle, &select_read_fd);
            return (int) IPort::io_port_read_select_error;
        }
        if (res == 1) {
            if (FD_ISSET(handle, &select_read_fd) != 0) {
                res = read(handle, in, size);
                if (res < 0) {
                    res = (int) IPort::io_port_read_io_error;
                    FD_CLR(handle, &select_read_fd);
                    return res;
                } else
                    if (res == 0) {
                    if (timeout != 0)
                        res = (int) IPort::io_port_read_timeout_error;
                    else
                        res = (int) IPort::io_port_handle_invalid;
                    FD_CLR(handle, &select_read_fd);
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
        FD_CLR(handle, &select_read_fd);
        return res;
    }

    int CPort::sliceWrite(int handle, unsigned char * buff, size_t size, size_t slice, size_t timeout) {
        int res = IPort::io_port_generic_error;
        size_t writed;
        size_t base = 0;
        while (size != 0) {
            res = Write(handle, &buff[base], (size > slice ? slice : size), writed, timeout);
            if (res != 0)
                break;
            size -= writed;
            base += writed;
        }
        return res;
    }

    int CPort::sliceWrite(unsigned char *out, size_t size, size_t slice, size_t timeout) {
        return CPort::sliceWrite(mHandle, out, size, slice, timeout);
    }

    int CPort::completeRead(int handle, unsigned char * buff, size_t size, size_t slice, size_t timeout) {
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
            result = Read(handle, &buff[total_readed], cur_size, readed, ticktime);
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

    int CPort::completeRead(unsigned char * buff, size_t size, size_t slice, size_t timeout) {
        return CPort::completeRead(mHandle, buff, size, slice, timeout);
    }

    bool CPort::isValidFileDescriptor(void) {
        return CPort::isValidFileDescriptor(mHandle);
    }
    ///

    bool CPort::isValidFileDescriptor(int handle) {
        if (handle > 0) {
            if (fcntl(handle, F_GETFL) != -1) {
                if (errno == EBADF)
                    return false;
                return true;
            }
        }
        return false;
    }



}
#endif

