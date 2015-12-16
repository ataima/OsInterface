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

#include "../inc/ITime.h"
#include "../inc/CGenericTime.h"
#include "../inc/CCurrentTime.h"
#include "../inc/CZeroTime.h"


#ifdef USE_LINUX


ITime *CGenericTime::currInstance = new hardware::CCurrentTime();

ITime *CGenericTime::zeroInstance = new hardware::CZeroTime();

#endif

#ifdef USE_WINDOWS



ITime *CGenericTime::currInstance = new hardware::CCurrentTime();

ITime *CGenericTime::zeroInstance = new hardware::CZeroTime();

#endif

CGenericTime::CGenericTime() {
    second = millisec = 0;
}

CGenericTime::~CGenericTime() {
    second = millisec = 0;
}

void CGenericTime::subc(CGenericTime & b) {
    if (b.millisec > millisec) {
        millisec = b.millisec - millisec + 1000;
        second = second - b.second - 1;
    } else {
        millisec -= b.millisec;
        second -= b.second;
    }
}


