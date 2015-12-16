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

#include <stdio.h> 
#include <stdio.h>
#include <string.h>
#include <string>
#include <map>
#include <fcntl.h>
#include <sstream>

#include "../inc/ILogger.h"
#include "../inc/ISoftware.h"
#include "../inc/IHardware.h"
#include "../inc/IDevice.h"
#include "../inc/CGenericDevice.h"
#include "../inc/CDefaultDevice.h"
#include "../inc/IBlock.h"
#include "../inc/IPort.h"
#include "../inc/IFile.h"
#include "../inc/CBasePort.h"
#include "../inc/CBaseBlock.h"
#include "../inc/IDownload.h"
#include "../inc/CMapToPointer.h"
#include "../inc/hardware_configure.h"
#include "../inc/CPort.h"
#include "../inc/CFile.h"
#include "../inc/ITime.h"
#include "../inc/CGenericTime.h"
#include "../inc/CZeroTime.h"
#include "../inc/CBaseDownloader.h"
#include "../inc/CFileDownloader.h"

namespace utility {
    namespace downloader {

        CFileDownloader::CFileDownloader(size_t _pwd1, size_t _pwd2) : pwd1(_pwd1),
        pwd2(_pwd2), port(NULL), status(file_downloader_wait_start), block_size(1024), debug(false) {
        }

        CFileDownloader::~CFileDownloader() {
            port = NULL;
        }

        int CFileDownloader::Open(IPort * RdWr, size_t bsize, bool _debug) {
            CBaseDownloader::Initialize();
            int res = file_downloader_generic_error;
            if (RdWr != NULL) {
                port = RdWr;
                status = file_downloader_wait_start;
                if (bsize % sizeof (size_t) != 0) {
                    bsize = ((bsize / sizeof (size_t)) + 1) * sizeof (size_t);
                    LOG(logger, "%s:%d realign block size (%d) to %d\n", __FUNCTION__, __LINE__, block_size, sizeof (size_t));
                }
                block_size = bsize;
                LOG(logger, "%s:%d   block size (%d) \n", __FUNCTION__, __LINE__, block_size);
                debug = _debug;
                res = 0;
            } else {
                LOG(logger, "%s:%d : Invalid IO Port\n", __FUNCTION__, __LINE__);
            }
            return res;
        }

        size_t CFileDownloader::ComputeMsgCrc(size_t *msg, size_t num) {
            size_t res = -1;
            if (msg != NULL) {
                size_t i;
                res = 0;
                for (i = 0; i < num; i++) {
                    //printf("%d) B: res=%08X\n",i,res);
                    res += msg[i];
                    //printf("%d) A: res=%08X\n",i,res);
                }
            } else {
                LOG(logger, "%s:%d : Invalid msg pointer\n", __FUNCTION__, __LINE__);
            }
            return res;
        }

        size_t CFileDownloader::ComputeMsgCrc(unsigned char *msg, size_t num) {
            size_t res = -1;
            size_t *ptr = (size_t *) (msg);
            size_t dim = num / sizeof (size_t);
            if (msg != NULL) {
                size_t i;
                res = 0;
                for (i = 0; i < dim; i++) {
                    //printf("%d) B: res=%08X\n",i,res);
                    res += ptr[i];
                    //printf("%d) A: res=%08X\n",i,res);
                }
            } else {
                LOG(logger, "%s:%d : Invalid msg pointer\n", __FUNCTION__, __LINE__);
            }
            return res;
        }

        int CFileDownloader::CheckPwds(size_t * m) {
            int res = file_downloader_generic_error;
            if (m[0] == pwd1 && m[1] == pwd2) {
                res = 0;
            }
            return res;
        }

#define MINSLICE 1024

        int CFileDownloader::InternalWrite(unsigned char * buff, size_t size, size_t timeout) {
            return port->sliceWrite(buff, size, MINSLICE, timeout);
        }

        int CFileDownloader::InternalRead(unsigned char * buff, size_t size, size_t timeout) {
            return port->completeRead(buff, size, MINSLICE, timeout);
        }

        int CFileDownloader::InternalFileRead(hardware::CFile * file, unsigned char * buff, size_t size, size_t & readed) {
            size_t total_readed = 0;
            size_t tick, cur_size;
            readed = 0;
            cur_size = size;
            int result = file_downloader_generic_error;
            for (tick = 0; tick < 20; tick++) {
                result = file->Read((unsigned char *) &buff[total_readed], cur_size, readed, 0);
                if (result == IPort::io_port_partial_read_error) {
                    total_readed += readed;
                    if (total_readed == size) {
                        result = 0;
                        break;
                    } else {
                        LOG(logger, "%s:%d : partial read P:%d T:%d\n", __FUNCTION__, __LINE__, readed, total_readed);
                        cur_size -= readed;
                    }
                    software->SleepThread(20);
                } else
                    if (result == IPort::io_port_read_timeout_error) {
                    LOG(logger, "%s:%d : Timeout\n", __FUNCTION__, __LINE__);
                    continue;
                } else
                    break;
            }
            return result;
        }

