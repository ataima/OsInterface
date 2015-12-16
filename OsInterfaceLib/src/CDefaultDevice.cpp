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
#include "../inc/IHardware.h"
#include "../inc/ICommPort.h"
#include "../inc/ISoftware.h"
#include "../inc/IHandler.h"
#include "../inc/IDevice.h"
#include "../inc/IMapToPointer.h"
#include "../inc/CGenericHardware.h"
#include "../inc/CGenericSoftware.h"
#include "../inc/CMapToPointer.h"
#include "../inc/CDefaultHardware.h"
#include "../inc/CGenericDevice.h"
#include "../inc/CDefaultDevice.h"
#include "../inc/CDefaultLogger.h"
#include "../inc/CBaseHandler.h"


namespace device {

    CDefaultDevice::CDefaultDevice(const char *filename) {
        hardware = hardware::CGenericHardware::Create();
        software = software::CGenericSoftware::Create();
        if (filename != NULL) {
#ifdef USE_WINDOWS
            if (strcmp(filename, "ExternLogger") == 0) {
                logger = new logger::CExternLogger();
            } else {
                logger = new logger::CDefaultLogger();
            }

#else
            logger = new logger::CDefaultLogger();
#endif
            if (logger->Open(filename) != 0) {
		printf("%s:%d:Cannot open %s for logger\n",__FUNCTION__,__LINE__,filename);    
                delete logger;
                logger = NULL;
            }
        } else {
            logger = NULL;
        }	
    }

    CDefaultDevice::~CDefaultDevice() {
        hardware = hardware::CGenericHardware::Destroy();
        software = software::CGenericSoftware::Destroy();
        if (logger != NULL) {
            logger->Close();
            delete logger;
            logger = NULL;
        }
    }
}

