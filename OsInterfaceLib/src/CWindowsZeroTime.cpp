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
#include <string>
#include <sstream>
#include <time.h>

#include <errno.h>
#include <winsock.h>
#include <sys/timeb.h>

#include "../inc/ITime.h"
#include "../inc/CGenericTime.h"
#include "../inc/CZeroTime.h"


#define	timeradd(tvp, uvp, vvp)						\
	do {								\
	(vvp)->tv_sec = (tvp)->tv_sec + (uvp)->tv_sec;		\
	(vvp)->tv_usec = (tvp)->tv_usec + (uvp)->tv_usec;	\
	if ((vvp)->tv_usec >= 1000000) {			\
	(vvp)->tv_sec++;				\
	(vvp)->tv_usec -= 1000000;			\
	}							\
	} while (0)
#define	timersub(tvp, uvp, vvp)						\
	do {								\
	(vvp)->tv_sec = (tvp)->tv_sec - (uvp)->tv_sec;		\
	(vvp)->tv_usec = (tvp)->tv_usec - (uvp)->tv_usec;	\
	if ((vvp)->tv_usec < 0) {				\
	(vvp)->tv_sec--;				\
	(vvp)->tv_usec += 1000000;			\
	}							\
	} while (0)

static int gettimeofday(struct timeval *curTimeP) {
    struct _timeb localTime;

    if (curTimeP == (struct timeval *) 0) {
        errno = EFAULT;
        return (-1);
    }
    _ftime(&localTime);
    curTimeP->tv_sec = (long) (localTime.time + localTime.timezone);
    curTimeP->tv_usec = (long) (localTime.millitm * 1000);
    return (0);
}
static struct timeval zeroTime;


namespace hardware {

    CZeroTime::CZeroTime() {
        gettimeofday(&zeroTime);
    }

    void CZeroTime::getTime(ITime *out) {
        struct timeval resTime;
        struct timeval curTime;
        gettimeofday(&curTime);
        timersub(&curTime, &zeroTime, &resTime);
        second = resTime.tv_sec;
        millisec = resTime.tv_usec / 1000;
        if (out != NULL) {
            CGenericTime *t = dynamic_cast<CGenericTime *> (out);
            t->setTime(second, millisec);
        }
    }

    const char * CZeroTime::toString(void) {
        static char buff[32];
        sprintf(buff, "[%05d:%03d]", second, millisec);
        return buff;
    }
}

#endif