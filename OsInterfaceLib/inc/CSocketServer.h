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


#ifndef C_SOCKETSERVER_H
#define	C_SOCKETSERVER_H


#ifdef USE_LINUX
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

#ifdef USE_WINDOWS
#include <winsock.h>
#endif


class IHandler;
class IPort;

namespace hardware {

    class CSocketServer
    : public CBaseSocketServer
    , public CPort
    , public ::device::CBaseBlock {
    protected:

        typedef struct tag_runnig_client_id {
            int mth_thread;
            int fileDesc;
            int ipaddress;
        } running_client;

        int m_mutex;
        int clients;
        int maxClients;
        running_client * maxClientThreadID;
    private:

        int CurrentError(void);

        int checkClientAtIp(int ip);

    public:

        CSocketServer(int fd = -1);

        virtual ~CSocketServer();

        int Open(int addr, int nPort, const ISocketServer::s_Setting & setting, int maxclients = 5);

        int Open(const char*ipcname, const ISocketServer::s_Setting & setting, int maxclients = 5);

        int WaitForClients(IHandler *p, unsigned int & client_address);

        int Close(void);

        int AvaiableClients(void);

        int RemoveClient(int client);

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
#endif


    };

    /* CLASS TO MANAGE CLIENT THREADS*/
    class CHandlerSocketServerClient
    : public CBaseHandler {
    protected:
        CSocketServer *socket;
        int numClient;
        volatile int fileDesc;
        int ipaddress;
    public:

        CHandlerSocketServerClient(ISoftware *_s = NULL, ILogger *_l = NULL) :
        CBaseHandler(_s, _l), socket(NULL),
        numClient(-1), fileDesc(-1), ipaddress(0) {

        }

        virtual ~CHandlerSocketServerClient() {
            socket = NULL;
            numClient = -1;
            software = NULL;
            logger = NULL;
            fileDesc = -1;
            ipaddress = 0;
        }

        virtual void closeClientConnection(void);

        void setServer(CSocketServer *s) {
            socket = s;
        };

        void setNumClient(int n) {
            numClient = n;
        }

        void setFileD(int n) {
            fileDesc = n;
        }

        void setIPaddress(int n) {
            ipaddress = n;
        }

        int getFileD(void) {
            return fileDesc;
        }

        int getIPaddress(void) {
            return ipaddress;
        }

    };
}
#endif

