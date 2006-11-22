// -*- C++ -*-

/**
 * @file   TestDLOptimizer.h
 * @author Thomas Krennwallner
 * @date   Wed Nov 22 11:44:00 2006
 * 
 * @brief  Tests the DL Optimizer facility.
 * 
 * 
 */

#ifndef _TESTDLOPTIMIZER_H
#define _TESTDLOPTIMIZER_H

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
  class TestDLOptimizer : public TestSuite
  {
    CPPUNIT_TEST_SUITE(TestDLOptimizer);
    CPPUNIT_TEST(runDLOptimize);
    CPPUNIT_TEST_SUITE_END();

  public: 
    void runDLOptimize();    
  };

} // namespace test
} // namespace dl
} // namespace dlvhex

#endif /* _TESTDLREWRITER_H */
