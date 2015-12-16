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

#ifndef BASESOCKETSERVER_H
#define	BASESOCKETSERVER_H

#include "ISocketServer.h"

class CBaseSocketServer
: public ISocketServer {
protected:
    ISocketServer::serverMode mode;
    ISocketServer::s_Setting setting;
    int numPort;
    int address;
public:

    CBaseSocketServer(int addr, int nPort, ISocketServer::serverMode _mode = mode_client_shared_ip) {
        mode = _mode;
        address = addr;
        numPort = nPort;
        memset(&setting, 0, sizeof (ISocketServer::s_Setting));
    }

    virtual ~CBaseSocketServer() {
        memset(&setting, 0, sizeof (ISocketServer::s_Setting));
        address = numPort = 0;
        mode = ISocketServer::mode_client_shared_ip;
    }

    inline int GetPortNumber(void) {
        return numPort;
    }

    inline int GetAddress(void) {
        return address;
    }

    inline ISocketServer::s_Setting * GetSetting(void) {
        return &setting;
    }

    inline ISocketServer::serverMode GetMode(void) {
        return mode;
    }

    inline void SetMode(ISocketServer::serverMode _mode) {
        mode = _mode;
    }


};


#endif	/* BASESOCKETSERVER_H */

