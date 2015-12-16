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

#ifndef CFILE_DOWNLOADER_H
#define	CFILE_DOWNLOADER_H

#include "CBaseDownloader.h"

/* a class to control the size occupied on a ptah an num files created*/

namespace utility {
    namespace downloader {

        typedef struct tag_start_dw {
            size_t pwd1;
            size_t pwd2;
            size_t msg_type;
            size_t total_size;
            size_t block_size;
            size_t num_blocks;
            char filename[256];
            size_t msg_crc;
        } start_dw;

        typedef struct tag_block_dw {
            size_t pwd1;
            size_t pwd2;
            size_t msg_type;
            size_t block_num;
            size_t block_size;
            size_t block_crc32;
            size_t msg_crc;

        } block_dw;

        typedef struct tag_close_dw {
            size_t pwd1;
            size_t pwd2;
            size_t msg_type;
            size_t error;
            size_t msg_crc;
        } close_dw;

#define DW_FILE_USER_MAX_MSG        200

        typedef struct tag_user_dw {
            size_t pwd1;
            size_t pwd2;
            size_t msg_type;
            size_t size; /* of user data */

            union usr_msg {
                size_t usercmd[DW_FILE_USER_MAX_MSG / sizeof (size_t)];
                char usermsg[DW_FILE_USER_MAX_MSG];
            } userdata;
            size_t msg_crc;
        } user_dw;


        typedef close_dw ack_dw;

        typedef close_dw nack_dw;

        typedef enum tag_file_downloader_error {
            file_downloader_generic_error = -1,
            file_downloader_not_initialized = -70000,
            file_downloader_no_valid_rules, // -69999
            file_downloader_no_start_service, // -69998
            file_downloader_create_mutex, // -69997
            file_downloader_block_out_of_sequence, //69996
            file_downloader_missing_close, //69995
            file_downloader_missing_start, //69994
            file_downloader_create_remote_file, //69993
            file_downloader_block_invalid, //69992
            file_downloader_file_write_fail //69991

        } file_downloader_error;

        typedef enum tag_file_downloader_messages {
            file_downloader_unk_msg,
            file_downloader_start_msg = 0x91827364,
            file_downloader_block_msg = 0x45673289,
            file_downloader_close_msg = 0x29185544,
            file_downloader_ack_msg = 0x67453423,
            file_downloader_nack_msg = 0x63423789,
            file_downloader_user_msg = 0x67984233
        } file_downloader_msg_types;

        typedef enum tag_file_downloader_status {
            file_downloader_wait_start = 100,
            file_downloader_block,
            file_downloader_close,
            file_downloader_status_error,
            file_upload_start = 200,
            file_upload_block,
            file_upload_close,
            file_user_start = 300,
            file_user_stop
        } file_downloader_status;

        class CFileDownloader
        : public CBaseDownloader {
        private:
            size_t pwd1;
            size_t pwd2;
            IPort *port;
            file_downloader_status status;
            size_t block_size;
            bool debug;
        protected:
            size_t ComputeMsgCrc(size_t *msg, size_t num);
            size_t ComputeMsgCrc(unsigned char *msg, size_t num);
            int CheckPwds(size_t * m);
            int InternalRead(unsigned char * buff, size_t size, size_t timeout);
            int InternalWrite(unsigned char * buff, size_t size, size_t timeout);
            int InternalFileRead(hardware::CFile *file, unsigned char * buff, size_t size, size_t & readed);

        protected:

            virtual int replyAck(void);

            virtual int RxStart(start_dw & start);

            virtual int RxBlock(block_dw & block, unsigned char *ptr_data);

            virtual int RxClose(close_dw & close);

            virtual int TxStart(const char * rfilename, size_t total_size, size_t & total_block);

            virtual int TxBlock(size_t num, size_t crc, size_t eff);

            virtual int TxClose(void);

            virtual int RxAckNack(size_t & error);

            virtual int RxUser(user_dw & m, start_dw & start);

            virtual int TxUser(void *buff, size_t & size);

        public:

            typedef enum tag_def_pwd {
                defs_pwd1 = 0xacabbaca, //S3500
                defs_pwd2 = 0xefacadde
            } defs_pwd;

        public:
            CFileDownloader(size_t _pwd1, size_t _pwd2);

            virtual ~CFileDownloader();

            virtual int Open(IPort * RdWr, size_t bsize = 0x4000, bool debug = false);

            virtual int Recv(void);

            virtual int Recv(start_dw start);

            virtual int Xmit(const char *src_file, const char * dest_file);

            virtual int Xmit(unsigned char *buffer, size_t buff_size, const char * dest_file, signalling_hook v = NULL);

            virtual int UserRecv(void *buff, size_t & size);

            virtual int UserXmit(void *buff, size_t & size);

            virtual int replyNack(size_t error);

        };




    }
}
#endif	/* CFILE_DOWNLOADER_H */

