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
#ifndef _ILOGGER_H
#define	_ILOGGER_H

class ILogger {
public:
    virtual int Open(const char *filename = NULL) = 0;
    virtual int Close(void) = 0;
    virtual int Write(const char *msg, unsigned int size) = 0;
    virtual void printf(const char * Format, ...) = 0;
    virtual bool enable(void) = 0;
    virtual void dump(const char *msg, void * buffer, size_t size) = 0;

    virtual ~ILogger() {
    }
};

#define LOG(a,...) if(a!=NULL && a->enable()==true){a->printf(__VA_ARGS__);}
#define LOGDUMP(a,b,c,d) if(a!=NULL && a->enable()==true){a->dump(b,c,d);}


#endif	/* _ILOGGER_H */

