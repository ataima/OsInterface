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

#ifndef ISOCKETCLIENT_H
#define	ISOCKETCLIENT_H

#define  MAX_C_SOCK_OPTION  16

class ISocketClient {
public:

    typedef struct tag_socket_client_option {
        int level;
        int option_name;
        int value;
    } s_Option;

    typedef struct tag_socket_client_setting {
        int domain;
        int type;
        int protocol;
        int num_opt;
        s_Option options[MAX_C_SOCK_OPTION];
    } s_Setting;

    typedef enum tag_spi_error {
        socket_client_generic_error = -1,
        socket_client_already_open_error = -10000, //10000
        socket_client_no_address_family_error, //9999        
        socket_client_no_more_file_descriptor_error, //9998
        socket_client_no_protocol_error, //9997
        socket_client_no_supported_by_protocol_error, //9996
        socket_client_eaccess_error, //9996        
        socket_client_no_more_resource_error, //9995   
        socket_client_no_valid_descriptor, //9994        
        socket_client_no_valid_option, //9993    
        socket_client_no_valid_socket, //9992        
        socket_client_max_clients, //9991  
        socket_client_no_service_thread, //9990
        socket_client_refused_connection, //9989
        socket_client_address_already_in_use, //9988
        socket_client_socket_not_avaiable, //9987
        socket_client_address_not_supported, //9986
        socket_client_insufficient_caches, //9985    
        socket_client_already_try_to_connect, //9984   
        socket_client_connection_in_progress, //9983
        socket_client_interrupted_by_signal, //9982
        socket_client_network_unavaiable, //9981
        socket_client_no_feature_supported, //9980
        socket_client_timeout, //9979
        socket_client_createmutex_fail, //9978
        socket_client_invalid_domain //9977
    } s_Error;

public:
    virtual int Open(int addr, int nPort, const ISocketClient::s_Setting & _setting) = 0;

    virtual int Open(const char *ipcname, const ISocketClient::s_Setting & _setting) = 0;

    virtual int Close(void) = 0;

    virtual ~ISocketClient() {
    }
};


#endif	/* ISOCKETCLIENT_H */

