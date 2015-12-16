/*
 * File:   CLinuxServerSocketTest.cpp
 * Author: acoppi
 *
 * Created on 11-feb-2015, 14.13.23
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <map>
#include "../inc/IHardware.h"
#include "../inc/ISoftware.h"
#include "../inc/ILogger.h"
#include "../inc/IDevice.h"
#include "../inc/IHandler.h"
#include "../inc/IPort.h"
#include "../inc/CBasePort.h"
#include "../inc/CLinuxPort.h"
#include "../inc/ISocketServer.h"
#include "../inc/CBaseSocketServer.h"
#include "../inc/CMapToPointer.h"
#include "../inc/CGenericDevice.h"
#include "../inc/IBlock.h"
#include "../inc/CBaseBlock.h"
#include "../inc/CGenericHardware.h"
#include "../inc/CGenericSoftware.h"
#include "../inc/ISocketClient.h"
#include "../inc/CBaseSocketClient.h"
#include "../inc/CLinuxSocketClient.h"
#include "../inc/CBaseHandler.h"
#include "../inc/CLinuxSocketServer.h"

#define SERV_NET_ADDR 0x7f000001

#ifdef USE_LINUX

#include "CLinuxServerSocketTest.h"

#include <signal.h>  // SIGALRM
#include <errno.h>  // ENOMEM
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>

CPPUNIT_TEST_SUITE_REGISTRATION(CLinuxServerSocketTest);

CLinuxServerSocketTest::CLinuxServerSocketTest() {
}

CLinuxServerSocketTest::~CLinuxServerSocketTest() {
}

void CLinuxServerSocketTest::setUp() {
    device::CGenericDevice::Create("stderr");

}

void CLinuxServerSocketTest::tearDown() {
    device::CGenericDevice::Destroy();
}

void CLinuxServerSocketTest::testServerOpenCloseMethod() {
    ISocketServer::s_Setting setting;
    memset(&setting, 0, sizeof (ISocketServer::s_Setting));
    setting.domain = AF_INET;
    setting.type = SOCK_STREAM;
    setting.protocol = IPPROTO_TCP;
    setting.num_opt = 2;
    setting.options[0].level = SOL_SOCKET;
    setting.options[0].option_name = SO_REUSEADDR;
    setting.options[0].value = 1;
    setting.options[1].level = IPPROTO_TCP;
    setting.options[1].option_name = TCP_NODELAY;
    setting.options[1].value = 1;
    hardware::CSocketServer *s = new hardware::CSocketServer();
    CPPUNIT_ASSERT_MESSAGE("Create CLinuxSocketServer", s != NULL);
    int res = s->Open(INADDR_ANY, 1000, setting);
    CPPUNIT_ASSERT_MESSAGE("Test CLinuxSocketServer::Open", res == 0);
    res = s->Close();
    CPPUNIT_ASSERT_MESSAGE("Test CLinuxSocketServer::Close", res == 0);
    delete s;
}

void CLinuxServerSocketTest::testClientOpenCloseMethod() {
    ISocketClient::s_Setting setting;
    memset(&setting, 0, sizeof (ISocketClient::s_Setting));
    setting.domain = AF_INET;
    setting.type = SOCK_STREAM;
    setting.protocol = IPPROTO_TCP;
    setting.num_opt = 2;
    setting.options[0].level = SOL_SOCKET;
    setting.options[0].option_name = SO_REUSEADDR;
    setting.options[0].value = 1;
    setting.options[1].level = IPPROTO_TCP;
    setting.options[1].option_name = TCP_NODELAY;
    setting.options[1].value = 1;
    hardware::CSocketClient *s = new hardware::CSocketClient();
    CPPUNIT_ASSERT_MESSAGE("Create CLinuxSocketClient", s != NULL);
    int res = s->Open(SERV_NET_ADDR, 1000, setting);
    CPPUNIT_ASSERT_MESSAGE("Test CLinuxSocketClient::Open", res == (int) ISocketClient::socket_client_refused_connection);
    res = s->Close();
    CPPUNIT_ASSERT_MESSAGE("Test CLinuxSocketClient::Close", res == 0);
    delete s;
}

void CLinuxServerSocketTest::testClientServerConnection() {
    ISocketServer::s_Setting setting;
    memset(&setting, 0, sizeof (ISocketServer::s_Setting));
    setting.domain = AF_INET;
    setting.type = SOCK_STREAM;
    setting.protocol = IPPROTO_TCP;
    setting.num_opt = 2;
    setting.options[0].level = SOL_SOCKET;
    setting.options[0].option_name = SO_REUSEADDR;
    setting.options[0].value = 1;
    setting.options[1].level = IPPROTO_TCP;
    setting.options[1].option_name = TCP_NODELAY;
    setting.options[1].value = 1;
    hardware::CSocketServer *s = new hardware::CSocketServer();
    CPPUNIT_ASSERT_MESSAGE("Create CLinuxSocketServer", s != NULL);
    int res = s->Open(INADDR_ANY, 10000, setting);
    CPPUNIT_ASSERT_MESSAGE("Test CLinuxSocketServer::Open", res == 0);
    // try to open a client...
    ISocketClient::s_Setting c_setting;
    memset(&setting, 0, sizeof (ISocketClient::s_Setting));
    c_setting.domain = AF_INET;
    c_setting.type = SOCK_STREAM;
    c_setting.protocol = IPPROTO_TCP;
    c_setting.num_opt = 2;
    c_setting.options[0].level = SOL_SOCKET;
    c_setting.options[0].option_name = SO_REUSEADDR;
    c_setting.options[0].value = 1;
    c_setting.options[1].level = IPPROTO_TCP;
    c_setting.options[1].option_name = TCP_NODELAY;
    c_setting.options[1].value = 1;
    hardware::CSocketClient *c = new hardware::CSocketClient();
    CPPUNIT_ASSERT_MESSAGE("Create CLinuxSocketClient", c != NULL);
    res = c->Open(SERV_NET_ADDR, 30000, c_setting);
    CPPUNIT_ASSERT_MESSAGE("Test CLinuxSocketClient::Open", res == 0);
    res = c->Close();
    CPPUNIT_ASSERT_MESSAGE("Test CLinuxSocketClient::Close", res == 0);
    res = s->Close();
    CPPUNIT_ASSERT_MESSAGE("Test CLinuxSocketServer::Close", res == 0);
    delete c;
    delete s;
}

class CTestHandler
: public hardware::CHandlerSocketServerClient {
public:

    int hook() {
        char buff[64];
        const char *msg = "Hello From Server\n";
        bzero(buff, 64);
        write(fileDesc, msg, strlen(msg));
        read(fileDesc, buff, 10);
        printf("%s", buff);
        return 0;
    }


};

void CLinuxServerSocketTest::testClientServerConnectionHelloMsg() {
    unsigned int caddress;
    ISocketServer::s_Setting setting;
    memset(&setting, 0, sizeof (ISocketServer::s_Setting));
    setting.domain = AF_INET;
    setting.type = SOCK_STREAM;
    setting.protocol = IPPROTO_TCP;
    setting.num_opt = 2;
    setting.options[0].level = SOL_SOCKET;
    setting.options[0].option_name = SO_REUSEADDR;
    setting.options[0].value = 1;
    setting.options[1].level = IPPROTO_TCP;
    setting.options[1].option_name = TCP_NODELAY;
    setting.options[1].value = 1;
    hardware::CSocketServer *s = new hardware::CSocketServer();
    CPPUNIT_ASSERT_MESSAGE("Create CLinuxSocketServer", s != NULL);
    int res = s->Open(INADDR_ANY, 31000, setting);
    CPPUNIT_ASSERT_MESSAGE("Test CLinuxSocketServer::Open", res == 0);
    // try to open a client...
    ISocketClient::s_Setting c_setting;
    memset(&setting, 0, sizeof (ISocketClient::s_Setting));
    c_setting.domain = AF_INET;
    c_setting.type = SOCK_STREAM;
    c_setting.protocol = IPPROTO_TCP;
    c_setting.num_opt = 2;
    c_setting.options[0].level = SOL_SOCKET;
    c_setting.options[0].option_name = SO_REUSEADDR;
    c_setting.options[0].value = 1;
    c_setting.options[1].level = IPPROTO_TCP;
    c_setting.options[1].option_name = TCP_NODELAY;
    c_setting.options[1].value = 1;
    hardware::CSocketClient *c = new hardware::CSocketClient();
    CPPUNIT_ASSERT_MESSAGE("Create CLinuxSocketClient", c != NULL);
    res = c->Open(SERV_NET_ADDR, 31000, c_setting);
    CPPUNIT_ASSERT_MESSAGE("Test CLinuxSocketClient::Open", res == 0);
    CTestHandler th;
    s->WaitForClients(&th,caddress);
    unsigned int readed;
    unsigned char buff[128];
    bzero(buff, sizeof (buff));
    c->Read(buff, 15, readed, 100);
    printf("client rx %s\n", buff);
    c->Write((unsigned char *) "Hello from client", 17, readed, 0);
    res = c->Close();
    CPPUNIT_ASSERT_MESSAGE("Test CLinuxSocketClient::Close", res == 0);
    res = s->Close();
    CPPUNIT_ASSERT_MESSAGE("Test CLinuxSocketServer::Close", res == 0);
    delete c;
    delete s;
}
#endif