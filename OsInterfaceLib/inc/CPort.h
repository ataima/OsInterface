
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

#ifndef C_LINUX_PORT_H
#define	C_LINUX_PORT_H


namespace hardware {

    class CPort
    : public CBasePort {
    public:

        CPort(int file_id = -1) : CBasePort(file_id) {
        }
        /// 
        virtual int Write(unsigned char *out, size_t size, size_t & writed, size_t timeout);
        /// 
        virtual int sliceWrite(unsigned char *out, size_t size, size_t slice, size_t timeout);
        /// 
        virtual int Read(unsigned char *in, size_t size, size_t & readed, size_t timeout);
        /// 
        virtual int completeRead(unsigned char *in, size_t size, size_t slice, size_t timeout);
        ///
        virtual int WriteAndRead(unsigned char *out, unsigned char *in, unsigned int size, unsigned int & readed, unsigned int timeout);
        /// 
        static int Write(int handle, unsigned char *out, size_t size, size_t & writed, size_t timeout);
        ///
        static int Read(int handle, unsigned char *in, size_t size, size_t & readed, size_t timeout);
        /// 
        static int sliceWrite(int handle, unsigned char *out, size_t size, size_t slice, size_t timeout);
        ///
        static int completeRead(int handle, unsigned char * buff, size_t size, size_t slice, size_t timeout);
        ///        
        virtual bool isValidFileDescriptor(void);
        ///
        static bool isValidFileDescriptor(int handle);
    };

}
#endif	

