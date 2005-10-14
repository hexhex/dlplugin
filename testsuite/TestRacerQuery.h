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

using namespace std;


namespace dlvex_racer {

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
      
      GAtomSet is2;
      is2.insert(GAtom("p(b)"));
      is2.insert(GAtom("p(a)"));
      is2.insert(GAtom("q(c)"));

      GAtom pa("p(a)");

      q1.setQuery(pa);
      q2.setQuery(pa);
      q1.setPlusConcept(is1);
      q2.setPlusConcept(is2);

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
      
      GAtomSet is2;
      is2.insert(GAtom("p(a)"));
      is2.insert(GAtom("p(b)"));
      is2.insert(GAtom("q(c)"));

      GAtom pa("p(a)");

      q1.setQuery(pa);
      q2.setQuery(pa);
      q1.setMinusConcept(is1);
      q2.setMinusConcept(is2);

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
      
      GAtomSet is2;
      is2.insert(GAtom("p(b)"));
      is2.insert(GAtom("q(c)"));
      
      GAtom pa("p(a)");

      q1.setQuery(pa);
      q2.setQuery(pa);
      q1.setPlusRole(is1);
      q2.setPlusRole(is2);

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


} /* namespace dlvex_racer */


#endif /* _TESTRACERQUERY_H */