        int CFileDownloader::RxStart(start_dw & m) {
            int res = file_downloader_generic_error;
            size_t result;
            result = InternalRead((unsigned char *) & m, sizeof (start_dw), 10000);
            if (result == 0) {
                size_t c_crc = ComputeMsgCrc((size_t *) & m, (sizeof (start_dw) / sizeof (size_t)) - 1);
                if (m.msg_crc == c_crc) {
                    if (CheckPwds((size_t *) & m) == 0 && m.msg_type == file_downloader_start_msg) {
                        res = 0;
                        if (debug == true) {
                            LOG(logger, "%s:%d:RX START : F(%s) : T(%d) : B(%d) : N(%d) : CRC(%08X)\n", __FUNCTION__, __LINE__, (char *) m.filename, m.total_size, m.block_size, m.num_blocks, c_crc);
                        }
                    }
                } else {
                    LOG(logger, "utility::downloader::CFileDownloader : start msg with invalid CRC R: %08X C: %08X\n", m.msg_crc, c_crc);
                }
            }
            return res;
        }

        int CFileDownloader::RxBlock(block_dw & m, unsigned char *ptr_data) {
            int res = file_downloader_generic_error;
            size_t result;
            result = InternalRead((unsigned char *) & m, sizeof (block_dw), 10000);
            if (result == 0) {
                size_t c_crc = ComputeMsgCrc((size_t *) & m, (sizeof (block_dw) / sizeof (size_t)) - 1);
                if (m.msg_crc == c_crc) {
                    //valid crc check pwd1
                    if (CheckPwds((size_t *) & m) == 0 && m.msg_type == file_downloader_block_msg) {
                        result = InternalRead((unsigned char *) ptr_data, m.block_size, 10000);
                        if (result == 0) {
                            c_crc = ComputeMsgCrc(ptr_data, m.block_size);
                            if (c_crc == m.block_crc32) {
                                res = 0;
                                if (debug == true) {
                                    LOG(logger, "%s:%d:RX BLOCK : N(%d) : S(%d) : CRC(%08X) :\n", __FUNCTION__, __LINE__, m.block_num, m.block_size, m.block_crc32);
                                }
                            } else {
                                LOG(logger, "%s:%d : block crc don't match R:%08X C:%08X\n", __FUNCTION__, __LINE__, m.block_crc32, c_crc);
                                LOGDUMP(logger, "DATA :", ptr_data, 256);
                            }
                        } else {
                            LOG(logger, "%s:%d : incomplete read of block data R: %d\n", __FUNCTION__, __LINE__, result);
                        }
                    }
                } else {
                    LOG(logger, "%s:%d : block msg with invalid CRC\n", __FUNCTION__, __LINE__);
                }
            }
            return res;
        }

        int CFileDownloader::RxClose(close_dw & m) {
            int res = file_downloader_generic_error;
            size_t result;
            result = InternalRead((unsigned char *) & m, sizeof (close_dw), 10000);
            if (result == 0) {
                if (m.msg_crc == ComputeMsgCrc((size_t *) & m, (sizeof (close_dw) / sizeof (size_t)) - 1)) {
                    if (CheckPwds((size_t *) & m) == 0 && m.msg_type == file_downloader_close_msg) {
                        res = 0;
                        if (debug == true) {
                            LOG(logger, "%s:%d:RX CLOSE\n", __FUNCTION__, __LINE__);
                        }
                    }
                } else {
                    LOG(logger, "%s:%d: close msg with invalid CRC\n", __FUNCTION__, __LINE__);
                }
            }
            return res;
        }

