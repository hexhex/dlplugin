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

#include "DLRewriter.h"

namespace dlvhex {
namespace racer {


  /**
   * @brief TestCases for DLRewriter. 
   *
   * @test
   */
  class TestDLRewriter : public CppUnit::TestFixture
  {
  private:
    CPPUNIT_TEST_SUITE(TestDLRewriter);
    CPPUNIT_TEST(runDLRewrite);
    CPPUNIT_TEST_SUITE_END();

  public: 
    virtual void setUp();

    void runDLRewrite();    
  };

} // namespace racer
} // namespace dlvhex

#endif /* _TESTDLREWRITER_H */
