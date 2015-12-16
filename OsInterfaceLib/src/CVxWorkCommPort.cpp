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

#include "../inc/IPort.h"
#include "../inc/CLinuxPort.h"
#include "../inc/ICommPort.h"
#include "../inc/CVxWorkCommPort.h"


namespace hardware {
    namespace vxwork {

        CCommPort::CCommPort(const char *_filename, const commSetting & _setting) {
            mHandle = -1;
            strcpy(filename, _filename);
            memcpy(&setting, &_setting, sizeof (commSetting));
        }

        CCommPort::~CCommPort() {
            mHandle = -1;
            bzero(filename, sizeof (filename));
            bzero(&setting, sizeof (commSetting));
        }

        int CCommPort::Open(commSetting * setting) {
            int res = -1;
            return res;
        }

        int CCommPort::Close(void) {
            int res = -1;
            return res;
        }

        int CCommPort::Flush(void) {
            int res = -1;
            return res;
        }


    }
}

#endif

