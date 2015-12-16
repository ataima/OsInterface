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
#include "../inc/IPort.h"
#include "../inc/CBasePort.h"
#include "../inc/CPort.h"



#include <io.h>
#include <fcntl.h>
#include <winsock.h>
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
        res = WriteFile((HANDLE) handle, out, size, (LPDWORD) & writed, NULL);
        //printf("WRITE:HANDLE : %d : RES = %d\n",handle,res);
        if (res == 0) {
            res = (int) IPort::io_port_write_io_error;
            return res;
        }
        if (writed == size) {
            res = 0;
        } else {
            res = (int) IPort::io_port_partial_write_error;
        }
        return res;
    }

    int CPort::Read(int handle, unsigned char *in, size_t size, size_t & readed, size_t timeout) {
        int res = IPort::io_port_generic_error;
        res = ReadFile((HANDLE) handle, in, size, (LPDWORD) & readed, NULL);
        if (res != 0) {
            res = (int) IPort::io_port_read_io_error;
            return res;
        }
        if (readed == size) {
            res = 0;
        } else {
            res = (int) IPort::io_port_partial_read_error;
        }
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
        if (handle != NULL) {
            const char out[]="PING";
            size_t writed;
            if (Write(handle,(unsigned char*) out, 5, writed, 0) == 0)
                return true;
        }
        return false;
    }
}
#endif

