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

#ifndef ISOCKETSERVER_H
#define	ISOCKETSERVER_H



#define MAX_S_SOCK_OPTION  16

class ISocketServer {
public:

    typedef struct tag_socket_server_option {
        int level;
        int option_name;
        int value;
    } s_Option;

    typedef struct tag_socket_server_setting {
        int domain;
        int type;
        int protocol;
        int num_opt;
        s_Option options[MAX_S_SOCK_OPTION];
    } s_Setting;

    typedef enum tag_server_mode {
        mode_client_shared_ip = 0,
        mode_client_exlusive_ip = 8
    } serverMode;

    typedef enum tag_spi_error {
        socket_server_generic_error = -1,
        socket_server_already_open_error = -10000, //10000
        socket_server_no_address_family_error, //9999        
        socket_server_no_more_file_descriptor_error, //9998
        socket_server_no_protocol_error, //9997
        socket_server_no_supported_by_protocol_error, //9996
        socket_server_eaccess_error, //9996        
        socket_server_no_more_resource_error, //9995   
        socket_server_no_valid_descriptor, //9994        
        socket_server_no_valid_option, //9993    
        socket_server_no_valid_socket, //9992        
        socket_server_max_clients, //9991  
        server_socket_no_service_thread, //9990
        server_socket_createmutex_fail, //9989
        server_socket_lockmutex_fail, //9988
        server_socket_destroy_service_thread, //9987
        server_socket_close_socket_error, //9986
        server_socket_shuntdown_socket_error, //9985
        server_socket_destroymutex_fail, //9984
        server_socket_invalid_domain //9983

    } s_Error;

public:

    virtual int Open(int adr, int port, const ISocketServer::s_Setting & setting, int maxclients = 5) = 0;

    virtual int Open(const char*ipcname, const ISocketServer::s_Setting & setting, int maxclients = 5) = 0;

    virtual int WaitForClients(IHandler *p, unsigned int & client_address) = 0;

    virtual int Close(void) = 0;

    virtual int AvaiableClients(void) = 0;

    virtual int RemoveClient(int client) = 0;

    virtual void SetMode(serverMode v) = 0;

    virtual serverMode GetMode(void) = 0;

    virtual ~ISocketServer() {
    }
};

#endif	/* ISOCKETSERVER_H */

