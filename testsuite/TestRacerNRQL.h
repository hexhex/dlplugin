// -*- C++ -*-

/**
 * @file   TestRacerNRQL.h
 * @author Thomas Krennwallner
 * @date   Thu Apr 13 14:38:37 2006
 * 
 * @brief  TestCases for Racer's nRQL.
 * 
 * 
 */

#ifndef _TESTRACERNRQL_H
#define _TESTRACERNRQL_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestSuite.h"

namespace dlvhex {
namespace dl {
namespace test {


  /**
   * @brief TestCases for Racer's nRQL.
   *
   * @test Builds various RACER commands and checks if they are valid.
   */
  class TestRacerNRQL : public TestSuite
  { 
    CPPUNIT_TEST_SUITE(TestRacerNRQL);
    CPPUNIT_TEST(runRacerRetrieveTest);
    CPPUNIT_TEST(runRacerTBoxRetrieveTest);
    CPPUNIT_TEST(runRacerPremiseRetrieveTest);
    CPPUNIT_TEST_SUITE_END();

  public: 
    void runRacerRetrieveTest();
    
    void runRacerTBoxRetrieveTest();

    void runRacerPremiseRetrieveTest();
  };

} // namespace test
} // namespace dl
} // namespace dlvhex

#endif /* _TESTRACERNRQL_H */
