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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "../inc/IPort.h"
#include "../inc/CBasePort.h"
#include "../inc/CPort.h"
#include "../inc/ILogger.h"
#include "../inc/ISoftware.h"
#include "../inc/IHardware.h"
#include "../inc/IDevice.h"
#include "../inc/CGenericHardware.h"
#include "../inc/CDefaultHardware.h"
#include "../inc/CDefaultLogger.h"
#include "../inc/IBlock.h"
#include "../inc/CBaseBlock.h"

#include "../inc/CCryptoProto.h"



namespace crypto {

    CCryptoProtocol::CCryptoProtocol() {
        port = NULL;
        cypher = NULL;
        debug = false;
        memIO = NULL;
        read_timeout = 10000;

    }

    CCryptoProtocol::~CCryptoProtocol() {
        port = NULL;
        cypher = NULL;
        debug = false;
        if (memIO != NULL) {
            delete []memIO;
            memIO = NULL;
        }
    }

    int CCryptoProtocol::Recv(unsigned char *out, size_t max_len, size_t & len) {
        int res = ICryptoProtocol::generic_error;
        size_t size_alloc;
        crypto_protocol_msg msg;
        res = InternalRead((unsigned char *) &msg, sizeof (crypto_protocol_msg), read_timeout);
        if (res == 0) {
            if (msg.cmd == cmd_data_msg) {
                size_alloc = (msg.blocks * 16);
                if (size_alloc > 0) {
                    unsigned char *decode = allocBuffer(size_alloc);
                    if (decode != NULL) {
                        res = InternalRead(decode, size_alloc, read_timeout);
                        if (res == 0) {
                            if (ComputeCrc32(msg, decode, size_alloc)+ msg.real_size == msg.crc ) {
                                len = 0;
								if (max_len < size_alloc) {
                                    unsigned char *temp = new unsigned char[size_alloc];
                                    if (temp != NULL) {
										res = cypher->Decrypt(decode, size_alloc, temp, size_alloc);
                                        if (res == 0) {
											memcpy(out, temp, max_len);
                                        }
                                        delete []temp;
                                    } else {
                                        LOG(logger, "%s:%d error on encrypting data\n", __FUNCTION__, __LINE__)
                                        res = ICryptoProtocol::dencrypt_data;
                                    }
                                } else {
									res = cypher->Decrypt(decode, size_alloc, out, max_len);
                                }
                                if (res == 0) {
                                    len = msg.real_size;
                                    //if (debug != 0) {
                                    //    LOG(logger, "%s:%d RXDATA(%d)\n", __FUNCTION__, __LINE__, len);
                                        //LOGDUMP(logger, "DATA:", out, len)
                                    //}
                                    replyAck();
                                } else {
                                    LOG(logger, "%s:%d error on encrypting data\n", __FUNCTION__, __LINE__)
                                    res = ICryptoProtocol::dencrypt_data;
                                }
                            } else {
                                LOG(logger, "%s:%d error bad CRC\n", __FUNCTION__, __LINE__);
                                res = ICryptoProtocol::bad_crc;
                            }

                        } else {
                            LOG(logger, "%s:%d error rxh no data\n", __FUNCTION__, __LINE__);
                            res = ICryptoProtocol::reading_error;
                        }
                        freeBuffer(&decode, size_alloc);
                    } else {
                        LOG(logger, "%s:%dnon memory to allocate dencryption buffer\n", __FUNCTION__, __LINE__);
                        res = ICryptoProtocol::no_memory_cypher;
                    }
                } else {
                    LOG(logger, "%s:%d error riceve a cmd data with no data\n", __FUNCTION__, __LINE__);
                    res = ICryptoProtocol::reading_invalid_msg;
                }
            } else {
                LOG(logger, "%s:%d error cmd msg invalid\n", __FUNCTION__, __LINE__);
                LOGDUMP(logger, "RX:", &msg, sizeof (crypto_protocol_msg));
                res = ICryptoProtocol::reading_error;
            }
            if (res != 0) {
                replyNack(-res);
            }
        } else {
            LOG(logger, "%s:%d error on read header\n", __FUNCTION__, __LINE__);
        }
        return res;
    }

