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

#ifndef CBASESOCKETCLIENT_H
#define	CBASESOCKETCLIENT_H

#include "ISocketClient.h"

class CBaseSocketClient
: public ISocketClient {
protected:
    ISocketClient::s_Setting setting;
    int numPort;
    int address;
public:

    CBaseSocketClient(int addr, int nPort) {
        address = addr;
        numPort = nPort;
        memset(&setting, 0, sizeof (ISocketClient::s_Setting));
    }

    virtual ~CBaseSocketClient() {
        memset(&setting, 0, sizeof (ISocketClient::s_Setting));
        address = numPort = 0;
    }

    inline int GetPortNumber(void) {
        return numPort;
    }

    inline int GetAddress(void) {
        return address;
    }

    inline ISocketClient::s_Setting * GetSetting(void) {
        return &setting;
    }

};


#endif	/* CBASESOCKETCLIENT_H */

