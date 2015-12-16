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

#include "../inc/IRawData.h"
#include "../inc/IMessage.h"
#include "../inc/ITime.h"
#include "../inc/CGenericTime.h"
#include "../inc/IMessageManager.h"
#include "../inc/CGenericMessageManager.h"
#include "../inc/CMessageManager.h"
#include "../inc/CGenericMessage.h"
#include "../inc/CRawData.h"
#include "../inc/IMessageFactory.h"


namespace messages {

    int CMessageManager::UID = 0;

    CMessageManager::CMessageManager() {
        factory = NULL;
    }

    CMessageManager::~CMessageManager() {
        factory = NULL; // to delete why as set the factory on caller    
    }

    void CMessageManager::DestroyMsg(IMessage ** msg) {

        if (msg != NULL && factory != NULL) {
            factory->DestroyMsg(msg);
        }
        *msg = NULL;
    }

    int CMessageManager::setFactory(IMessageFactory *nfactory) {
        if (factory != NULL) {
            delete factory;
        }
        factory = nfactory;
        return 0;
    }

    IMessageFactory * CMessageManager::getFactory(void) {
        return factory;
    }

    IMessage *CMessageManager::CreateMsg(unsigned int message_type, unsigned int timeout, int numRetry) {
        IMessage *msg = NULL;
        if (factory != NULL) {
            msg = factory->CreateMsg(message_type);
        }
        return msg;
    }

