// -*- C++ -*-

#ifndef _TESTOWLPARSER_H
#define _TESTOWLPARSER_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestSuite.h"


namespace dlvhex {
namespace racer {

  /**
   * @brief TestCases for OWLParser
   *
   * @test Tests parsing an OWL document.
   */
  class TestOWLParser : public TestSuite
  {
    CPPUNIT_TEST_SUITE(TestOWLParser);
    CPPUNIT_TEST(runParserTest);
    CPPUNIT_TEST_SUITE_END();

  public:
    void runParserTest();
  };

} // namespace racer
} // namespace dlvhex


#endif /* _TESTRACERDIRECTOR_H */
