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
#if USE_VXWORK
#include "../inc/device_configure.h"
#include <stdio.h>
#include <string.h>
#include "../inc/ISpiPort.h"
#include "../inc/CBaseSpiPort.h"
#include "../inc/CVxWorkSpiPort.h"


namespace hardware {
    namespace vxwork {

        CSpiPort::CSpiPort(int nPort, const char *_filename, const spiSetting & setting)
        : CBaseSpiPort(nPort, _filename, setting) {
        }

        CSpiPort::~CSpiPort() {
        }

        int
        CSpiPort::Open(spiSetting * setting) {
            return 0;
        }

        int
        CSpiPort::Close(void) {
            return 0;

        }

        int
        CSpiPort::Flush(void) {
            return 0;

        }

        int
        CSpiPort::Write(unsigned char *out, unsigned int size, unsigned int & writed) {
            return 0;

        }

        int
        CSpiPort::Read(unsigned char *in, unsigned int size, unsigned int & readed, unsigned int timeout) {
            return 0;

        }

        int
        CSpiPort::WriteAndRead(unsigned char *out, unsigned char *in, unsigned int size, unsigned int & readed, unsigned int timeout) {
            return 0;

        }

    }
}



#endif