    //#define CMSG(A)    msg = messages::A::Create(UID++,id,timeout,numRetry,_data);
    //TODO
    //#define CMSG(A)      
    /*
        IMessage *CMessageManager::CreateMsg(unsigned int message_type, unsigned int timeout, int numRetry) {
            IRawData *_data = new utility::CRawData();
            IMessage *msg = NULL;
            switch (id) {
                case PN_EMPTY:
                    break;
                case PN_NOTY_RESET:
                    CMSG(CPnpNotifyResetMsg);
                    break;
                case PN_NOTY_ALIVE:
                    CMSG(CPnpNotifyAliveMsg);
                    break;
                case PN_NOTY_SCANINFO:
                    CMSG(CPnpNotifyScanInfoMsg);
                    break;
                case PN_NOTY_ENSINFO:
                    CMSG(CPnpNotifyEnsInfoMsg);
                    break;
                case PN_NOTY_SRVINFO:
                    CMSG(CPnpNotifySvrInfoMsg);
                    break;
                case PN_NOTY_SRVCOMP:
                    CMSG(CPnpNotifySvrCompMsg);
                    break;
                case PN_NOTY_PLAYINFO:
                    CMSG(CPnpNotifyPlayInfoMsg);
                    break;
                case PN_NOTY_DLS:
                    CMSG(CPnpNotifyDlsMsg);
                    break;
                case PN_NOTY_DATA:
                    CMSG(CPnpNotifyDataMsg);
                    break;
                case PN_NOTY_SIGNAL_QUALITY:
                    CMSG(CPnpNotifySignalQualityMsg);
                    break;
                case PN_NOTY_TUNER_STATUS:
                    CMSG(CPnpNotifyTunerStatusMsg);
                    break;
                case PN_NOTY_BER_MEASUREMENT:
                    CMSG(CPnpNotifyBerMeasurementsMsg);
                    break;
                case PN_NOTY_DIAG_MSG:
                    CMSG(CPnpNotifyDiagMsg);
                    break;
                case PN_NOTY_LLHI:
                    CMSG(CPnpNotifyLLHIMsg);
                    break;
                case PN_NOTY_RDS_RADIOTEXT:
                    CMSG(CPnpNotifyRdsRadioTextMsg);
                    break;
                case PN_NOTY_RDS_PSN:
                    CMSG(CPnpNotifyRdsPsnMsg);
                    break;
                case PN_NOTY_RDS_PTY:
                    CMSG(CPnpNotifyRdsPtyMsg);
                    break;
                case PN_CMD_SETCONFIG:
                    CMSG(CPnpCommandSetConfigMsg);
                    break;
                case PN_CMD_POWER:
                    CMSG(CPnpCommandPowerMsg);
                    break;
                case PN_CMD_ACTSTOP:
                    CMSG(CPnpCommandActStopMsg);
                    break;
                case PN_CMD_TUNE:
                    CMSG(CPnpCommandTuneMsg);
                    break;
                case PN_CMD_SRCH:
                    CMSG(CPnpCommandSrchMsg);
                    break;
                case PN_CMD_SELECT:
                    CMSG(CPnpCommandSelectMsg);
                    break;
                case PN_CMD_SETDATA:
                    CMSG(CPnpCommandSetDataMsg);
                    break;
                case PN_CMD_SCAN:
                    CMSG(CPnpCommandScanMsg);
                    break;
                case PN_CMD_GET_VERSION:
                    CMSG(CPnpCommandGetVersionMsg);
                    break;
                case PN_CMD_TUNER_STATUS:
                    CMSG(CPnpCommandTunerStatusMsg);
                    break;
                case PN_CMD_BER_MEASUREMENT:
                    CMSG(CPnpCommandBerMeasurementMsg);
                    break;
                case PN_CMD_DRC:
                    CMSG(CPnpCommandDrcMsg);
                    break;
                case PN_CMD_SELECT_AUDIO_SERVICE:
                    CMSG(CPnpCommandSelectAudioServiceMsg);
                    break;
                case PN_CMD_SELECT_SUBCHANNEL_DATA:
                    CMSG(CPnpCommandSelectSubChannelDataMsg);
                    break;
                case PN_CMD_SELECT_SUBCHANNEL_PAD:
                    CMSG(CPnpCommandSelectSubChannelPadMsg);
                    break;
                case PN_CMD_SET_ASMODE:
                    CMSG(CPnpCommandSetAsModeMsg);
                    break;
                case PN_CMD_SET_FICDUMP:
                    CMSG(CPnpCommandSetFicDumpMsg);
                    break;
                case PN_CMD_LLHI:
                    CMSG(CPnpCommandLLHIMsg);
                    break;
                case PN_CMD_GETCHDBINFO:
                    CMSG(CPnpCommandGetChDbInfoMsg);
                    break;
                case PN_CMD_SET_MUTE_CONFIG:
                    CMSG(CPnpCommandSetMuteConfigMsg);
                    break;
                    // response
                case PN_NOTY_RESET_RESP:
                    CMSG(CPnpNotifyResetResponseMsg);
                    break;
                case PN_NOTY_ALIVE_RESP:
                    CMSG(CPnpNotifyAliveResponseMsg);
                    break;
                case PN_NOTY_SCANINFO_RESP:
                    CMSG(CPnpNotifyScanInfoResponseMsg);
                    break;
                case PN_NOTY_ENSINFO_RESP:
                    CMSG(CPnpNotifyEnsInfoResponseMsg);
                    break;
                case PN_NOTY_SRVINFO_RESP:
                    CMSG(CPnpNotifySvrInfoResponseMsg);
                    break;
                case PN_NOTY_SRVCOMP_RESP:
                    CMSG(CPnpNotifySvrCompResponseMsg);
                    break;
                case PN_NOTY_PLAYINFO_RESP:
                    CMSG(CPnpNotifyPlayInfoResponseMsg);
                    break;
                case PN_NOTY_DLS_RESP:
                    CMSG(CPnpNotifyDlsResponseMsg);
                    break;
                case PN_NOTY_DATA_RESP:
                    CMSG(CPnpNotifyDataResponseMsg);
                    break;
                case PN_NOTY_SIGNAL_QUALITY_RESP:
                    CMSG(CPnpNotifySignalQualityResponseMsg);
                    break;
                case PN_NOTY_TUNER_STATUS_RESP:
                    CMSG(CPnpNotifyTunerStatusResponseMsg);
                    break;
                case PN_NOTY_BER_MEASUREMENT_RESP:
                    CMSG(CPnpNotifyBerMeasurementsResponseMsg);
                    break;
                case PN_NOTY_DIAG_MSG_RESP:
                    CMSG(CPnpNotifyDiagResponseMsg);
                    break;
                case PN_NOTY_LLHI_RESP:
                    CMSG(CPnpNotifyLLHIResponseMsg);
                    break;
                case PN_NOTY_RDS_RADIOTEXT_RESP:
                    CMSG(CPnpNotifyRdsRadioTextResponseMsg);
                    break;
                case PN_NOTY_RDS_PSN_RESP:
                    CMSG(CPnpNotifyRdsPsnResponseMsg);
                    break;
                case PN_NOTY_RDS_PTY_RESP:
                    CMSG(CPnpNotifyRdsPtyResponseMsg);
                    break;
                case PN_CMD_SETCONFIG_RESP:
                    CMSG(CPnpCommandSetConfigResponseMsg);
                    break;
                case PN_CMD_POWER_RESP:
                    CMSG(CPnpCommandPowerResponseMsg);
                    break;
                case PN_CMD_ACTSTOP_RESP:
                    CMSG(CPnpCommandActStopResponseMsg);
                    break;
                case PN_CMD_TUNE_RESP:
                    CMSG(CPnpCommandTuneResponseMsg);
                    break;
                case PN_CMD_SRCH_RESP:
                    CMSG(CPnpCommandSrchResponseMsg);
                    break;
                case PN_CMD_SELECT_RESP:
                    CMSG(CPnpCommandSelectResponseMsg);
                    break;
                case PN_CMD_SETDATA_RESP:
                    CMSG(CPnpCommandSetDataResponseMsg);
                    break;
                case PN_CMD_SCAN_RESP:
                    CMSG(CPnpCommandScanResponseMsg);
                    break;
                case PN_CMD_GET_VERSION_RESP:
                    CMSG(CPnpCommandGetVersionResponseMsg);
                    break;
                case PN_CMD_TUNER_STATUS_RESP:
                    CMSG(CPnpCommandTunerStatusResponseMsg);
                    break;
                case PN_CMD_BER_MEASUREMENT_RESP:
                    CMSG(CPnpCommandBerMeasurementResponseMsg);
                    break;
                case PN_CMD_DRC_RESP:
                    CMSG(CPnpCommandDrcResponseMsg);
                    break;
                case PN_CMD_SELECT_AUDIO_SERVICE_RESP:
                    CMSG(CPnpCommandSelectAudioServiceResponseMsg);
                    break;
                case PN_CMD_SELECT_SUBCHANNEL_DATA_RESP:
                    CMSG(CPnpCommandSelectSubChannelDataResponseMsg);
                    break;
                case PN_CMD_SELECT_SUBCHANNEL_PAD_RESP:
                    CMSG(CPnpCommandSelectSubChannelPadResponseMsg);
                    break;
                case PN_CMD_SET_ASMODE_RESP:
                    CMSG(CPnpCommandSetAsModeResponseMsg);
                    break;
                case PN_CMD_SET_FICDUMP_RESP:
                    CMSG(CPnpCommandSetFicDumpResponseMsg);
                    break;
                case PN_CMD_LLHI_RESP:
                    CMSG(CPnpCommandLLHIResponseMsg);
                    break;
                case PN_CMD_GETCHDBINFO_RESP:
                    CMSG(CPnpCommandGetChDbInfoResponseMsg);
                    break;
                case PN_CMD_SET_MUTE_CONFIG_RESP:
                    CMSG(CPnpCommandSetMuteConfigResponseMsg);
                    break;
            }
            return msg;
        }
     */

}