        int CFileDownloader::RxAckNack(size_t & error) {
            int res = file_downloader_generic_error;
            error = res;
            close_dw m;
            size_t result;
            result = InternalRead((unsigned char *) & m, sizeof (close_dw), 10000);
            if (result == 0) {
                if (m.msg_crc == ComputeMsgCrc((size_t *) & m, (sizeof (close_dw) / sizeof (size_t)) - 1)) {
                    if (CheckPwds((size_t *) & m) == 0 && m.msg_type == file_downloader_ack_msg) {
                        res = 0;
                        if (debug == true) {
                            LOG(logger, "%s:%d:RX ACK\n", __FUNCTION__, __LINE__);
                        }
                        error = 0;
                    } else
                        if (CheckPwds((size_t *) & m) == 0 && m.msg_type == file_downloader_nack_msg) {
                        if (debug == true) {
                            LOG(logger, "%s:%d:RX NACK(%d)\n", __FUNCTION__, __LINE__, m.error);
                        }
                        error = m.error;
                        res = 1;
                    } else
                        if (CheckPwds((size_t *) & m) == 0 && m.msg_type == file_downloader_close_msg) {
                        if (debug == true) {
                            LOG(logger, "%s:%d:RX CLOSE\n", __FUNCTION__, __LINE__);
                        }
                        error = 0;
                        res = 2;
                    } else
                        if (CheckPwds((size_t *) & m) == 0) {
                        if (debug == true) {
                            LOG(logger, "%s:%d:RX UNKNOW T:%08X \n", __FUNCTION__, __LINE__, m.msg_type);
                            LOGDUMP(logger, "DATA:", &m, sizeof (close_dw));
                        }
                        error = file_downloader_generic_error;
                        res = 3;
                    }
                } else {
                    LOG(logger, "%s:%d: close msg with invalid CRC\n", __FUNCTION__, __LINE__);
                    LOGDUMP(logger, "DATA:", &m, sizeof (close_dw));
                }
            } else {
                LOG(logger, "%s:%d: cannot read data : %d \n", __FUNCTION__, __LINE__, res);
            }
            return res;
        }

        int CFileDownloader::RxUser(user_dw & m, start_dw & start) {
            int res = file_downloader_generic_error;
            size_t result;
            result = InternalRead((unsigned char *) & m, sizeof (user_dw), 10000);
            if (result == 0) {
                size_t c_crc = ComputeMsgCrc((size_t *) & m, (sizeof (user_dw) / sizeof (size_t)) - 1);
                if (m.msg_type == file_downloader_user_msg && m.msg_crc == c_crc) {
                    if (CheckPwds((size_t *) & m) == 0 && m.msg_type == file_downloader_user_msg) {
                        res = 0;
                        if (debug == true) {
                            LOG(logger, "%s:%d:RX USER : U1F(%08X) : U2(%08X) : U3(%08X) : U4(%08X) : CRC(%08X)\n", __FUNCTION__, __LINE__, m.userdata.usercmd[0], m.userdata.usercmd[1], m.userdata.usercmd[2], m.userdata.usercmd[3], c_crc);
                        }
                    } else {
                        if (CheckPwds((size_t *) & m) != 0)
                            LOG(logger, "%s:%d: msg with invalid pwd\n", __FUNCTION__, __LINE__);
                        if (m.msg_type != file_downloader_user_msg)
                            LOG(logger, "%s:%d: no user msg : %08X \n", __FUNCTION__, __LINE__, m.msg_type);
                    }

                } else {
                    if (m.msg_type == file_downloader_start_msg) {
                        if (debug == true) {
                            LOG(logger, "%s:%d:RX USER : MSG TYPE=START TRY TO RX ...\n", __FUNCTION__, __LINE__);
                        }
                        // RX start msg so change input to retro compatibility
                        memcpy(&start, &m, sizeof (user_dw));
                        unsigned char *p = (unsigned char *) &start;
                        size_t remain = sizeof (start_dw) - sizeof (user_dw);
                        result = InternalRead((unsigned char *) &p[sizeof (user_dw)], remain, 10000);
                        if (result == 0) {
                            c_crc = ComputeMsgCrc((size_t *) & start, (sizeof (start_dw) / sizeof (size_t)) - 1);
                            if (start.msg_crc == c_crc) {
                                if (CheckPwds((size_t *) & start) == 0 && m.msg_type == file_downloader_start_msg) {
                                    res = 1;
                                    if (debug == true) {
                                        LOG(logger, "%s:%d:RX START : F(%s) : T(%d) : B(%d) : N(%d) : CRC(%08X)\n", __FUNCTION__, __LINE__, (char *) start.filename, start.total_size, start.block_size, start.num_blocks, c_crc);
                                    }
                                } else {
                                    if (CheckPwds((size_t *) & m) != 0)
                                        LOG(logger, "%s:%d: msg with invalid pwd\n", __FUNCTION__, __LINE__);
                                    if (m.msg_type != file_downloader_user_msg)
                                        LOG(logger, "%s:%d: no user msg : %08X \n", __FUNCTION__, __LINE__, m.msg_type);
                                }
                            }
                        } else {
                            LOG(logger, "%s:%d : res(%d)-(%d)\n", __FUNCTION__, __LINE__, result, remain);
                        }
                    } else {
                        LOG(logger, "%s:%d: unknow msg\n", __FUNCTION__, __LINE__);
                        LOGDUMP(logger, "MSG :", & m, sizeof (user_dw));
                    }
                }
            } else {
                LOG(logger, "%s:%d: read fail :%d\n", __FUNCTION__, __LINE__, result);
            }
            return res;
        }

