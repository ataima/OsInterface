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
#include <string.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "../inc/IPort.h"
#include "../inc/CBasePort.h"
#include "../inc/CPort.h"
#include "../inc/IRawData.h"
#include "../inc/ILogger.h"
#include "../inc/ISoftware.h"
#include "../inc/IHandler.h"
#include "../inc/IHardware.h"
#include "../inc/ICommPort.h"
#include "../inc/IDevice.h"
#include "../inc/ISpiPort.h"
#include "../inc/ISocketServer.h"
#include "../inc/ISocketClient.h"
#include "../inc/IMapToPointer.h"
#include "../inc/CBaseCommPort.h"
#include "../inc/CGenericHardware.h"
#include "../inc/CDefaultHardware.h"
#include "../inc/CDefaultLogger.h"
#include "../inc/CMapToPointer.h"
#include "../inc/IBlock.h"
#include "../inc/CBaseBlock.h"
#include <termios.h>
#include "../inc/CHardware.h"
#include "../inc/CLinuxCommPort.h"
#include "../inc/CGenericTestPort.h"
#include "../inc/CLinuxSoftware.h"
#include "../inc/CBaseHandler.h"
#include "../inc/CBaseSocketServer.h"
#include "../inc/CBaseSocketClient.h"
#include "../inc/CSocketServer.h"
#include "../inc/CSocketClient.h"
#include "../inc/CBaseSpiPort.h"
#include "../inc/CSpiPort.h"




// CONFIGURE YOUR HARDWARE HERE !!!!

namespace hardware {
    /* default port to open   comm port*/
    static const char* comName[MAX_HARDWARE_SERIAL_PORT] = {
        "/dev/ttyS0", "/dev/ttyS1",
        "/dev/ttyS2", "/dev/ttyS3",
        "/dev/ttyS4", "/dev/ttyS5",
        "/dev/ttyS6", "/dev/ttyS7",
    };

    static const commSetting defaultSetting[MAX_HARDWARE_SERIAL_PORT] = {
        {57600, 8, 0, 1, false},
        {57600, 8, 0, 1, false},
        {57600, 8, 0, 1, false},
        {57600, 8, 0, 1, false},
        {57600, 8, 0, 1, false},
        {57600, 8, 0, 1, false},
        {57600, 8, 0, 1, false},
        {57600, 8, 0, 1, false},
    };

    CHardware::CHardware() {
        int i;
        for (i = 0; i < MAX_HARDWARE_SERIAL_PORT; i++) {
            comPort[i] = new CCommPort(i, comName[i], defaultSetting[i]);
        }
        for (i = 0; i < MAX_HARDWARE_SOCKET_SERVER; i++) {
            socketServer[i] = new CSocketServer();
        }
        for (i = 0; i < MAX_HARDWARE_SOCKET_CLIENT; i++) {
            socketClient[i] = new CSocketClient();
        }
        for (i = 0; i < MAX_HARDWARE_SPI_PORT; i++) {
            spiPort[i] = new CSpiPort();
        }
    }

    CHardware::~CHardware() {
        int i;
        for (i = 0; i < MAX_HARDWARE_SERIAL_PORT; i++) {
            delete comPort[i];
            comPort[i] = NULL;
        }
        for (i = 0; i < MAX_HARDWARE_SOCKET_SERVER; i++) {
            delete socketServer[i];
            socketServer[i] = NULL;
        }
        for (i = 0; i < MAX_HARDWARE_SOCKET_CLIENT; i++) {
            delete socketClient[i];
            socketClient[i] = NULL;
        }
        for (i = 0; i < MAX_HARDWARE_SPI_PORT; i++) {
            delete spiPort[i];
            spiPort[i] = NULL;
        }
    }


}

#endif