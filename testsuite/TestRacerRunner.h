// -*- C++ -*-

#ifndef _TESTRACERRUNNER_H
#define _TESTRACERRUNNER_H

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "RacerRunner.h"

namespace dlvhex {
namespace racer {

  /**
   * @brief TestCase for RacerRunner.
   *
   * @test Tests RacerRunner.
   */
  class TestRacerRunner : public CppUnit::TestCase
  {
  public: 
    void runRacerRunnerTest()
    {
      CPPUNIT_ASSERT(RacerRunner::instance() != 0);
      RacerRunner::instance()->run();
    }

    static CppUnit::Test *suite()
    {
      CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestRacerRunner");
      
      
      suiteOfTests->addTest(new CppUnit::TestCaller<TestRacerRunner>
			    ( 
			     "RacerRunner", 
			     &TestRacerRunner::runRacerRunnerTest
			     )
			    );
      
      return suiteOfTests;
    }
    
  };

} // namespace racer
} // namespace dlvhex

#endif /* _TESTRACERRUNNER_H */
