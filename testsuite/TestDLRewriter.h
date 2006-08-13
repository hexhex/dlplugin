// -*- C++ -*-

/**
 * @file   TestDLRewriter.h
 * @author Thomas Krennwallner
 * @date   Thu Apr 27 13:28:39 2006
 * 
 * @brief  Tests the DL Rewriter facility.
 * 
 * 
 */

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
namespace dl {
namespace test {


  /**
   * @brief TestCases for HexDLRewriter. 
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

} // namespace test
} // namespace dl
} // namespace dlvhex

#endif /* _TESTDLREWRITER_H */
