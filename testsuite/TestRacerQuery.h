// -*- C++ -*-

/**
 * @file   TestRacerQuery.h
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:31:18 2006
 * 
 * @brief  TestCases for Query.
 * 
 * 
 */

#ifndef _TESTRACERQUERY_H
#define _TESTRACERQUERY_H

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
   * @brief TestCases for Query.
   *
   * @test Tests Query methods.
   *
   */
  class TestRacerQuery : public TestSuite
  {
    CPPUNIT_TEST_SUITE(TestRacerQuery);
    CPPUNIT_TEST(runRacerQueryEqualityTest);
    CPPUNIT_TEST(runRacerQuerySubsetTest);
    CPPUNIT_TEST(runRacerQuerySupersetTest);
    CPPUNIT_TEST(runRacerQueryLessThanTest);
    CPPUNIT_TEST_SUITE_END();

  public:
    void runRacerQueryEqualityTest();

    void runRacerQuerySubsetTest();

    void runRacerQuerySupersetTest();

    void runRacerQueryLessThanTest();
  };

} // namespace test
} // namespace dl
} // namespace dlvhex


#endif /* _TESTRACERQUERY_H */
