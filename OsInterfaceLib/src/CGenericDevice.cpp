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
#include <string>
#include <sstream>

#include "../inc/IPort.h"
#include "../inc/CBasePort.h"
#include "../inc/CPort.h"
#include "../inc/ILogger.h"
#include "../inc/IRawData.h"
#include "../inc/IHardware.h"
#include "../inc/ISoftware.h"
#include "../inc/IDevice.h"
#include "../inc/IHandler.h"
#include "../inc/CGenericHardware.h"
#include "../inc/CGenericDevice.h"
#include "../inc/CDefaultDevice.h"

namespace device {

    IDevice *CGenericDevice::instance = NULL;

    IDevice *CGenericDevice::Create(const char *filename) {
        if (instance == NULL) {
            instance = new device::CDefaultDevice(filename);
        }
        return instance;
    }

    IDevice * CGenericDevice::Destroy() {
        if (instance != NULL) {
            delete instance;
        }
        instance = NULL;
        return instance;
    }

    CGenericDevice::~CGenericDevice() {
    }

}


