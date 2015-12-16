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

#ifndef ISPIPORT_H
#define	ISPIPORT_H

class ISpiPort {
public:

    typedef struct tag_spi_setting {
        int speed;
        int bit_len;
        int bit_order;
        int datalen;
        //  add ?
    } spiSetting;

    typedef enum tag_spi_error {
        spi_generic_error = -1,
        spi_already_open_error = -10000, //10000
        spi_open_error, //9999
        spi_set_wr_mode, //9998            
        spi_set_rd_mode, //9997            
        spi_set_bit_len, //9996            
        spi_set_speed, //9995
        spi_close_error, //9994
        spi_flush_error, // 9993
        spi_write_error, //9992
        spi_partial_write_error, ///9991
        spi_read_timeout_errror // 9990

    } spiError;
public:
    virtual int Open(int nPort, const char *_filename, const spiSetting & setting) = 0;

    virtual int Close(void) = 0;

    virtual int Flush(void) = 0;

    virtual ~ISpiPort() {
    }
};


#endif	/* ISPIPORT_H */

