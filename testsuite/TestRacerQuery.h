// -*- C++ -*-

#ifndef _TESTRACERQUERY_H
#define _TESTRACERQUERY_H

#include <iostream>
#include <string>
#include <functional>
#include <iterator>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "RacerQuery.h"

namespace dlvhex {
namespace racer {

  /**
   * @brief TestCases for Query.
   *
   * @test Tests Query methods.
   *
   */
  class TestRacerQuery : public CppUnit::TestCase
  {
  public:
    void runRacerQueryEqualityTest()
    {
      Query q1;
      Query q2;

      GAtomSet is1;
      is1.insert(GAtom("p(a)"));
      is1.insert(GAtom("p(b)"));
      is1.insert(GAtom("q(c)"));
      Interpretation ints1(is1);
      
      GAtomSet is2;
      is2.insert(GAtom("p(b)"));
      is2.insert(GAtom("p(a)"));
      is2.insert(GAtom("q(c)"));
      Interpretation ints2(is2);

      Term pa("p(a)");

      q1.setQuery(pa);
      q1.setInterpretation(ints1);
      q1.setPlusC("p");
      q1.setMinusC("q");
      q1.setPlusR("foo");
      q1.setMinusR("foo");

      q2.setQuery(pa);
      q2.setInterpretation(ints2);
      q2.setPlusC("p");
      q2.setMinusC("p");
      q2.setPlusR("foo");
      q2.setMinusR("foo");

      CPPUNIT_ASSERT(   q1 == q2  );
      CPPUNIT_ASSERT( !(q1.isSubset(q2)) );
      CPPUNIT_ASSERT( !(q1.isSuperset(q2)) );
    }

    void runRacerQuerySubsetTest()
    {
      Query q1;
      Query q2;

      GAtomSet is1;
      is1.insert(GAtom("p(b)"));
      is1.insert(GAtom("q(c)"));
      Interpretation ints1(is1);
      
      GAtomSet is2;
      is2.insert(GAtom("p(a)"));
      is2.insert(GAtom("p(b)"));
      is2.insert(GAtom("q(c)"));
      Interpretation ints2(is2);

      Term pa("p(a)");

      q1.setQuery(pa);
      q1.setInterpretation(ints1);
      q1.setPlusC("p");
      q1.setMinusC("q");
      q1.setPlusR("foo");
      q1.setMinusR("foo");

      q2.setQuery(pa);
      q2.setInterpretation(ints2);
      q2.setPlusC("p");
      q2.setMinusC("q");
      q2.setPlusR("foo");
      q2.setMinusR("foo");

      CPPUNIT_ASSERT( !(q1 == q2) );
      CPPUNIT_ASSERT(   q1.isSubset(q2)  );
      CPPUNIT_ASSERT( !(q1.isSuperset(q2)) );
    }

    void runRacerQuerySupersetTest()
    {
      Query q1;
      Query q2;

      GAtomSet is1;
      is1.insert(GAtom("p(b)"));
      is1.insert(GAtom("p(a)"));
      is1.insert(GAtom("q(c)"));
      Interpretation ints1(is1);
      
      GAtomSet is2;
      is2.insert(GAtom("p(b)"));
      is2.insert(GAtom("q(c)"));
      Interpretation ints2(is2);
      
      Term pa("p(a)");

      q1.setQuery(pa);
      q1.setInterpretation(ints1);
      q1.setPlusC("p");
      q1.setMinusC("q");
      q1.setPlusR("foo");
      q1.setMinusR("foo");

      q2.setQuery(pa);
      q2.setInterpretation(ints2);
      q2.setPlusC("p");
      q2.setMinusC("q");
      q2.setPlusR("foo");
      q2.setMinusR("foo");

      CPPUNIT_ASSERT( !(q1 == q2) );
      CPPUNIT_ASSERT( !(q1.isSubset(q2)) );
      CPPUNIT_ASSERT(   q1.isSuperset(q2) );
    }
    
    static CppUnit::Test *suite()
    {
      CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestRacerQuery");
      
      suiteOfTests->addTest(new CppUnit::TestCaller<TestRacerQuery>
			    ( 
			     "RacerQueryEquality", 
			     &TestRacerQuery::runRacerQueryEqualityTest
			     )
			    );

      suiteOfTests->addTest(new CppUnit::TestCaller<TestRacerQuery>
			    ( 
			     "RacerQuerySubset", 
			     &TestRacerQuery::runRacerQuerySubsetTest
			     )
			    );

      suiteOfTests->addTest(new CppUnit::TestCaller<TestRacerQuery>
			    ( 
			     "RacerQuerySuperset", 
			     &TestRacerQuery::runRacerQuerySupersetTest
			     )
			    );
      
      return suiteOfTests;
    }
    
  };

} // namespace racer
} // namespace dlvhex


#endif /* _TESTRACERQUERY_H */
