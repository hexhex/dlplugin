// -*- C++ -*-

#ifndef _TESTRACERINTERFACE_H
#define _TESTRACERINTERFACE_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestSuite.h"

namespace dlvhex {
namespace racer {


  /**
   * @brief TestCases for RacerDirector. 
   *
   * @test Imports the callback functions and runs some tests on them.
   */
  class TestRacerInterface : public TestSuite
  {
    CPPUNIT_TEST_SUITE(TestRacerInterface);
    CPPUNIT_TEST(runRacerConsistentTest);
    CPPUNIT_TEST(runRacerConceptTest);
    CPPUNIT_TEST(runRacerRoleTest);
    CPPUNIT_TEST(runRacerIsConceptTest);
    CPPUNIT_TEST(runRacerIsRoleTest);
    CPPUNIT_TEST(runRacerRoleFillersTest);
    CPPUNIT_TEST(runRacerDatatypeRoleFillersTest);
    CPPUNIT_TEST(runGetUniverseTest);
    CPPUNIT_TEST_SUITE_END();

  public: 
    virtual void setUp();

    void runRacerConsistentTest();

    void runRacerConceptTest();
    
    void runRacerRoleTest();

    void runRacerIsConceptTest();

    void runRacerIsRoleTest();

    void runRacerRoleFillersTest();

    void runRacerDatatypeRoleFillersTest();

    void runGetUniverseTest();    
  };

} // namespace racer
} // namespace dlvhex

#endif /* _TESTRACERBUILDER_H */
