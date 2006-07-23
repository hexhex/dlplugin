// -*- C++ -*-

#ifndef _TESTRACERTYPES_H
#define _TESTRACERTYPES_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestSuite.h"

namespace dlvhex {
namespace racer {

  /**
   * @brief TestCase for some types.
   *
   * @test Tests types like Atom and AtomSet
   */
  class TestRacerTypes : public TestSuite
  {
    CPPUNIT_TEST_SUITE(TestRacerTypes);
    CPPUNIT_TEST(runRacerSetTest);
    CPPUNIT_TEST_SUITE_END();

  public:
    void runRacerSetTest();   
  };

} // namespace racer
} // namespace dlvhex

#endif /* _TESTRACERRUNNER_H */