        int CFileDownloader::replyAck(void) {
            size_t writed;
            int res = file_downloader_generic_error;
            ack_dw ack;
            ack.pwd1 = pwd1;
            ack.pwd2 = pwd2;
            ack.msg_type = (size_t) file_downloader_ack_msg;
            ack.msg_crc = this->ComputeMsgCrc((size_t *) & ack, (sizeof (ack_dw) / sizeof (size_t) - 1));
            res = port->Write((unsigned char *) &ack, sizeof (ack_dw), writed, 0);
            if (debug == true) {
                LOG(logger, "%s:%d:TX ACK\n", __FUNCTION__, __LINE__);
            }
            return res;
        }

        int CFileDownloader::replyNack(size_t error) {
            size_t writed;
            int res = file_downloader_generic_error;
            nack_dw nack;
            nack.pwd1 = pwd1;
            nack.pwd2 = pwd2;
            nack.msg_type = (size_t) file_downloader_nack_msg;
            nack.error = error;
            nack.msg_crc = this->ComputeMsgCrc((size_t *) & nack, (sizeof (nack_dw) / sizeof (size_t) - 1));
            res = port->Write((unsigned char *) &nack, sizeof (nack_dw), writed, 0);
            if (debug == true) {
                LOG(logger, "%s:%d:TX NACK\n", __FUNCTION__, __LINE__);
            }
            return res;

        }

        int CFileDownloader::TxStart(const char * rfilename, size_t total_size, size_t & total_block) {
            int res = file_downloader_generic_error;
            size_t writed;
            start_dw start;
            start.pwd1 = pwd1;
            start.pwd2 = pwd2;
            start.msg_type = file_downloader_start_msg;
            start.num_blocks = total_size / block_size;
            if ((total_size % block_size) != 0)
                start.num_blocks++;
            total_block = start.num_blocks;
            start.total_size = total_size;
            start.block_size = block_size;
            memset(start.filename, 0, sizeof (start.filename));
            strncpy((char*) (start.filename), rfilename, sizeof (start.filename));
            start.msg_crc = ComputeMsgCrc((size_t *) & start, (sizeof (start_dw) / sizeof (size_t)) - 1);
            res = port->Write((unsigned char *) &start, sizeof (start_dw), writed, 0);
            if (debug == true) {
                LOG(logger, "%s:%d:TX START : : N(%s) : T(%d) : B(%d)\n", __FUNCTION__, __LINE__, (char *) start.filename, start.total_size, start.num_blocks);
            }
            return res;
        }

        int CFileDownloader::TxBlock(size_t num, size_t crc, size_t size_eff) {
            int res = file_downloader_generic_error;
            size_t writed;
            block_dw block;
            block.pwd1 = pwd1;
            block.pwd2 = pwd2;
            block.msg_type = file_downloader_block_msg;
            block.block_num = num;
            block.block_crc32 = crc;
            block.block_size = size_eff;
            block.msg_crc = ComputeMsgCrc((size_t *) & block, (sizeof (block_dw) / sizeof (size_t)) - 1);
            res = port->Write((unsigned char *) &block, sizeof (block_dw), writed, 0);
            if (debug == true) {
                LOG(logger, "%s:%d:TX BLOCK : N(%d) : S(%d) : CRC(%08X) :\n", __FUNCTION__, __LINE__, block.block_num, block.block_size, block.block_crc32);
            }
            return res;
        }

        int CFileDownloader::TxClose(void) {
            int res = file_downloader_generic_error;
            size_t writed;
            close_dw close;
            close.pwd1 = pwd1;
            close.pwd2 = pwd2;
            close.msg_type = file_downloader_close_msg;
            close.msg_crc = ComputeMsgCrc((size_t *) & close, (sizeof (close_dw) / sizeof (size_t)) - 1);
            res = port->Write((unsigned char *) &close, sizeof (close_dw), writed, 0);
            if (debug == true) {
                LOG(logger, "%s:%d:TX CLOSE\n", __FUNCTION__, __LINE__);
            }
            return res;
        }

