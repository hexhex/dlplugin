/* dlvhex-dlplugin -- Integration of Answer-Set Programming and Description Logics.
 *
 * Copyright (C) 2005, 2006, 2007  Thomas Krennwallner
 * 
 * This file is part of dlvhex-dlplugin.
 *
 * dlvhex-dlplugin is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * dlvhex-dlplugin is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dlvhex-dlplugin; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


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


// Local Variables:
// mode: C++
// End:
