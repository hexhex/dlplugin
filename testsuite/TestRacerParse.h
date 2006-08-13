// -*- C++ -*-

/**
 * @file   TestRacerParse.h
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:27:06 2006
 * 
 * @brief  TestCases for RacerAnswer Parser.
 * 
 * 
 */

#ifndef _TESTRACERPARSE_H
#define _TESTRACERPARSE_H

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
   * @brief TestCases for RacerAnswer Parser.
   *
   * @test Tests the RacerSimpleAnswer and RacerAnswerList parsers and
   * performs some sanity checks on the result.
   */
  class TestRacerParse : public TestSuite
  {
    CPPUNIT_TEST_SUITE(TestRacerParse);
    CPPUNIT_TEST(runRacerSimpleAnswerTest);
    CPPUNIT_TEST(runRacerAnswerListTest);
    CPPUNIT_TEST_SUITE_END();

  public:
    void runRacerSimpleAnswerTest();

    void runRacerAnswerListTest();    
  };

} // namespace test
} // namespace dl
} // namespace dlvhex


#endif /* _TESTRACERPARSE_H */
