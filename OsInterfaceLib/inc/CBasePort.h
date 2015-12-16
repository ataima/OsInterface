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

#ifndef CBASEPORT_H
#define	CBASEPORT_H

/*!
 * Class Base for all IO device (Comm,SPI , USB etc)
 * OPen Close FlusH IOCTL are implement to specific 
 * IO Class 
 */
class CBasePort
: public IPort {
protected:

    int mHandle; ///file Descriptor 

public:
    ///default costructor

    CBasePort(int file_id = -1) {
        mHandle = file_id;
    }
    ///default desctructor

    virtual ~CBasePort() {
        mHandle = -1;
    }
    /// to obtain the current file descriptor

    virtual inline int getFileDescriptor(void) {
        return mHandle;
    }

    virtual inline bool isOpen(void) {
        return ( mHandle != -1);
    }
};


#endif	/* CBASEPORT_H */

