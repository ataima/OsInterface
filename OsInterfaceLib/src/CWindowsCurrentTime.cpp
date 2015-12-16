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
#include <time.h>
#include <map>

#include <errno.h>
#include <winsock.h>
#include <sys/timeb.h>

#include "../inc/ITime.h"
#include "../inc/CGenericTime.h"
#include "../inc/CCurrentTime.h"
#include "../inc/CMapToPointer.h"

static int gettimeofday(struct timeval *curTimeP) {
    struct _timeb localTime;

    if (curTimeP == (struct timeval *) 0) {
        errno = EFAULT;
        return (-1);
    }
    _ftime(&localTime);
    curTimeP->tv_sec = (long) (localTime.time + localTime.timezone);
    curTimeP->tv_usec = localTime.millitm * 1000;
    return (0);
}



namespace hardware {

    void CCurrentTime::getTime(ITime * out) {
        struct timeval curTime;
        gettimeofday(&curTime);
        second = curTime.tv_sec;
        millisec = curTime.tv_usec / 1000;
        if (out != NULL) {
            CGenericTime *t = dynamic_cast<CGenericTime*> (out);
            t->setTime(second, millisec);
        }
    }

    const char * CCurrentTime::toString(void) {
        time_t curTime;
        time(&curTime);
        return ctime(&curTime);
    }

}
#endif