    int CCryptoProtocol::Xmit(unsigned char *in, size_t len) {
        int res = ICryptoProtocol::generic_error;
        size_t size_alloc, error;
        unsigned char *encode = allocBuffer(len, size_alloc);
        if (encode != NULL) {
            res = cypher->Encrypt(in, len, encode, size_alloc);
            if (res == 0) {
                crypto_protocol_msg msg;
                msg.blocks = size_alloc / 16;
                msg.cmd = cmd_data_msg;
                msg.real_size = len;
                msg.crc = ComputeCrc32(msg, encode, size_alloc);
                msg.crc += msg.real_size;
                res = InternalWrite((unsigned char *) &msg, sizeof (crypto_protocol_msg), read_timeout);
                if (res == 0) {
                    res = InternalWrite(encode, size_alloc, read_timeout);
                    if (res == 0) {
                        res = RxAckNack(error);
                        if (res != 0) {
                            res = error;
                            LOG(logger, "%s:%d NACK(%d)\n", __FUNCTION__, __LINE__, res);
                        } //else {
                            //if (debug != 0) {
                            //    LOG(logger, "%s:%d TXDATA(%d)\n", __FUNCTION__, __LINE__, len);
                            //LOGDUMP(logger, "DATA:", in, len)
                            //}
                        //}
                    } else {
                        LOG(logger, "%s:%d error on writing data\n", __FUNCTION__);
                        res = ICryptoProtocol::writing_error;
                    }
                } else {
                    LOG(logger, "%s:%d error on writing header\n", __FUNCTION__);
                    res = ICryptoProtocol::writing_error;
                }
            } else {
                LOG(logger, "%s:%d error on encrypting data\n", __FUNCTION__)
                res = ICryptoProtocol::encrypt_data;
            }
            freeBuffer(&encode, size_alloc);
        } else {
            LOG(logger, "%s:%d no memory to allocate encryption buffer\n", __FUNCTION__);
            res = ICryptoProtocol::no_memory_cypher;
        }
        return res;
    }

    unsigned char * CCryptoProtocol::allocBuffer(size_t size_allocated) {
        unsigned char *buffer;
        //buffer = (unsigned char *)malloc(size_allocated);
        if (size_allocated < max_mem_size)
            buffer = memIO;
        else
            buffer = NULL;
        return buffer;
    }

    unsigned char * CCryptoProtocol::allocBuffer(size_t req_size, size_t & size_allocated) {
        size_allocated = (((req_size / 16) + 1)*16);
        return allocBuffer(size_allocated);
    }

    int CCryptoProtocol::freeBuffer(unsigned char ** buff, size_t size) {
        int res = ICryptoProtocol::generic_error;
        if (buff != NULL) {
            if (*buff != memIO) {
                LOG(logger, "%s:%dERROR deallocate pointer != memIO\n");
                exit(-1);
            }
            if (*buff != NULL) {
                memset(*buff, 0, size);
                //free(*buff);
                *buff = NULL;
                res = 0;
            }
        }
        return res;
    }

    int CCryptoProtocol::InternalWrite(unsigned char * buff, size_t size, size_t timeout) {
        return port->sliceWrite(buff, size, 1024, timeout);
    }

    int CCryptoProtocol::InternalRead(unsigned char * buff, size_t size, size_t timeout) {
        return port->completeRead(buff, size, 1024, timeout);
    }

    size_t CCryptoProtocol::ComputeCrc32(const CCryptoProtocol::crypto_protocol_msg & msg, const unsigned char *ptr, size_t size) {
        size_t crc = 0xfedecefa;
        crc += msg.blocks;
        crc += msg.cmd;
        if (ptr != NULL && size > 0) {
            while (size-- > 0) {
                crc += *ptr++;
            }
        }
        return crc;
    }

