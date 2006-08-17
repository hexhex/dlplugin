/**
 * @file   TestRacerRunner.cpp
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:35:11 2006
 * 
 * @brief  TestCase for RacerRunner.
 * 
 * 
 */

#include "RacerRunner.h"

#include "TestRacerRunner.h"

using namespace dlvhex::dl;
using namespace dlvhex::dl::racer;
using namespace dlvhex::dl::test;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestRacerRunner);


void
TestRacerRunner::runRacerRunnerTest()
{
  CPPUNIT_ASSERT(RacerRunner::instance() != 0);
  //RacerRunner::instance()->run();
}

