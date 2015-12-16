
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
// Created on 13 Nov 2011
////////////////////////////////////////////////////////////////////////////////
#ifndef IFACE_CYPHER_H
#define	IFACE_CYPHER_H

class ICypher {
public:

    typedef enum tag_cypher_error {
        generic_error = -1,
        error_create_decode_key = -50000,
        error_create_encode_key, //49999
        error_invalid_out_size, //49998
        error_invalid_in_size //49997
    } cypher_error;

public:

    virtual const char * toString(void) = 0;

    virtual int Encrypt(unsigned char *in, size_t in_len, unsigned char *out, size_t & out_len) = 0;

    virtual int Decrypt(unsigned char *in, size_t in_len, unsigned char *out, size_t & out_len) = 0;

    virtual int Open(const unsigned char *key, size_t key_len) = 0;

    virtual ~ICypher() {
    }

};

#endif