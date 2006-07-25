// -*- C++ -*-

#ifndef _TESTDLREWRITER_H
#define _TESTDLREWRITER_H

#include <sstream>
#include <iostream>
#include <string>
#include <map>
#include <iterator>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestSuite.h"

namespace dlvhex {
namespace racer {


  /**
   * @brief TestCases for DLRewriter. 
   *
   * @test Tests the DL Rewriter facility.
   */
  class TestDLRewriter : public TestSuite
  {
    CPPUNIT_TEST_SUITE(TestDLRewriter);
    CPPUNIT_TEST(runDLRewrite);
    CPPUNIT_TEST(runDLNoRewrite);
    CPPUNIT_TEST_SUITE_END();

  public: 
    void runDLRewrite();    

    void runDLNoRewrite();    
  };

} // namespace racer
} // namespace dlvhex

#endif /* _TESTDLREWRITER_H */
