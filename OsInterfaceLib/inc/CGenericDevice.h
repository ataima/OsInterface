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

#ifndef CGENERICDEVICE_H
#define	CGENERICDEVICE_H


namespace device {

    class CGenericDevice
    : public IDevice {
    protected:
        static IDevice *instance;
    public:
        static IDevice *Create(const char *filename);
        static IDevice * Destroy();

        static IDevice * getDevice() {
            return instance;
        }
        virtual ~CGenericDevice();
    };
}

#endif	/* CGENERICDEVICE_H */

