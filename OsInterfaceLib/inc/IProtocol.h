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

#ifndef _IPROTOCOL_H
#define	_IPROTOCOL_H

class IProtocol {
public:

    typedef enum tag_protocol_error {
        protocol_generic_error = -1,
        protocol_device_error = -2,
        protocol_hardware_error = -3,
        protocol_software_error = -4,
        protocol_logger_error = -5,
        protocol_create_rx_process_error = -30000,
        protocol_create_tx_process_error, //29999
        protocol_create_mutex_rx_error, //29998
        protocol_create_mutex_tx_error, //29997
        protocol_create_semph_tx_error, //29996
        protocol_create_semph_rx_error, //29995
        protocol_destroy_rx_process_error, //29994
        protocol_destroy_tx_process_error, //29993
        protocol_create_factory, //29992
        protocol_invalid_messagemanager, //29991
        protocol_invalid_tx_rx_managment //29990
    } protocolError;

public:
    virtual int dispatchMessage(IMessage *msg) = 0;
    virtual int listenerMessage(IMessage *msg) = 0;
    virtual int Open(int enableRX = 0, int enableTX = 0) = 0;
    virtual int Close(void) = 0;
    virtual void setRxService(int rd) = 0;
    virtual void setTxService(int wr) = 0;
    virtual void isRxEmpthy(int *res) = 0;
    virtual IMessage * GetLastRxMessage(void) = 0;
    virtual void setRxSemaphore(int mph) = 0;
};



#endif	/* _IPROTOCOL_H */

