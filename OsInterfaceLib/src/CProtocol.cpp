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
#include <string.h>
#include <time.h>
#include <string>
#include <sstream>

#include "../inc/IPort.h"
#include "../inc/CBasePort.h"
#include "../inc/CPort.h"
#include "../inc/IRawData.h"
#include "../inc/CRawData.h"
#include "../inc/IMessage.h"
#include "../inc/ISoftware.h"
#include "../inc/ILogger.h"
#include "../inc/IHandler.h"
#include "../inc/ITime.h"
#include "../inc/CGenericTime.h"
#include "../inc/CBaseHandler.h"
#include "../inc/IProtocol.h"
#include "../inc/IMapToPointer.h"
#include "../inc/ICommPort.h"
#include "../inc/IHardware.h"
#include "../inc/IDevice.h"
#include "../inc/ILogger.h"
#include "../inc/CDefaultLogger.h"
#include "../inc/CGenericDevice.h"
#include "../inc/IMessageFactory.h"
#include "../inc/CCircularBuffer.h"
#include "../inc/CGenericMessage.h"
#include "../inc/CGenericHardware.h"
#include "../inc/IMessageManager.h"
#include "../inc/CDefaultDevice.h"
#include "../inc/IBlock.h"
#include "../inc/CBaseBlock.h"
#include "../inc/CGenericMessageManager.h"
#include "../inc/CMessageManager.h"
#include "../inc/hardware_configure.h"
#include "../inc/CProtocol.h"
#include "../inc/CMapToPointer.h"



namespace protocol {

    static void * handlerTrasmitEntry(void *param) {
        CProtocol * p = utility::obj_cast<protocol::CProtocol *>(param);
        return p->handlerTrasmit(p);
    }

    static void * handlerReceiveEntry(void *param) {
        CProtocol * p = utility::obj_cast<protocol::CProtocol *>(param);
        return p->handlerReceive(p);
    }

    void * CProtocol::handlerTrasmit(CProtocol * p) {
        IMessage * ptr[1];
        unsigned char *out;
        unsigned int size;
        unsigned int writed = 0;
        size_t sTx = 0;
        if (device == NULL) {
            return NULL;
        }
        LOG(logger, "%s:%d:%08X : start trasmit thread\n", __FUNCTION__, __LINE__, this);
        IMessageManager * MM = messages::CGenericMessageManager::getMessageManager();
        if (MM != NULL) {
            IMessageFactory* factory = MM->getFactory();
            if (factory != NULL) {

                while (1) {
                    if (mph_Tx <= 0)break;
                    software->SemphWait(p->mph_Tx); // wait while dispatchMessage do sem_post
                    if (exit_from_tx == true)
                        break;
                    ptr[0] = NULL;
                    software->MutexLock(p->mtx_Tx);
                    sTx = Tx.Size();
                    if (sTx > 0)
                        Tx.Remove(ptr, 1);
                    software->MutexUnlock(p->mtx_Tx);
                    if (sTx == 0)
                        continue;
                    if (ptr[0] != NULL) {
                        factory->toRaw(&out, size, ptr[0]);
                        if (wr_Port > 0)
                            hardware::CPort::Write(wr_Port, out, size, writed, 0);
                        if (writed != size) {
                            //first retry
                            if (wr_Port > 0)
                                hardware::CPort::Write(wr_Port, &out[writed], size - writed, writed, 0);
                            LOG(logger, "%s:%d:%08X : write queue full\n", __FUNCTION__, __LINE__, this);
                            // next timeout retry this message
                        }
                        MM->DestroyMsg(&ptr[0]);
                    } else {
                        LOG(logger, "%s:%d:%08X : warning pointer to msg is NULL\n", __FUNCTION__, __LINE__, this);
                    }
                }
            }
        } else {
            LOG(logger, "%s:%d:%08X : No MessageFactory Avaible\n", __FUNCTION__, __LINE__, this);
        }
        LOG(logger, "%s:%d:%08X : end trasmit thread\n", __FUNCTION__, __LINE__, this);
        return NULL;
    }

    CProtocol::CProtocol() {
        exit_from_tx = exit_from_rx = false;
        mtx_Rx = mtx_Tx = -1;
        mph_Rx = mph_Tx = -1;
        rd_Port = -1;
        wr_Port = -1;
        enableThreadRx = enableThreadTx = 0;
    }

    CProtocol::~CProtocol() {
        exit_from_rx = true;
        exit_from_tx = true;
        mtx_Rx = mtx_Tx = -1;
        mph_Rx = mph_Tx = -1;
        rd_Port = -1;
        wr_Port = -1;
        enableThreadRx = enableThreadTx = 0;
    }

