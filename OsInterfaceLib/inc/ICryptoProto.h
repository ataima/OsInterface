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
// Created on 14 Nov 2011
////////////////////////////////////////////////////////////////////////////////



#ifndef IFACE_CRYPTO_PROTO_H
#define IFACE_CRYPTO_PROTO_H

class ICypher;

class ICryptoProtocol {
public:

    typedef enum tag_crypto_protocol_error {
        generic_error = -1,
        invalid_io_port = -20000,
        invalid_cypher, //19999
        no_memory_cypher, //19998
        writing_error, //19997
        encrypt_data, //19996
        reading_error, //19995
        reading_invalid_msg, //19994
        dencrypt_data, //19993
        no_memory, //19992
		bad_crc //19991
    } crypto_protocol_error;
public:

    virtual int Open(IPort * RdWr, ICypher *cypher, size_t rmem, size_t timeout, bool debug = false) = 0;

    virtual int Recv(unsigned char *out, size_t max_len, size_t & len) = 0;

    virtual int Xmit(unsigned char *in, size_t len) = 0;

    virtual ~ICryptoProtocol() {
    }
};



#endif