        int CFileDownloader::TxUser(void *buff, size_t & size) {
            int res = file_downloader_generic_error;
            size_t writed;
            user_dw cmd;
            cmd.pwd1 = pwd1;
            cmd.pwd2 = pwd2;
            cmd.msg_type = file_downloader_user_msg;
            writed = DW_FILE_USER_MAX_MSG;
            memset(cmd.userdata.usermsg, 0, writed);
            if (size < writed)
                writed = size;
            memcpy(cmd.userdata.usermsg, buff, writed);
            cmd.size = writed;
            cmd.msg_crc = ComputeMsgCrc((size_t *) & cmd, (sizeof (user_dw) / sizeof (size_t)) - 1);
            res = port->Write((unsigned char *) &cmd, sizeof (user_dw), writed, 0);
            if (debug == true) {
                LOG(logger, "%s:%d:TX USER : U1(%08X) : U2(%08X) : U3(%08X) : U4(%08X) \n", __FUNCTION__, __LINE__, cmd.userdata.usercmd[0], cmd.userdata.usercmd[1], cmd.userdata.usercmd[2], cmd.userdata.usercmd[3]);
            }
            return res;
        }

        /** entry point for thread*/
        int CFileDownloader::Recv(void) {
            CGenericTime tstart, tend;
            start_dw start;
            int res = file_downloader_generic_error;
            if (device != NULL) {
                if (port != NULL) {
                    status = file_downloader_wait_start;
                    CGenericTime::getZeroTime()->getTime(&tstart);
                    if (RxStart(start) == 0) {
                        replyAck();
                        status = file_downloader_block;
                        size_t total_size = 0;
                        size_t current_block = 0;
                        size_t writed;
                        hardware::CFile file;
                        unsigned char *data = new unsigned char[start.block_size];
                        if (file.Open((const char *) (start.filename)) == 0) {
                            while (current_block < start.num_blocks) {
                                block_dw block;
                                memset(data, 0, start.block_size);
                                if (RxBlock(block, data) == 0 && block.block_num == current_block) {
                                    file.Write(data, block.block_size, writed, 0);
                                    if (block.block_size != writed) {
                                        LOG(logger, "%s:%d:block out of sequence\n", __FUNCTION__, __LINE__);
                                        replyNack(file_downloader_file_write_fail);
                                    } else {
                                        replyAck();
                                    }
                                    current_block++;
                                    total_size += block.block_size;
                                } else
                                    if (block.block_num != current_block) {
                                    LOG(logger, "%s:%d:block out of sequence\n", __FUNCTION__, __LINE__);
                                    replyNack(file_downloader_block_out_of_sequence);
                                    break;
                                } else {
                                    replyNack(file_downloader_block_invalid);
                                    break;
                                }
                            }
                            if (total_size == start.total_size && current_block == start.num_blocks) {
                                close_dw close;
                                if (RxClose(close) == 0) {
                                    res = 0;
                                    status = file_downloader_close;
                                    replyAck();
                                } else {
                                    LOG(logger, "%s:%d:missing close msg\n", __FUNCTION__, __LINE__);
                                    status = file_downloader_status_error;
                                    replyNack(file_downloader_missing_close);
                                }
                            }
                            file.Close();
                            CGenericTime::getZeroTime()->getTime(&tend);
                        } else {
                            LOG(logger, "%s:%d:Cannot create %s file \n", __FUNCTION__, __LINE__, start.filename);
                            replyNack(file_downloader_create_remote_file);
                        }
                        delete[]data;
                    } else {
                        LOG(logger, "%s:%d:missing start msg\n", __FUNCTION__, __LINE__);
                        replyNack(file_downloader_missing_start);
                    }
                } else {
                    LOG(logger, "%s:%d:object not opened\n", __FUNCTION__, __LINE__);
                }
            } else {
                LOG(logger, "%s:%d:object not initialized\n", __FUNCTION__, __LINE__);
            }
            if (res == 0) {
                tend.subc(tstart);
                float time_elapsed = (float) (tend.getSecond())+(float) (tend.getMilliSec()) / 1000;
                size_t tr = (size_t) ((float) (start.total_size * 8) / time_elapsed);
                LOG(logger, "%s:%d:Recv %d bytes at %d bit/sec\n", __FUNCTION__, __LINE__, start.total_size, tr);
            }
            return res;
        }

