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






#ifndef _CLINUXCOMMPort_H
#define	_CLINUXCOMMPort_H

#include "CPort.h"
#include <termios.h>

// Customize Your Port CPort_conf1, conf2 etc

namespace hardware {

    class CCommPort
    : public CBaseCommPort
    , public CPort
    , public ::device::CBaseBlock {
        struct termios tio;
        struct termios old_tio;
    private:
        unsigned int traslateConfigSpeed(unsigned int speed);
        unsigned int traslateConfigBit(unsigned int bits);
		int SetFilenameFromPort(void);
    public:
        CCommPort(int port,  const commSetting & _setting);
        virtual ~CCommPort();
        int Open(const commSetting * setting = NULL);
        int Close(void);
        int Flush(void);
    };
}

#endif

#endif	/* _CLINUXPort_H */

