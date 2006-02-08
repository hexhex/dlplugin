/**
 * @file   TestRacerQuery.cpp
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:31:18 2006
 * 
 * @brief  
 * 
 * 
 */

#include "TestRacerQuery.h"

using namespace dlvhex::racer;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestRacerQuery);


void
TestRacerQuery::runRacerQueryEqualityTest()
{
  Query q1;
  Query q2;

  AtomSet is1;
  is1.insert(Atom("p(a)"));
  is1.insert(Atom("p(b)"));
  is1.insert(Atom("q(c)"));
  Interpretation ints1(is1);
      
  AtomSet is2;
  is2.insert(Atom("p(b)"));
  is2.insert(Atom("p(a)"));
  is2.insert(Atom("q(c)"));
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

void
TestRacerQuery::runRacerQuerySubsetTest()
{
  Query q1;
  Query q2;
  
  AtomSet is1;
  is1.insert(Atom("p(b)"));
  is1.insert(Atom("q(c)"));
  Interpretation ints1(is1);
  
  AtomSet is2;
  is2.insert(Atom("p(a)"));
  is2.insert(Atom("p(b)"));
  is2.insert(Atom("q(c)"));
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

void
TestRacerQuery::runRacerQuerySupersetTest()
{
  Query q1;
  Query q2;
  
  AtomSet is1;
  is1.insert(Atom("p(b)"));
  is1.insert(Atom("p(a)"));
  is1.insert(Atom("q(c)"));
  Interpretation ints1(is1);
  
  AtomSet is2;
  is2.insert(Atom("p(b)"));
  is2.insert(Atom("q(c)"));
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
    
