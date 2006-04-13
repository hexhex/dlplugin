// -*- C++ -*-

#ifndef _TESTRACERNRQL_H
#define _TESTRACERNRQL_H

#include <sstream>
#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "RacerNRQL.h"


namespace dlvhex {
namespace racer {


  /**
   * @brief TestCases for RacerNRQL.
   *
   * @test Builds various RACER commands and checks if they are valid.
   */
  class TestRacerNRQL : public CppUnit::TestFixture
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

} // namespace racer
} // namespace dlvhex

#endif /* _TESTRACERNRQL_H */
