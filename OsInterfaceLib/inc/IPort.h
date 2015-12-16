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

#ifndef IPORT_H
#define	IPORT_H

/*! This is a generic abstraction Class base for all  IO  classes
 */
class IPort {
public:

    typedef enum tag_port_error {
        io_port_generic_error = -1,
        io_port_write_error = -100000,
        io_port_write_timeout_error, //99999
        io_port_partial_write_error, //99998
        io_port_write_io_error, //99997
        io_port_read_timeout_error, //99996
        io_port_read_io_error, //99995
        io_port_read_select_error, //99994
        io_port_write_select_error, //99993
        io_port_partial_read_error, //99992
        io_port_handle_invalid //99991

    } port_error;


    virtual int Write(unsigned char *out, size_t size, size_t & writed, size_t timeout) = 0;

    virtual int Read(unsigned char *in, size_t size, size_t & readed, size_t timeout) = 0;

    virtual int sliceWrite(unsigned char *out, size_t size, size_t slice, size_t timeout) = 0;

    virtual int completeRead(unsigned char *in, size_t size, size_t slice, size_t timeout) = 0;

    virtual int getFileDescriptor(void) = 0;

    virtual int WriteAndRead(unsigned char *out, unsigned char *in, unsigned int size, unsigned int & readed, unsigned int timeout) = 0;

    virtual bool isOpen(void) = 0;

    virtual bool isValidFileDescriptor(void) = 0;
};

#endif	/* IPORT_H */

