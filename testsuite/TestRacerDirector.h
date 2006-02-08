// -*- C++ -*-

#ifndef _TESTRACERDIRECTOR_H
#define _TESTRACERDIRECTOR_H

#include <iostream>
#include <string>
#include <functional>
#include <iterator>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "RacerDirector.h"
#include "RacerRunner.h"
#include "TCPStream.h"

namespace dlvhex {
namespace racer {

  /**
   * @brief TestCases for RacerDirector.
   *
   * @test Opens a connection to RACER with TCPIOStream, sends some
   * commands and performs some sanity checks on the result.
   */
  class TestRacerDirector : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(TestRacerDirector);
    CPPUNIT_TEST(runRacerPlusConceptTest);
    CPPUNIT_TEST(runRacerAllIndividualsTest);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void output(const std::vector<Tuple>& tv);

  public:
    virtual void setUp();

    void runRacerPlusConceptTest();
    
    void runRacerAllIndividualsTest();
  };

} // namespace racer
} // namespace dlvhex


#endif /* _TESTRACERDIRECTOR_H */
