// -*- C++ -*-

#ifndef _TESTRACERPARSE_H
#define _TESTRACERPARSE_H

#include <sstream>
#include <iostream>
#include <string>
#include <exception>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "RacerParse.h"

namespace dlvhex {
namespace racer {

  /**
   * @brief TestCases for RacerParse and its children.
   *
   * @test Tests the RacerSimpleAnswer and RacerAnswerList parsers and
   * performs some sanity checks on the result.
   */
  class TestRacerParse : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(TestRacerParse);
    CPPUNIT_TEST(runRacerSimpleAnswerTest);
    CPPUNIT_TEST(runRacerAnswerListTest);
    CPPUNIT_TEST_SUITE_END();

  public: 
    void runRacerSimpleAnswerTest();

    void runRacerAnswerListTest();    
  };

} // namespace racer
} // namespace dlvhex


#endif /* _TESTRACERPARSE_H */