        /** entry point for thread*/
        int CFileDownloader::Recv(start_dw start) {
            CGenericTime tstart, tend;
            int res = file_downloader_generic_error;
            if (device != NULL) {
                if (port != NULL) {
                    status = file_downloader_wait_start;
                    CGenericTime::getZeroTime()->getTime(&tstart);
                    status = file_downloader_block;
                    size_t total_size = 0;
                    size_t current_block = 0;
                    size_t writed;
                    hardware::CFile file;
                    unsigned char *data = new unsigned char[start.block_size];
                    if (file.Open((const char *) (start.filename)) == 0) {
                        while (current_block < start.num_blocks) {
                            block_dw block;
                            memset(data, 0, start.block_size);
                            if (RxBlock(block, data) == 0 && block.block_num == current_block) {
                                file.Write(data, block.block_size, writed, 0);
                                if (block.block_size != writed) {
                                    LOG(logger, "%s:%d:: block out of sequence\n", __FUNCTION__, __LINE__);
                                    replyNack(file_downloader_file_write_fail);
                                } else {
                                    replyAck();
                                }
                                current_block++;
                                total_size += block.block_size;
                            } else
                                if (block.block_num != current_block) {
                                LOG(logger, "%s:%d:block out of sequence\n", __FUNCTION__, __LINE__);
                                replyNack(file_downloader_block_out_of_sequence);
                                break;
                            } else {
                                replyNack(file_downloader_block_invalid);
                                break;
                            }
                        }
                        if (total_size == start.total_size && current_block == start.num_blocks) {
                            close_dw close;
                            if (RxClose(close) == 0) {
                                res = 0;
                                status = file_downloader_close;
                                replyAck();
                            } else {
                                LOG(logger, "%s:%d:missing close msg\n", __FUNCTION__, __LINE__);
                                status = file_downloader_status_error;
                                replyNack(file_downloader_missing_close);
                            }
                        }
                        file.Close();
                        CGenericTime::getZeroTime()->getTime(&tend);
                    } else {
                        LOG(logger, "%s:%d:Cannot create %s file \n", __FUNCTION__, __LINE__, start.filename);
                        replyNack(file_downloader_create_remote_file);
                    }
                    delete[]data;
                } else {
                    LOG(logger, "%s:%d:object not opened\n", __FUNCTION__, __LINE__);
                }
            } else {
                LOG(logger, "%s:%d:object not initialized\n", __FUNCTION__, __LINE__);
            }
            if (res == 0) {
                tend.subc(tstart);
                float time_elapsed = (float) (tend.getSecond())+(float) (tend.getMilliSec()) / 1000;
                size_t tr = (size_t) ((float) (start.total_size * 8) / time_elapsed);
                LOG(logger, "%s:%d:Recv %d bytes at %d bit/sec\n", __FUNCTION__, __LINE__, start.total_size, tr);
            }
            return res;
        }

