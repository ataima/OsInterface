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
#ifdef USE_VXWORK

#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include "../inc/ILogger.h"
#include "../inc/ICommPort.h"
#include "../inc/ISoftware.h"
#include "../inc/IHardware.h"
#include "../inc/CGenericHardware.h"
#include "../inc/CDefaultHardware.h"
#include "../inc/CDefaultLogger.h"
#include "../inc/CVxWorkCommPort.h"
#include "../inc/CVxWorkHardware.h"
#include "../inc/CVxWorkSoftware.h"


namespace hardware {
    namespace vxwork {

        /* default port to open  16 comm port*/
        static const char* comName[MAX_HARDWARE_SERIAL_PORT] = {
            "/tyCo/0", "/tyCo/1",
            "/tyCo/2", "/tyCo/3",
            "/tyCo/4", "/tyCo/5",
            "/tyCo/6", "/tyCo/7",
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
                comPort[i] = new CCommPort(comName[i], defaultSetting[i]);
            }
        }

        CHardware::~CHardware() {
            int i;
            for (i = 0; i < MAX_HARDWARE_SERIAL_PORT; i++) {
                delete comPort[i];
                comPort[i] = NULL;
            }
        }

    }
}

#endif

