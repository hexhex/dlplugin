// -*- C++ -*-

/**
 * @file   TestRacerBuilder.h
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:06:24 2006
 * 
 * @brief  Builds various RACER commands and checks if they are valid.
 * 
 * 
 */

#ifndef _TESTRACERBUILDER_H
#define _TESTRACERBUILDER_H

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
   * @brief TestCases for RacerBuilder and its children.
   *
   * @test Builds various RACER commands and checks if they are valid.
   */
  class TestRacerBuilder : public TestSuite
  { 
    CPPUNIT_TEST_SUITE(TestRacerBuilder);
    CPPUNIT_TEST(runRacerPosIndBuilderTest);
    CPPUNIT_TEST(runRacerNegIndBuilderTest);
    CPPUNIT_TEST(runRacerPosPairBuilderTest);
    CPPUNIT_TEST_SUITE_END();

  public: 
    void runRacerPosIndBuilderTest();
    
    void runRacerNegIndBuilderTest();
    
    void runRacerPosPairBuilderTest();
  };

} // namespace test
} // namespace dl
} // namespace dlvhex

#endif /* _TESTRACERBUILDER_H */
