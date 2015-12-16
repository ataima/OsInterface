/*
 * File:   CLinuxServerSocketTest.h
 * Author: acoppi
 *
 * Created on 11-feb-2015, 14.13.23
 */


#ifndef CLINUXSERVERSOCKETTEST_H
#define	CLINUXSERVERSOCKETTEST_H

#include <cppunit/extensions/HelperMacros.h>

class CLinuxServerSocketTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(CLinuxServerSocketTest);

    CPPUNIT_TEST(testServerOpenCloseMethod);
    CPPUNIT_TEST(testClientOpenCloseMethod);
    CPPUNIT_TEST(testClientServerConnection);
    CPPUNIT_TEST(testClientServerConnectionHelloMsg);

    CPPUNIT_TEST_SUITE_END();

public:
    CLinuxServerSocketTest();
    virtual ~CLinuxServerSocketTest();
    void setUp();
    void tearDown();

private:
    void testServerOpenCloseMethod();
    void testClientOpenCloseMethod();
    void testClientServerConnection();
    void testClientServerConnectionHelloMsg();
};

#endif	/* CLINUXSERVERSOCKETTEST_H */

