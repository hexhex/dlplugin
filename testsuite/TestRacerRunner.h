// -*- C++ -*-

/**
 * @file   TestRacerRunner.h
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:35:11 2006
 * 
 * @brief  TestCase for RacerRunner.
 * 
 * 
 */

#ifndef _TESTRACERRUNNER_H
#define _TESTRACERRUNNER_H

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
   * @brief TestCase for RacerRunner.
   *
   * @test Tests RacerRunner.
   */
  class TestRacerRunner : public TestSuite
  {
    CPPUNIT_TEST_SUITE(TestRacerRunner);
    CPPUNIT_TEST(runRacerRunnerTest);
    CPPUNIT_TEST_SUITE_END();

  public:
    void runRacerRunnerTest();    
  };

} // namespace test
} // namespace dl
} // namespace dlvhex

#endif /* _TESTRACERRUNNER_H */