        int CFileDownloader::Xmit(const char *src_file, const char * dest_file) {
            int res = file_downloader_generic_error;
            CGenericTime tstart, tend;
            struct stat st;
            size_t error;
            if (device != NULL) {
                if (port != NULL) {
                    status = file_upload_start;
                    if (stat(src_file, &st) == 0) {
                        size_t total_size, total_block = 0;
                        CGenericTime::getZeroTime()->getTime(&tstart);
                        if (TxStart(dest_file, st.st_size, total_block) == 0) {
                            hardware::CFile file;
                            status = file_upload_block;
                            total_size = st.st_size;
                            res = RxAckNack(error);
                            if (res == 0) {
                                if (file.Open(src_file, O_RDONLY, S_IRUSR | S_IRGRP | S_IROTH) == 0) {
                                    unsigned char *buff = new unsigned char[block_size];
                                    if (buff != NULL) {
                                        size_t current_block, readed;
                                        for (current_block = 0; current_block < total_block; current_block++) {
                                            memset(buff, 0, block_size);
                                            if (InternalFileRead(&file, buff, (total_size > block_size ? block_size : total_size), readed) == 0) {
                                                size_t crc = ComputeMsgCrc(buff, readed);
                                                total_size -= readed;
                                                if (TxBlock(current_block, crc, readed) == 0) {
                                                    if (InternalWrite(buff, readed, 5000) == 0) {
                                                        res = RxAckNack(error);
                                                        if (res == 0) {
                                                        } else {
                                                            LOG(logger, "%s:%d:receive a Nack on xmit block R(%d)\n", __FUNCTION__, __LINE__, res);
                                                            break;
                                                        }
                                                    } else {
                                                        LOG(logger, "%s:%d:Cannot send block data\n", __FUNCTION__, __LINE__);
                                                        replyNack(file_downloader_block_invalid);
                                                    }
                                                } else {
                                                    LOG(logger, "%s:%d:Cannot send msg block dowwnload\n", __FUNCTION__, __LINE__);
                                                    replyNack(file_downloader_block_invalid);
                                                }

                                            } else {
                                                LOG(logger, "%s:%d:Cannot read file %s\n", __FUNCTION__, __LINE__, src_file);
                                            }

                                        }
                                        delete[]buff;
                                        if (current_block == total_block) {
                                            if (TxClose() == 0) {
                                                if (RxAckNack(error) == 0) {
                                                    res = 0;
                                                    CGenericTime::getZeroTime()->getTime(&tend);
                                                } else {
                                                    LOG(logger, "%s:%d:receive a nack E: %d on xmit a close msg\n", __FUNCTION__, __LINE__, error);
                                                }
                                            } else {
                                                LOG(logger, "%s:%d:Cannot send msg close\n", __FUNCTION__, __LINE__);
                                            }
                                        }
                                    } else {
                                        LOG(logger, "%s:%d:Cannot allocate buffer to send data\n", __FUNCTION__, __LINE__);
                                    }
                                    file.Close();
                                } else {
                                    LOG(logger, "%s:%d:Cannot open file %s\n", __FUNCTION__, __LINE__, src_file);
                                }
                            } else {
                                LOG(logger, "%s:%d: receive a Nack R:%d-E:%d \n", __FUNCTION__, __LINE__, res, error);
                            }
                        } else {
                            LOG(logger, "%s:%d: Cannot send start download msg \n", __FUNCTION__, __LINE__);
                        }
                    } else {
                        LOG(logger, "%s:%d: Cannot stat file %s\n", __FUNCTION__, __LINE__, src_file);
                    }
                } else {
                    LOG(logger, "%s:%d: object not opened\n", __FUNCTION__, __LINE__);
                }
            } else {
                LOG(logger, "%s:%d: object not initialized\n", __FUNCTION__, __LINE__);
            }
            if (res == 0) {
                tend.subc(tstart);
                float time_elapsed = (float) (tend.getSecond())+(float) (tend.getMilliSec()) / 1000;
                size_t tr = (size_t) ((float) (st.st_size * 8) / time_elapsed);
                LOG(logger, "%s:%d:Xmit %d bytes at %d bit/sec\n", __FUNCTION__, __LINE__, st.st_size, tr);
            }
            return res;
        }

