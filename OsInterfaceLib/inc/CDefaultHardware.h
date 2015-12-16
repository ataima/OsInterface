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
#ifndef _CDEFAULTHARDWARE_H
#define	_CDEFAULTHARDWARE_H


#include "../inc/hardware_configure.h"

namespace hardware {

    class CDefaultHardware
    : public hardware::CGenericHardware {
    protected:
        ICommPort *comPort[MAX_HARDWARE_SERIAL_PORT];
        ISocketServer *socketServer[MAX_HARDWARE_SOCKET_SERVER];
        ISocketClient *socketClient[MAX_HARDWARE_SOCKET_CLIENT];
        ISpiPort *spiPort[MAX_HARDWARE_SPI_PORT];
    public:
        CDefaultHardware();
        virtual ~CDefaultHardware();

        inline ICommPort * GetAvaiableSerialPort(int offset) {
            if (offset >= MAX_HARDWARE_SERIAL_PORT)
                return NULL;
            return comPort[offset];
        }

        inline ISocketServer * GetAvaiableSocketServer(int offset) {
            if (offset >= MAX_HARDWARE_SOCKET_SERVER)
                return NULL;
            return socketServer[offset];
        }

        inline ISocketClient * GetAvaiableSocketClient(int offset) {
            if (offset >= MAX_HARDWARE_SOCKET_CLIENT)
                return NULL;
            return socketClient[offset];
        };

        inline ISpiPort * GetAvaiableSpiPort(int offset) {
            if (offset >= MAX_HARDWARE_SPI_PORT)
                return NULL;
            return spiPort[offset];
        }



    };
}

#endif	/* _CDEFAULTHARDWARE_H */

