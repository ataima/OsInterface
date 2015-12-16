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
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>

#include "../inc/IPort.h"
#include "../inc/CBasePort.h"
#include "../inc/CPort.h"
#include "../inc/IRawData.h"
#include "../inc/CRawData.h"
#include "../inc/ILogger.h"
#include "../inc/ISoftware.h"
#include "../inc/IHardware.h"
#include "../inc/ICommPort.h"
#include "../inc/IDevice.h"
#include "../inc/CGenericTestPort.h"
#include "../inc/CDefaultLogger.h"
#include "../inc/CGenericHardware.h"
#include "../inc/CGenericDevice.h"
#include "../inc/CDefaultHardware.h"
#include "../inc/CDefaultDevice.h"



namespace test {

    CGenericTestPort::CGenericTestPort() {
        device = NULL;
        software = NULL;
        hardware = NULL;
        logger = NULL;
    }

    CGenericTestPort::~CGenericTestPort() {
        device = NULL;
        software = NULL;
        hardware = NULL;
        logger = NULL;
    }

    int CGenericTestPort::Open(void) {
        int res = -1;
        ::device::CBaseBlock::Initialize();
        if (device != NULL) {
            LOG(logger, "CCommPortTest::Open() +\n");
            LOG(logger, "CCommPortTest::Open() -\n");
            res = 0;
        }
        return res;
    }

    int CGenericTestPort::Close(void) {
        if (device != NULL) {
            LOG(logger, "CCommPortTest::Close()  +\n");
            LOG(logger, "CCommPortTest::Close()  -\n");
        }
        return 0;
    }

    int CGenericTestPort::Flush(void) {
        if (device != NULL) {
            LOG(logger, "CCommPortTest::Flush()  +\n");
            LOG(logger, "CCommPortTest::Flush()  -\n");
        }
        return 0;
    }

}
