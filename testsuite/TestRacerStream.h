// -*- C++ -*-

#ifndef _TESTRACERSTREAM_H
#define _TESTRACERSTREAM_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestSuite.h"

namespace dlvhex {
namespace racer {

  /**
   * @brief TestCases for TCPIOStream and TCPStreamBuf
   *
   * @test Sends the (all-individual) command to RACER with help of
   * TCPIOStream and TCPStreamBuf and checks whether we receive an
   * answer.
   */
  class TestRacerStream : public TestSuite
  { 
    CPPUNIT_TEST_SUITE(TestRacerStream);
    CPPUNIT_TEST(runRacerStreamBufTest);
    CPPUNIT_TEST(runRacerIOStreamTest);
    CPPUNIT_TEST_SUITE_END();

  public:
    virtual void setUp();

    void runRacerStreamBufTest();   

    void runRacerIOStreamTest();   
  };

} // namespace racer
} // namespace dlvhex

#endif /* _TESTRACERSTREAM_H */
