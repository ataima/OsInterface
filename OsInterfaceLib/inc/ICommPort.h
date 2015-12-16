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
#ifndef _ICOMMPORT_H
#define	_ICOMMPORT_H

typedef struct tag_comm_setting {
    int baudrate;
    int databit;
    int parity; // 0 none ,1 odd 2 event
    int stop;
    bool ctsrts;
} commSetting;

typedef enum tag_comm_error {
    comm_generic_error = -1,
    comm_already_open_error = -10000, //10000
    comm_open_error, //9999
    comm_attribute_error, //9998
    comm_setconfig_error, //9997
    comm_flush_error, //9996
    comm_close_error //9995
} commError;

class ICommPort {
public:

    virtual int Open(const commSetting * setting = NULL) = 0;
    virtual int Close(void) = 0;
    virtual int Flush(void) = 0;
	
	virtual int GetPortNumber(void) = 0;
	virtual commSetting * GetSetting(void) = 0;
	virtual char * GetPortName(void) = 0;

    virtual ~ICommPort() {
    }
};

#endif	/* _IPORT_H */