    int CProtocol::CreateRxObject(void) {
        int res = (int) IProtocol::protocol_generic_error;
        if (device != NULL) {
            if (software->CreateBinaryMutex(mtx_Rx) == 0) {
                if (software->CreateThread((void (*)()) handlerReceiveEntry, this, thread_rx) != 0) {//fail
                    res = (int) IProtocol::protocol_create_rx_process_error;
                    LOG(logger, "%s:%d:%08X : cannot create rx thread\n", __FUNCTION__, __LINE__, this);
                    Close();
                } else {
                    res = 0;
                }

            } else {
                res = (int) IProtocol::protocol_create_mutex_rx_error;
                LOG(logger, "%s:%d:%08X : cannot create rx mutex\n", __FUNCTION__, __LINE__, this);
            }
        }
        return res;
    }

    int CProtocol::CreateTxObject(void) {
        int res = (int) IProtocol::protocol_generic_error;
        if (device != NULL) {
            if (software->CreateBinaryMutex(mtx_Tx) == 0) {
                if (software->CreateSemph(mph_Tx, 0) == 0) {
                    if (software->CreateThread((void (*)()) handlerTrasmitEntry, this, thread_tx) == 0) {
                        event_Tx(); //try handler TX
                        res = 0;
                    }
                } else {
                    res = (int) IProtocol::protocol_create_tx_process_error;
                    LOG(logger, "%s:%d:%08X : cannot create tx thread\n", __FUNCTION__, __LINE__, this);
                }
            } else {
                res = (int) IProtocol::protocol_create_semph_tx_error;
                LOG(logger, "%s:%d:%08X : cannot create tx semph\n", __FUNCTION__, __LINE__, this);
            }
        } else {
            res = (int) IProtocol::protocol_create_mutex_tx_error;
            LOG(logger, "%s:%d:%08X : cannot create tx mutex\n", __FUNCTION__, __LINE__, this);
        }
        return res;
    }

    int CProtocol::DestroyTxObject(void) {
        int res = (int) IProtocol::protocol_generic_error;
        if (device != NULL) {
            event_Tx();
            exit_from_tx = true;
            software->SleepThread(10);
            if (software != NULL) {
                IMessageManager *msgManager = messages::CGenericMessageManager::getMessageManager();
                if (VALID_HANDLE(thread_tx)) {
                    if (software->DestroyThread(thread_tx) != 0) {
                        LOG(logger, "%s:%d:%08X : cannot terminate tx thread\n", __FUNCTION__, __LINE__, this);
                        res = (int) IProtocol::protocol_destroy_tx_process_error;
                    }
                }
                if (VALID_HANDLE(mtx_Tx)) {
                    software->MutexUnlock(mtx_Tx);
                    software->SleepThread(10);
                    software->DestroyMutex(mtx_Tx);
                    mtx_Tx = -1;
                }
                if (VALID_HANDLE(mph_Tx)) {
                    software->SemphPost(mph_Tx);
                    software->SleepThread(10);
                    software->DestroySemph(mph_Tx);
                    mph_Tx = -1;
                }
                std::size_t i;
                for (i = 0; i < Tx.Size(); i++) {
                    IMessage *msg = NULL;
                    if (Tx.Remove(&msg, 1) == true) {
                        msgManager->DestroyMsg(&msg);
                    }
                }
                res = 0;
            } else {
                LOG(logger, "%s:%d:%08X : no software avaiable\n", __FUNCTION__, __LINE__, this);
                res = (int) IProtocol::protocol_software_error;
            }
        }
        return res;
    }

    int CProtocol::DestroyRxObject(void) {
        int res = (int) IProtocol::protocol_generic_error;
        if (device != NULL) {
            exit_from_rx = true;
            software->SleepThread(200);
            if (software != NULL) {
                IMessageManager *msgManager = messages::CGenericMessageManager::getMessageManager();
                if (VALID_HANDLE(thread_rx)) {
                    if (software->DestroyThread(thread_rx) != 0) {
                        LOG(logger, "%s:%d:%08X : cannot terminate rx thread\n", __FUNCTION__, __LINE__, this);
                        res = (int) IProtocol::protocol_destroy_rx_process_error;
                    }
                }
                if (VALID_HANDLE(mtx_Rx)) {
                    software->MutexUnlock(mtx_Rx);
                    software->SleepThread(10);
                    software->DestroyMutex(mtx_Rx);
                    mtx_Rx = -1;
                }
                std::size_t i;
                for (i = 0; i < Rx.Size(); i++) {
                    IMessage *msg = NULL;
                    if (Rx.Remove(&msg, 1) == true) {
                        msgManager->DestroyMsg(&msg);
                    }
                }
                res = 0;
            } else {
                LOG(logger, "%s:%d:%08X : no software avaiable\n", __FUNCTION__, __LINE__, this);
                res = (int) IProtocol::protocol_software_error;
            }
        }
        return res;
    }

