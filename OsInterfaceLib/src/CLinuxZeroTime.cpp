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
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sstream>


#include "../inc/ITime.h"
#include "../inc/CGenericTime.h"
#include "../inc/CZeroTime.h"

static struct timeval zeroTime;


namespace hardware {

    CZeroTime::CZeroTime() {
        gettimeofday(&zeroTime, NULL);
    }

    void CZeroTime::getTime(ITime *out) {
        struct timeval resTime;
        struct timeval curTime;
        gettimeofday(&curTime, NULL);
        timersub(&curTime, &zeroTime, &resTime);
        second = resTime.tv_sec;
        millisec = resTime.tv_usec / 1000;
        if (out != NULL) {
            CGenericTime *t = dynamic_cast<CGenericTime *> (out);
            t->setTime(second, millisec);
        }
    }

    const char * CZeroTime::toString(void) {
        static char buff[64];
        sprintf(buff, "[%06d:%03d] ", second, millisec);
        return buff;
    }
}

#endif