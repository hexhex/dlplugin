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
 * @file   TestSuite.h
 * @author Thomas Krennwallner
 * @date   Sun Aug 13 15:27:48 2006
 * 
 * @brief  TestSuite base class.
 * 
 * 
 */

#ifndef _TESTSUITE_H
#define _TESTSUITE_H

#include <dlvhex/Term.h>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace dlvhex {
namespace dl {
namespace test {

  /**
   * @brief Base class for the TestCases.
   */
  class TestSuite : public CppUnit::TestFixture
  {
  protected:
    std::string examples;
    std::string shop;
    std::string shopuri;
    std::string test;
    std::string testuri;

  public:
    TestSuite();

    void
    output(const std::vector<Tuple>& tv);
  };

} // namespace test
} // namespace dl
} // namespace dlvhex


#endif /* _TESTSUITE_H */


// Local Variables:
// mode: C++
// End:
