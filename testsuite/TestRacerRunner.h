// -*- C++ -*-

#ifndef _TESTRACERRUNNER_H
#define _TESTRACERRUNNER_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestSuite.h"

namespace dlvhex {
namespace racer {

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

} // namespace racer
} // namespace dlvhex

#endif /* _TESTRACERRUNNER_H */
