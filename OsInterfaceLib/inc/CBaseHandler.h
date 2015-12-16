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

#ifndef CBASEHANDLER_H
#define	CBASEHANDLER_H

class ISoftware;
class ILogger;

class CBaseHandler
: public IHandler {
protected:
    ISoftware *software;
    ILogger *logger;

public:

    CBaseHandler(ISoftware *_s = NULL, ILogger *_l = NULL) :
    software(_s), logger(_l) {
    }

    virtual ~CBaseHandler() {
        software = NULL;
        logger = NULL;
    }

    void setSoftware(ISoftware *_s) {
        software = _s;
    }

    void setLogger(ILogger *_l) {
        logger = _l;
    }

    ISoftware * getSoftware(void) {
        return software;
    }

    ILogger * getLogger(void) {
        return logger;
    }

};

#endif	/* CBASEHANDLER_H */