        int CFileDownloader::Xmit(unsigned char *mem, size_t buff_size, const char * dest_file, signalling_hook hook) {
            int res = file_downloader_generic_error;
            CGenericTime tstart, tend;
            size_t error;
            int mth_sign_thread = 0;
            if (device != NULL) {
                if (port != NULL) {
                    status = file_upload_start;
                    size_t total, total_size, total_block = 0;
                    CGenericTime::getZeroTime()->getTime(&tstart);
                    if (TxStart(dest_file, buff_size, total_block) == 0) {
                        hardware::CFile file;
                        status = file_upload_block;
                        total = 0;
                        total_size = buff_size;
                        res = RxAckNack(error);
                        if (res == 0) {
                            unsigned char *buff = new unsigned char[block_size];
                            if (buff != NULL) {
                                size_t current_block, readed;
                                for (current_block = 0; current_block < total_block; current_block++) {
                                    total_size > block_size ? readed = block_size : readed = total_size;
                                    memcpy(buff, &mem[total], readed);
                                    size_t crc = ComputeMsgCrc(buff, readed);
                                    total_size -= readed;
                                    if (TxBlock(current_block, crc, readed) == 0) {
                                        if (InternalWrite(buff, readed, 5000) == 0) {
                                            res = RxAckNack(error);
                                            if (res == 0) {
                                                total += readed;
                                            } else {
                                                LOG(logger, "%s:%d: receive a Nack on xmit block R(%d)-E(%d)\n", __FUNCTION__, __LINE__, res, error);
                                                LOGDUMP(logger, "DATA:", buff, 256);
                                                break;
                                            }
                                            if (hook != NULL) {
                                                //LOG(logger, ">>>>>>>>>>>>>>>>> %d :%d\n", total, buff_size);
                                                if (VALID_HANDLE(mth_sign_thread)) {
                                                    software->DestroyThread(mth_sign_thread);
                                                }
                                                size_t per100 = ((total * 100) / buff_size);
                                                software->CreateThread((void (*)()) hook, (void *) (per100), mth_sign_thread);
                                            }
                                        } else {
                                            LOG(logger, "%s:%d: Cannot send block data\n", __FUNCTION__, __LINE__);
                                            replyNack(file_downloader_block_invalid);
                                        }
                                    } else {
                                        LOG(logger, "%s:%d: Cannot send msg block dowwnload\n", __FUNCTION__, __LINE__);
                                        replyNack(file_downloader_block_invalid);
                                    }
                                }
                                if (hook != NULL) {
                                    if (VALID_HANDLE(mth_sign_thread)) {
                                        software->DestroyThread(mth_sign_thread);
                                    }
                                    software->CreateThread((void (*)()) hook, (void *) 100, mth_sign_thread);
                                }
                                delete[]buff;
                                if (current_block == total_block) {
                                    if (TxClose() == 0) {
                                        if (RxAckNack(error) == 0) {
                                            res = 0;
                                            CGenericTime::getZeroTime()->getTime(&tend);
                                        } else {
                                            LOG(logger, "%s:%d: receive a nack E: %d on xmit a close msg\n", __FUNCTION__, __LINE__, error);
                                        }
                                    } else {
                                        LOG(logger, "%s:%d: Cannot send msg close\n", __FUNCTION__, __LINE__);
                                    }
                                }
                            } else {
                                LOG(logger, "%s:%d: Cannot allocate buffer to send data\n", __FUNCTION__, __LINE__);
                            }
                        } else {
                            LOG(logger, "%s:%d: receive a Nack R:%d-E:%d \n", __FUNCTION__, __LINE__, res, error);
                        }
                    } else {
                        LOG(logger, "%s:%d: Cannot send start download msg \n", __FUNCTION__, __LINE__);
                    }
                } else {
                    LOG(logger, "%s:%d: object not opened\n", __FUNCTION__, __LINE__);
                }
                if (VALID_HANDLE(mth_sign_thread)) {
                    software->DestroyThread(mth_sign_thread);
                }
            } else {
                LOG(logger, "%s:%d: object not initialized\n", __FUNCTION__, __LINE__);
            }
            if (res == 0) {
                tend.subc(tstart);
                float time_elapsed = (float) (tend.getSecond())+(float) (tend.getMilliSec()) / 1000;
                size_t tr = (size_t) ((float) (buff_size * 8) / time_elapsed);
                LOG(logger, "%s:%d:Xmit %d bytes at %d bit/sec\n", __FUNCTION__, __LINE__, buff_size, tr);
            }
            return res;
        }

        /** entry point for thread*/
        int CFileDownloader::UserRecv(void * buff, size_t & size /* input/output*/) {
            user_dw cmd;
            start_dw start;
            int res = file_downloader_generic_error;
            if (device != NULL) {
                if (port != NULL) {
                    status = file_user_start;
                    res = RxUser(cmd, start);
                    if (res == 0) { //user msg
                        replyAck();
                        size_t dim = DW_FILE_USER_MAX_MSG;
                        if (cmd.size < dim)
                            dim = cmd.size;
                        if (size < dim)
                            dim = size;
                        memcpy(buff, cmd.userdata.usermsg, dim);
                        size = dim;
                        res = 0;
                    } else
                        if (res == 1) { //start msg
                        replyAck();
                        size_t dim = sizeof (start_dw);
                        if (size < dim)
                            dim = size;
                        memcpy(buff, &start, dim);
                        size = dim;
                        res = 1;
                    } else {
                        LOG(logger, "%s:%d:missing user msg\n", __FUNCTION__, __LINE__);
                        replyNack(file_downloader_missing_start);
                    }
                } else {
                    LOG(logger, "%s:%d:object not opened\n", __FUNCTION__, __LINE__);
                }
            } else {
                LOG(logger, "%s:%d:object not initialized\n", __FUNCTION__, __LINE__);
            }
            return res;
        }

        int CFileDownloader::UserXmit(void *buff, size_t & size) {
            int res = file_downloader_generic_error;
            size_t error;
            if (device != NULL) {
                if (port != NULL) {
                    status = file_user_start;
                    if (TxUser(buff, size) == 0) {
                        res = RxAckNack(error);
                        if (res == 0) {
                            status = file_user_stop;
                            res = 0;
                        } else {
                            LOG(logger, "%s:%d:receive a Nack R:%d-E:%d \n", __FUNCTION__, __LINE__, res, error);
                        }
                    } else {
                        LOG(logger, "%s:%d:Cannot send user  msg \n", __FUNCTION__, __LINE__);
                    }
                } else {
                    LOG(logger, "%s:%d:object not opened\n", __FUNCTION__, __LINE__);
                }
            } else {
                LOG(logger, "%s:%d:object not initialized\n", __FUNCTION__, __LINE__);
            }
            return res;
        }
    }
}

