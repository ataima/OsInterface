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
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>

#include <vector>
#include <map>
#include "../inc/IPort.h"
#include "../inc/CBasePort.h"
#include "../inc/CPort.h"
#include "../inc/ILogger.h"
#include "../inc/ISoftware.h"
#include "../inc/IHardware.h"
#include "../inc/ICommPort.h"
#include "../inc/ISpiPort.h"
#include "../inc/IHandler.h"
#include "../inc/ISocketServer.h"
#include "../inc/ISocketClient.h"
#include "../inc/IMapToPointer.h"
#include "../inc/CGenericHardware.h"
#include "../inc/CMapToPointer.h"
#include "../inc/CDefaultHardware.h"



namespace hardware {

    CDefaultHardware::CDefaultHardware() {
        memset(comPort, 0, sizeof (comPort));
        memset(socketServer, 0, sizeof (socketServer));
        memset(socketClient, 0, sizeof (socketClient));
        memset(spiPort, 0, sizeof (spiPort));
    }

    CDefaultHardware::~CDefaultHardware() {
    }
}