    int CProtocol::Open(int enableRX, int enableTX) {
        ::device::CBaseBlock::Initialize();
        int res = 0;
        if (device == NULL) {
            return IProtocol::protocol_device_error;
        }
        if (enableRX == 0 && enableTX == 0) {
            return IProtocol::protocol_invalid_tx_rx_managment;
        }
        enableThreadRx = enableRX;
        enableThreadTx = enableTX;
        thread_rx = thread_tx = -1;
        //LOG(logger, "%s:%d: %08X :  +\n",__func__ , this);
        if (device != NULL) {
            if (software != NULL) {
                if (enableThreadRx != 0) {
                    res = CreateRxObject();
                }
                if (res == 0 && enableThreadTx != 0) {
                    res = CreateTxObject();
                }

            } else {
                res = (int) IProtocol::protocol_software_error;
                LOG(logger, "%s:%d:%08X :  hardware not avaiable\n", __FUNCTION__, __LINE__, this);
            }
        }
        //LOG(logger, "%s:%d: %08X :  -:%d\n",__FUNCTION__,__LINE__, this,res);
        return res;
    }

    int CProtocol::Close(void) {
        //LOG(logger, "%s:%d: %08X :  +\n",__func__, this);
        int res = (int) (IProtocol::protocol_generic_error);
        software->MutexUnlock(mtx_Rx);
        software->MutexUnlock(mtx_Tx);
        software->SemphPost(mph_Rx);
        software->SemphPost(mph_Tx);

        if (this->enableThreadRx != 0) {
            res = DestroyRxObject();
        }
        if (this->enableThreadTx != 0) {
            res |= DestroyTxObject();
        }

        //LOG(logger, "%s:%d: %08X : -:%d\n",__func__, this, res);
        return res;
    }

    int CProtocol::dispatchMessage(IMessage * msg) {
        int res = -1;
        IMessage * p[1];
        if (software != NULL) {
            software->MutexLock(mtx_Tx);
            p[0] = msg;
            Tx.Insert(p, 1);
            software->MutexUnlock(mtx_Tx);
            //LOG(logger, "protocol::CProtocol::dispatchMessage():\n");
            //LOG(logger, msg->toString());
        } else {
            LOG(logger, "%s:%d:%08X : internal error\n", __FUNCTION__, __LINE__, this);
            res = (int) IProtocol::protocol_software_error;
        }
        event_Tx();
        return res;
    }

    int CProtocol::listenerMessage(IMessage * msg) {
        int res = -1;
        IMessage * p[1];
        if (software != NULL) {
            software->MutexLock(mtx_Rx);
            p[0] = msg;
            Rx.Insert(p, 1);
            software->MutexUnlock(mtx_Rx);
        } else {
            LOG(logger, "%s:%d:%08X : internal error\n", __FUNCTION__, __LINE__, this);
            res = (int) IProtocol::protocol_software_error;
        }
        //LOG(logger, "protocol::CProtocol::listenerMessage():\n");
        //LOG(logger, msg->toString());
        event_Rx();
        return res;
    }

    void CProtocol::event_Tx(void) {
        if (software != NULL) {
            //LOG(logger, ">>>>>>>>>>>>>>>>>>>>>>>>>>EVENT TX\n");
            software->SemphPost(mph_Tx);
        }
    }

    void CProtocol::event_Rx(void) {
        if (software != NULL) {
            //LOG(logger, ">>>>>>>>>>>>>>>>>>>>>>>>>>EVENT RX\n");
            software->SemphPost(mph_Rx);
        }
    }

    IMessage * CProtocol::GetLastRxMessage(void) {
        IMessage * p[1];
        p[0] = NULL;
        software->MutexLock(mtx_Rx);
        Rx.Remove(p, 1);
        software->MutexUnlock(mtx_Rx);
        return p[0];
    }

    void CProtocol::isRxEmpthy(int *res) {
        if (res != NULL) {
            software->MutexLock(mtx_Rx);
            *res = Rx.Size();
            software->MutexUnlock(mtx_Rx);
            //LOG(logger, "RX Queue size =%d\n", *res);
        }
    }
}
