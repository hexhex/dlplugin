// -*- C++ -*-

#ifndef _TESTRACERQUERY_H
#define _TESTRACERQUERY_H

#include <iostream>
#include <string>
#include <functional>
#include <iterator>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "RacerQuery.h"

namespace dlvhex {
namespace racer {

  /**
   * @brief TestCases for Query.
   *
   * @test Tests Query methods.
   *
   */
  class TestRacerQuery : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(TestRacerQuery);
    CPPUNIT_TEST(runRacerQueryEqualityTest);
    CPPUNIT_TEST(runRacerQuerySubsetTest);
    CPPUNIT_TEST(runRacerQuerySupersetTest);
    CPPUNIT_TEST_SUITE_END();

  public:
    void runRacerQueryEqualityTest();

    void runRacerQuerySubsetTest();

    void runRacerQuerySupersetTest();
  };

} // namespace racer
} // namespace dlvhex


#endif /* _TESTRACERQUERY_H */
