/*
 * File:   TestProtocolclass.cpp
 * Author: ubuntu
 *
 * Created on Feb 13, 2015, 12:34:48 PM
 */

#include "TestProtocolclass.h"
#include "../inc/IPort.h"
#include "../inc/ISoftware.h"
#include "../inc/ILogger.h"
#include "../inc/IHandler.h"
#include "../inc/IRawData.h"
#include "../inc/CRawData.h"
#include "../inc/IMessage.h"
#include "../inc/IProtocol.h"
#include "../inc/IMapToPointer.h"
#include "../inc/CBasePort.h"
#include "../inc/CLinuxPort.h"
#include "../inc/ICommPort.h"
#include "../inc/IHardware.h"
#include "../inc/IDevice.h"
#include "../inc/IMessageFactory.h"
#include "../inc/CCircularBuffer.h"
#include "../inc/CGenericMessage.h"
#include "../inc/CGenericHardware.h"
#include "../inc/IMessageManager.h"
#include "../inc/CGenericMessageManager.h"
#include "../inc/CMessageManager.h"
#include "../inc/CGenericDevice.h"
#include "../inc/IBlock.h"
#include "../inc/CBaseBlock.h"
#include "../inc/hardware_configure.h"

#include "../inc/CProtocol.h"
#include "../inc/CMapToPointer.h"
#include "../inc/ILogger.h"
#include "../inc/CDefaultLogger.h"
#include "../inc/CGenericTestPort.h"
#include "../inc/CBaseHandler.h"
#include "CDefaultDevice.h"



CPPUNIT_TEST_SUITE_REGISTRATION(TestProtocolclass);

TestProtocolclass::TestProtocolclass() {
}

TestProtocolclass::~TestProtocolclass() {
}

void TestProtocolclass::setUp() {
    device::CGenericDevice::Create("stderr");
    messages::CGenericMessageManager::Create();
}

void TestProtocolclass::tearDown() {
    device::CGenericDevice::Destroy();
    messages::CGenericMessageManager::Destroy();
}

/*
service_wr->set(0, device);
service_wr->set(1, software);
service_wr->set(2, hardware);
service_wr->set(3, logger);
service_wr->set(4, this);
service_wr->set(5, service_wr);
service_wr->set(6, wr_Port);
service_wr->set(7, service_rd);
service_wr->set(8, rd_Port);
service_wr->set(9, MM);
service_wr->set(10, factory);
 */

class CTestPort
: public test::CGenericTestPort {
public:

    int getFileDescriptor(void) {
        return 0;
    }

    int WriteAndRead(unsigned char *out, unsigned char *in, unsigned int size, unsigned int & readed, unsigned int timeout){
        return 0;
    }

    
    int Write(unsigned char *out, size_t size, size_t & writed, size_t timeout) {
        LOG(logger, "CTestPort::Write")
        writed = size;
        return 0;
    }

    int Read(unsigned char *in, size_t size, size_t & readed, size_t timeout) {
        LOG(logger, "CTestPort::Read")
        in[0] = 'A';
        in[1] = 'B';
        in[2] = 'C';
        in[3] = 'D';
        in[4] = 'E';
        readed = size;
        return 0;
    }

};

class CTestFactory
: public IMessageFactory {
public:

    IMessage *CreateMsg(const size_t message_type) {
        return new messages::CGenericMessage();
    }

    void DestroyMsg(IMessage **msg) {
        delete *msg;
        *msg = NULL;
    }

    int toRaw(unsigned char **out, size_t &size, IMessage *msg) {
        *out = new unsigned char[5];
        size = 5;
        return 0;

    }

    int fromRaw(size_t message_type, unsigned char *in, size_t size, IMessage **msg) {
        return 0;

    }

};

class CTestProtocol
: public protocol::CProtocol {
public:

    CTestProtocol() {
    }

    virtual void * handlerReceive(CProtocol * p) {
        unsigned char in[32];
        unsigned int readed = 0;
        LOG(logger, "CTEstProtocol::HandlerReceive Start\n");
        while (1) {
            if (rd_Port->Read(in, 1, readed, 0) == 0) {
                if (readed == 1)
                    LOG(logger, "CTEstProtocol::HandlerReceive RX=%02X\n", in[0]);
            }
            if (exit_from_rx != 0)
                break;
        }
        LOG(logger, "CTEstProtocol::HandlerReceive End\n");
    }

};

void TestProtocolclass::testRXMethod() {

    IProtocol* proto = new CTestProtocol();
    IPort *port = new CTestPort();
    IMessageFactory * factory = new CTestFactory();
    proto->setRxService(port);
    proto->setTxService(port);
    IMessageManager * mm = messages::CGenericMessageManager::getMessageManager();
    mm->setFactory(factory);
    CPPUNIT_ASSERT_MESSAGE("Protocol Open", proto->Open() == 0);
    device::CDefaultDevice::getDevice()->GetAvaiableSoftware()->SleepThread(1);
    CPPUNIT_ASSERT_MESSAGE("Protocol Close", proto->Close() == 0);
    delete factory;
    delete port;
    delete proto;
}

void TestProtocolclass::testRX1Method() {

    IProtocol* proto = new CTestProtocol();
    IPort *port = new CTestPort();
    IMessageFactory * factory = new CTestFactory();
    proto->setRxService(port);
    proto->setTxService(port);
    IMessageManager * mm = messages::CGenericMessageManager::getMessageManager();
    mm->setFactory(factory);
    CPPUNIT_ASSERT_MESSAGE("Protocol Open", proto->Open() == 0);
    proto->dispatchMessage(factory->CreateMsg(0));
    device::CDefaultDevice::getDevice()->GetAvaiableSoftware()->SleepThread(1);
    CPPUNIT_ASSERT_MESSAGE("Protocol Close", proto->Close() == 0);
    delete factory;
    delete port;
    delete proto;
}

void TestProtocolclass::testTXMethod() {

}


