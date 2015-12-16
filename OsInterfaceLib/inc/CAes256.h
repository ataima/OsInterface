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


#ifndef CAES256_H
#define	CAES256_H

#include "CBaseCypher.h"




namespace crypto {

    class CAes256
    : public CBaseCypher {
#define AES_MAXNR 14
#define AES_BLOCK_SIZE 16

        typedef struct aes_key_st {
            unsigned int rd_key[4 * (AES_MAXNR + 1)];
            int rounds;
        } AES_KEY;
        AES_KEY encKey;
        AES_KEY decKey;
    private:

        void AES_decrypt(const unsigned char *in, unsigned char *out, const AES_KEY *key);

        void AES_encrypt(const unsigned char *in, unsigned char *out, const AES_KEY *key);

        int AES_set_decrypt_key(const unsigned char *userKey, const int bits, AES_KEY *key);

        int AES_set_encrypt_key(const unsigned char *userKey, const int bits, AES_KEY *key);

        int Test(void);

    public:

        CAes256();

        virtual ~CAes256();

        const char * toString(void) {
            return "AES-256";
        };

        int Encrypt(unsigned char *in, size_t in_len, unsigned char *out, size_t & out_len);

        int Decrypt(unsigned char *in, size_t in_len, unsigned char *out, size_t & out_len);

        int Open(const unsigned char *key, size_t key_len);

    };

}
#endif	/* CBASECONTROLLER_H */

