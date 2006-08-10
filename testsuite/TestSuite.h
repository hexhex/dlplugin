// -*- C++ -*-

#ifndef _TESTSUITE_H
#define _TESTSUITE_H

#include <dlvhex/Term.h>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace dlvhex {
namespace racer {

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

} // namespace racer
} // namespace dlvhex


#endif /* _TESTSUITE_H */
