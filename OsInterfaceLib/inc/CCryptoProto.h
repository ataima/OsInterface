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
// Created on 10 Apr 2015
////////////////////////////////////////////////////////////////////////////////



#ifndef C_CRYPTO_PROTO_H
#define C_CRYPTO_PROTO_H


#include "ICypher.h"
#include "CBaseCryptoProto.h"

namespace crypto {

    class CCryptoProtocol
    : public CBaseCryptoProtocol {
    protected:

        typedef enum tag_cmd_msg {
            cmd_data_msg = 0x10000,
            cmd_ack_msg = 0,
            cmd_nack_msg = 0x80000000,
            cmd_nack_mask_cmd = 0xffff0000,
            cmd_nack_mask_data = 0xffff
        } cmd_msg;

        typedef struct tag_crypto_protocol_msg {
            size_t crc;
            size_t blocks;
            size_t real_size;
            cmd_msg cmd;
        } crypto_protocol_msg;


        IPort *port;

        ICypher *cypher;

        bool debug;

        unsigned char *memIO;

        size_t max_mem_size;

        size_t read_timeout;

    private:
        // support functions

        unsigned char * allocBuffer(size_t size_allocated);

        unsigned char * allocBuffer(size_t req_size, size_t & size_allocated);

        int freeBuffer(unsigned char ** buff, size_t size);

        size_t ComputeCrc32(const CCryptoProtocol::crypto_protocol_msg & msg, const unsigned char *ptr, size_t size);

        int InternalWrite(unsigned char * buff, size_t size, size_t timeout);

        int InternalRead(unsigned char * buff, size_t size, size_t timeout);

        int RxAckNack(size_t & error);

        int replyAck(void);

        int replyNack(size_t error);

    public:

        CCryptoProtocol();

        ~CCryptoProtocol();

        virtual int Open(IPort * RdWr, ICypher *cypher, size_t size, size_t read_timeout, bool debug = false);

        virtual int Recv(unsigned char *out, size_t max_len, size_t & len);

        virtual int Xmit(unsigned char *in, size_t len);

        inline void Debug(bool v) {
            debug = v;
        }


    };


}

#endif