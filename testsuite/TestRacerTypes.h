// -*- C++ -*-

#ifndef _TESTRACERTYPES_H
#define _TESTRACERTYPES_H

#include <iostream>
#include <string>
#include <iterator>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <dlvhex/Atom.h>

namespace dlvhex {
namespace racer {

  /**
   * @brief TestCase for some types.
   *
   * @test Tests types like GAtom and GAtomSet
   */
  class TestRacerTypes : public CppUnit::TestCase
  {
  public: 
    void runRacerSetTest()
    {
      GAtomSet is;

      is.insert(GAtom("p(b)"));
      is.insert(GAtom("p(b)"));
      is.insert(GAtom("q(a)"));
      is.insert(GAtom("q(a,c)"));
      is.insert(GAtom("q(a,b)"));
      is.insert(GAtom("p(a)"));

      GAtomSet::const_iterator it = is.begin();
      CPPUNIT_ASSERT(*it++ == std::string("p(a)"));
      CPPUNIT_ASSERT(*it++ == std::string("p(b)"));
      CPPUNIT_ASSERT(*it++ == std::string("q(a)"));
      CPPUNIT_ASSERT(*it++ == std::string("q(a,b)"));
      CPPUNIT_ASSERT(*it++ == std::string("q(a,c)"));
    }

    static CppUnit::Test *suite()
    {
      CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestRacerTypes");
      
      
      suiteOfTests->addTest(new CppUnit::TestCaller<TestRacerTypes>
			    ( 
			     "RacerTypes", 
			     &TestRacerTypes::runRacerSetTest
			     )
			    );
      
      return suiteOfTests;
    }
    
  };

} // namespace racer
} // namespace dlvhex

#endif /* _TESTRACERRUNNER_H */
