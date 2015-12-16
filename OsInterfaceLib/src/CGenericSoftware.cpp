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
#include <map>

#include "../inc/ILogger.h"
#include "../inc/IRawData.h"
#include "../inc/ISoftware.h"
#include "../inc/CMapToPointer.h"
#include "../inc/CGenericSoftware.h"
#include "../inc/CLinuxSoftware.h"
#include "../inc/CVxWorkSoftware.h"
#include "../inc/CWindowsSoftware.h"


namespace software {

    ISoftware *CGenericSoftware::instance = NULL;

    ISoftware *CGenericSoftware::Create() {
        if (instance == NULL) {
#ifdef USE_WINDOWS
            instance = new software::CSoftware();
#endif

#ifdef USE_LINUX
            instance = new software::CSoftware();
#endif

#ifdef USE_VXWORK
            instance = new software::CSoftware();
#endif

        }
        return instance;
    }

    ISoftware * CGenericSoftware::Destroy() {
        if (instance != NULL) {
            delete instance;
        }
        instance = NULL;
        return instance;
    }

    CGenericSoftware::~CGenericSoftware() {
    }

}