    int CCryptoProtocol::RxAckNack(size_t & error) {
        int res = ICryptoProtocol::generic_error;
        error = res;
        unsigned char rxmsg[sizeof (crypto_protocol_msg)];
        size_t result;
        result = InternalRead(rxmsg, sizeof (crypto_protocol_msg), read_timeout);
        if (result == 0) {
            crypto_protocol_msg *msg = (crypto_protocol_msg *) (rxmsg);
            size_t compute_crc = ComputeCrc32(*msg, NULL, 0);
            if (msg->crc == compute_crc) {
                if (msg->cmd == cmd_ack_msg) {
                    res = 0;
                    //if (debug == true) {
                    //    LOG(logger, "%s:RX ACK\n", __FUNCTION__);
                    //}
                    error = 0;
                } else
                    if ((msg->cmd & cmd_nack_mask_cmd) == cmd_nack_msg) {
                    //if (debug == true) {
                    //    LOG(logger, "%s:RX NACK(%d)\n", __FUNCTION__, error);
                    //}
                    error = -(msg->cmd & cmd_nack_mask_data);
                    res = 1;
                } else {
                    if (debug == true) {
                        LOG(logger, "%s:RX UNKNOW T:%08X \n", __FUNCTION__, msg->cmd);
                        LOGDUMP(logger, "DATA:", rxmsg, sizeof (crypto_protocol_msg));
                    }
                    error = ICryptoProtocol::generic_error;
                    res = 3;
                }
            } else {
                LOG(logger, "%s:%d Rx msg with invalid CRC_R(%08x)-CRC_C(%08X)\n", __FUNCTION__, __LINE__, msg->crc, compute_crc);
                LOGDUMP(logger, "DATA:", rxmsg, sizeof (crypto_protocol_msg));
            }
        } else {
            LOG(logger, "%s:%d cannot read data : %d \n", __FUNCTION__, __LINE__, res);
        }
        return res;
    }

    int CCryptoProtocol::replyAck(void) {
        int res = ICryptoProtocol::generic_error;
        crypto_protocol_msg msg;
        msg.blocks = 0;
        msg.cmd = cmd_ack_msg;
        msg.crc = ComputeCrc32(msg, NULL, 0);
        res = InternalWrite((unsigned char *) &msg, sizeof (crypto_protocol_msg), read_timeout);
        if (debug == true) {
            LOG(logger, "%s:TX ACK(%d)\n", __FUNCTION__, res);
        }
        return res;
    }

    int CCryptoProtocol::replyNack(size_t error) {
        int res = ICryptoProtocol::generic_error;
        crypto_protocol_msg msg;
        msg.blocks = 0;
        msg.cmd = (cmd_msg) (cmd_nack_msg | (cmd_nack_mask_data & error));
        msg.crc = ComputeCrc32(msg, NULL, 0);
        res = InternalWrite((unsigned char *) &msg, sizeof (crypto_protocol_msg), read_timeout);
        if (debug == true) {
            LOG(logger, "%s:TX NACK(%08X-%d)\n", __FUNCTION__, error, res);
        }
        return res;
    }

    int CCryptoProtocol::Open(IPort * RdWr, ICypher *_cypher, size_t int_mem, size_t _read_timeout, bool _debug) {
        int res = ICryptoProtocol::generic_error;
        device::CBaseBlock::Initialize();
        debug = _debug;
        if (RdWr != NULL) {
            port = RdWr;
            if (_cypher != NULL) {
                cypher = _cypher;
                max_mem_size = int_mem;
                memIO = new unsigned char [int_mem];
                if (memIO != NULL) {
                    read_timeout = _read_timeout;
                    res = 0;
                } else {
                    LOG(logger, "%s:%d : No memory\n", __FUNCTION__, __LINE__);
                    res = ICryptoProtocol::no_memory;
                }
            } else {
                LOG(logger, "%s:%d : Invalid cypher\n", __FUNCTION__, __LINE__);
                res = ICryptoProtocol::invalid_cypher;
            }
        } else {
            LOG(logger, "%s:%d : Invalid IO port\n", __FUNCTION__, __LINE__);
            res = ICryptoProtocol::invalid_io_port;
        }
        return res;
    }


}

