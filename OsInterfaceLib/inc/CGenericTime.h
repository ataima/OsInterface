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

#ifndef C_GENERIC_TIME_H
#define C_GENERIC_TIME_H

class CGenericTime
: public ITime {
private:
    static ITime *currInstance;
    static ITime *zeroInstance;
protected:
    size_t second;
    size_t millisec;
public:

    CGenericTime();

    virtual ~CGenericTime();

    static ITime* getCurrentTime(void) {
        return currInstance;
    }

    static ITime* getZeroTime(void) {
        return zeroInstance;
    }

    virtual inline size_t getSecond(void) {
        return second;
    }

    virtual inline size_t getMilliSec(void) {
        return millisec;
    }

    virtual inline void setTime(size_t sec, size_t mil) {
        second = sec;
        millisec = mil;
    }

    virtual inline void getTime(ITime *out) {
    }

    virtual inline const char * toString(void) {
        return NULL;
    }
    virtual void subc(CGenericTime & b);
};

#endif