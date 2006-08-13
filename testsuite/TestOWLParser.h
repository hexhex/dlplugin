// -*- C++ -*-

/**
 * @file   TestOWLParser.h
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:02:29 2006
 * 
 * @brief  Tests parsing an OWL document.
 * 
 * 
 */

#ifndef _TESTOWLPARSER_H
#define _TESTOWLPARSER_H

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

} // namespace test
} // namespace dl
} // namespace dlvhex


#endif /* _TESTRACERDIRECTOR_H */
