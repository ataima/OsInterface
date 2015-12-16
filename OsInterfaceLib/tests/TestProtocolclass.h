/*
 * File:   TestProtocolclass.h
 * Author: ubuntu
 *
 * Created on Feb 13, 2015, 12:34:48 PM
 */

#ifndef TESTPROTOCOLCLASS_H
#define	TESTPROTOCOLCLASS_H

#include <cppunit/extensions/HelperMacros.h>

class TestProtocolclass : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(TestProtocolclass);

    CPPUNIT_TEST(testRXMethod);
    CPPUNIT_TEST(testRX1Method);
    CPPUNIT_TEST(testTXMethod);

    CPPUNIT_TEST_SUITE_END();

public:
    TestProtocolclass();
    virtual ~TestProtocolclass();
    void setUp();
    void tearDown();

private:
    void testRXMethod();
    void testRX1Method();
    void testTXMethod();
};

#endif	/* TESTPROTOCOLCLASS_H */

