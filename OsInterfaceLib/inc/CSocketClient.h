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


#ifndef C_SOCKETCLIENT_H
#define	C_SOCKETCLIENT_H


#ifdef USE_WINDOWS
#include <winsock.h>
#endif


#ifdef USE_LINUX
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

namespace hardware {

    class CSocketClient
    : public CBaseSocketClient
    , public CPort
    , public ::device::CBaseBlock {
    protected:
        int m_mutex;
    private:
        int CurrentError(void);

    public:

        CSocketClient(int fd = -1);

        virtual ~CSocketClient();

        int Open(int addr, int nPort, const ISocketClient::s_Setting & _setting);

        int Open(const char *ipcname, const ISocketClient::s_Setting & _setting);

        int WaitForClients(IHandler *p);

        int Close(void);

        bool CheckConnection(void);

#ifdef USE_WINDOWS
        ///
        virtual int Write(unsigned char *out, size_t size, size_t & writed, size_t timeout);
        /// 
        virtual int sliceWrite(unsigned char *out, size_t size, size_t slice, size_t timeout);
        /// 
        virtual int Read(unsigned char *in, size_t size, size_t & readed, size_t timeout);
        /// 
        virtual int completeRead(unsigned char *in, size_t size, size_t slice, size_t timeout);
        ///
		virtual bool isValidFileDescriptor(void);
#endif
    };
}
#endif




