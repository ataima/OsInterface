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
#include <string.h>
#include <iostream>
#include <windows.h>

#include "../inc/ISoftware.h"
#include "../inc/IHardware.h"
#include "../inc/ILogger.h"
#include "../inc/IDevice.h"
#include "../inc/IPort.h"
#include "../inc/IBlock.h"
#include "../inc/CBaseBlock.h"
#include "../inc/CBasePort.h"
#include "../inc/CPort.h"
#include "../inc/ISpiPort.h"
#include "../inc/CBaseSpiPort.h"
#include "../inc/CSpiPort.h"


namespace hardware {

    CSpiPort::CSpiPort()
    : CBaseSpiPort(-1) {
    }

    CSpiPort::~CSpiPort() {
    }

    int CSpiPort::Open(int nPort, const char *_filename, const spiSetting & setting) {
        return 0;
    }

    int CSpiPort::Close(void) {
        return 0;

    }

    int CSpiPort::Flush(void) {
        return 0;

    }


}

#endif
