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

  AtomPtr ap11(new Atom("p(a)"));
  AtomPtr ap12(new Atom("p(b)"));
  AtomPtr ap13(new Atom("q(c)"));
  AtomSet is1;
  is1.insert(ap11);
  is1.insert(ap12);
  is1.insert(ap13);
  Interpretation ints1(is1);

  AtomPtr ap21(new Atom("p(b)"));
  AtomPtr ap22(new Atom("p(a)"));
  AtomPtr ap23(new Atom("q(c)"));
  AtomSet is2;
  is2.insert(ap21);
  is2.insert(ap22);
  is2.insert(ap23);
  Interpretation ints2(is2);

  Tuple pt1;
  Tuple pt2;
  pt1.push_back(Term("foo",true));
  pt2.push_back(Term("foo",true));

  Term pa("a");

  q1.setQuery(pa);
  q1.setPatternTuple(pt1);
  q1.setInterpretation(ints1);
  q1.setPlusC("p");
  q1.setMinusC("q");
  q1.setPlusR("foo");
  q1.setMinusR("foo");

  q2.setQuery(pa);
  q2.setPatternTuple(pt2);
  q2.setInterpretation(ints2);
  q2.setPlusC("p");
  q2.setMinusC("q");
  q2.setPlusR("foo");
  q2.setMinusR("foo");

  CPPUNIT_ASSERT(   q1 == q2  );
  CPPUNIT_ASSERT( q1.isSubseteq(q2) );
  CPPUNIT_ASSERT( q1.isSuperseteq(q2) );
}

void
TestRacerQuery::runRacerQuerySubsetTest()
{
  Query q1;
  Query q2;
  
  AtomPtr ap11(new Atom("p(b)"));
  AtomPtr ap12(new Atom("q(c)"));
  AtomSet is1;
  is1.insert(ap11);
  is1.insert(ap12);
  Interpretation ints1(is1);
  
  AtomPtr ap21(new Atom("p(a)"));
  AtomPtr ap22(new Atom("p(b)"));
  AtomPtr ap23(new Atom("q(c)"));
  AtomSet is2;
  is2.insert(ap21);
  is2.insert(ap22);
  is2.insert(ap23);
  Interpretation ints2(is2);

  Tuple pt1;
  Tuple pt2;
  pt1.push_back(Term("foo",true));
  pt2.push_back(Term("foo",true));
  
  Term pa("a");
  
  q1.setQuery(pa);
  q1.setPatternTuple(pt1);
  q1.setInterpretation(ints1);
  q1.setPlusC("p");
  q1.setMinusC("q");
  q1.setPlusR("foo");
  q1.setMinusR("foo");
  
  q2.setQuery(pa);
  q2.setPatternTuple(pt2);
  q2.setInterpretation(ints2);
  q2.setPlusC("p");
  q2.setMinusC("q");
  q2.setPlusR("foo");
  q2.setMinusR("foo");
  
  CPPUNIT_ASSERT( q1 == q2 );
  CPPUNIT_ASSERT(   q1.isSubseteq(q2)  );
  CPPUNIT_ASSERT( !(q1.isSuperseteq(q2)) );
}

void
TestRacerQuery::runRacerQuerySupersetTest()
{
  Query q1;
  Query q2;
  
  AtomPtr ap11(new Atom("p(b)"));
  AtomPtr ap12(new Atom("p(a)"));
  AtomPtr ap13(new Atom("q(c)"));
  AtomSet is1;
  is1.insert(ap11);
  is1.insert(ap12);
  is1.insert(ap13);
  Interpretation ints1(is1);
  
  AtomPtr ap21(new Atom("p(b)"));
  AtomPtr ap22(new Atom("q(c)"));
  AtomSet is2;
  is2.insert(ap21);
  is2.insert(ap22);
  Interpretation ints2(is2);
  
  Tuple pt1;
  Tuple pt2;
  pt1.push_back(Term("foo",true));
  pt2.push_back(Term("foo",true));

  Term pa("a");
  
  q1.setQuery(pa);
  q1.setPatternTuple(pt1);
  q1.setInterpretation(ints1);
  q1.setPlusC("p");
  q1.setMinusC("q");
  q1.setPlusR("foo");
  q1.setMinusR("foo");
  
  q2.setQuery(pa);
  q2.setPatternTuple(pt2);
  q2.setInterpretation(ints2);
  q2.setPlusC("p");
  q2.setMinusC("q");
  q2.setPlusR("foo");
  q2.setMinusR("foo");
  
  CPPUNIT_ASSERT( q1 == q2 );
  CPPUNIT_ASSERT( !(q1.isSubseteq(q2)) );
  CPPUNIT_ASSERT(   q1.isSuperseteq(q2) );
}
    
void
TestRacerQuery::runRacerQueryLessThanTest()
{
  Query q1;
  Query q2;

  Tuple pt1;
  Tuple pt2;
  pt1.push_back(Term("foo",true));
  pt2.push_back(Term("foo",true));

  Term pa("a");

  q1.setQuery(pa);
  q1.setPatternTuple(pt1);
  q1.setPlusC("p");
  q1.setMinusC("q");
  q1.setPlusR("foo");
  q1.setMinusR("foo");

  Term pb("b");

  q2.setQuery(pb);
  q2.setPatternTuple(pt2);
  q2.setPlusC("p");
  q2.setMinusC("q");
  q2.setPlusR("foo");
  q2.setMinusR("foo");

  CPPUNIT_ASSERT( (q1 < q2) && !(q2 < q1) );

  q2.setQuery(pa);
  q2.setPatternTuple(pt1);
  q2.setPlusC("p");
  q2.setMinusC("q");
  q2.setPlusR("foo");
  q2.setMinusR("foo");

  CPPUNIT_ASSERT( !(q1 < q2) && !(q2 < q1) );

  pt1.clear();
  pt2.clear();

  pt1.push_back(Term("X"));
  pt1.push_back(Term("foo",true));

  pt2.push_back(Term("foo",true));
  pt2.push_back(Term("X"));

  q1.setPatternTuple(pt1);
  q2.setPatternTuple(pt2);

  CPPUNIT_ASSERT( (q1 < q2) && !(q2 < q1) );

  pt1.clear();
  pt2.clear();

  pt1.push_back(Term("X"));
  pt1.push_back(Term("X"));

  pt2.push_back(Term("X"));
  pt2.push_back(Term("Y"));

  q1.setPatternTuple(pt1);
  q2.setPatternTuple(pt2);

  CPPUNIT_ASSERT( !(q1 < q2) && !(q2 < q1) );

  pt1.clear();
  pt2.clear();

  pt1.push_back(Term("a"));
  pt1.push_back(Term("b"));

  pt2.push_back(Term("X"));
  pt2.push_back(Term("Y"));

  q1.setPatternTuple(pt1);
  q2.setPatternTuple(pt2);

  CPPUNIT_ASSERT( !(q1 < q2) && (q2 < q1) );

  pt1.clear();
  pt2.clear();

  pt1.push_back(Term("X"));
  pt1.push_back(Term("a"));

  pt2.push_back(Term("Z"));
  pt2.push_back(Term("a"));

  q1.setPatternTuple(pt1);
  q2.setPatternTuple(pt2);

  CPPUNIT_ASSERT( !(q1 < q2) && !(q2 < q1) );
}
