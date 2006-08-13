// -*- C++ -*-

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
