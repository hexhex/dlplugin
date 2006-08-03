/**
 * @file   TestRacerQuery.cpp
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:31:18 2006
 * 
 * @brief  
 * 
 * 
 */

#include "Query.h"

#include "TestRacerQuery.h"

using namespace dlvhex::racer;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestRacerQuery);


void
TestRacerQuery::runRacerQueryEqualityTest()
{
  AtomPtr ap11(new Atom("p(a,x)"));
  AtomPtr ap12(new Atom("p(b,y)"));
  AtomPtr ap13(new Atom("q(c,z)"));
  AtomSet is1;
  is1.insert(ap11);
  is1.insert(ap12);
  is1.insert(ap13);
  Interpretation ints1(is1);

  AtomPtr ap21(new Atom("p(b,y)"));
  AtomPtr ap22(new Atom("p(a,x)"));
  AtomPtr ap23(new Atom("q(c,z)"));
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

  Query q1("","",Term("p"),Term("q"),Term("foo"),Term("foo"),DLQuery(pa,pt1),ints1);
  Query q2("","",Term("p"),Term("q"),Term("foo"),Term("foo"),DLQuery(pa,pt2),ints2);

  CPPUNIT_ASSERT(   q1 == q2  );
  CPPUNIT_ASSERT( q1.isSubseteq(q2) );
  CPPUNIT_ASSERT( q1.isSuperseteq(q2) );
}

void
TestRacerQuery::runRacerQuerySubsetTest()
{
  AtomPtr ap11(new Atom("p(b,y)"));
  AtomPtr ap12(new Atom("q(c,z)"));
  AtomSet is1;
  is1.insert(ap11);
  is1.insert(ap12);
  Interpretation ints1(is1);
  
  AtomPtr ap21(new Atom("p(a,x)"));
  AtomPtr ap22(new Atom("p(b,y)"));
  AtomPtr ap23(new Atom("q(c,z)"));
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

  Query q1("","",Term("p"),Term("q"),Term("foo"),Term("foo"),DLQuery(pa,pt1),ints1);
  Query q2("","",Term("p"),Term("q"),Term("foo"),Term("foo"),DLQuery(pa,pt2),ints2);
  
  CPPUNIT_ASSERT( q1 == q2 );
  CPPUNIT_ASSERT(   q1.isSubseteq(q2)  );
  CPPUNIT_ASSERT( !(q1.isSuperseteq(q2)) );
}

void
TestRacerQuery::runRacerQuerySupersetTest()
{
  AtomPtr ap11(new Atom("p(b,y)"));
  AtomPtr ap12(new Atom("p(a,x)"));
  AtomPtr ap13(new Atom("q(c,z)"));
  AtomSet is1;
  is1.insert(ap11);
  is1.insert(ap12);
  is1.insert(ap13);
  Interpretation ints1(is1);
  
  AtomPtr ap21(new Atom("p(b,y)"));
  AtomPtr ap22(new Atom("q(c,z)"));
  AtomSet is2;
  is2.insert(ap21);
  is2.insert(ap22);
  Interpretation ints2(is2);
  
  Tuple pt1;
  Tuple pt2;
  pt1.push_back(Term("foo",true));
  pt2.push_back(Term("foo",true));

  Term pa("a");
  
  Query q1("","",Term("p"),Term("q"),Term("foo"),Term("foo"),DLQuery(pa,pt1),ints1);
  Query q2("","",Term("p"),Term("q"),Term("foo"),Term("foo"),DLQuery(pa,pt2),ints2);
  
  CPPUNIT_ASSERT( q1 == q2 );
  CPPUNIT_ASSERT( !(q1.isSubseteq(q2)) );
  CPPUNIT_ASSERT(   q1.isSuperseteq(q2) );
}
    
void
TestRacerQuery::runRacerQueryLessThanTest()
{
  Tuple pt1;
  Tuple pt2;
  pt1.push_back(Term("foo",true));
  pt2.push_back(Term("foo",true));

  Term pa("a");

  Query q1("","",Term("p"),Term("q"),Term("foo"),Term("foo"),DLQuery(pa,pt1),AtomSet());
  
  Term pb("b");

  Query q2("","",Term("p"),Term("q"),Term("foo"),Term("foo"),DLQuery(pb,pt2),AtomSet());

  CPPUNIT_ASSERT(q1.getDLQuery().isBoolean());
  CPPUNIT_ASSERT(q2.getDLQuery().isBoolean());
  CPPUNIT_ASSERT( (q1 < q2) && !(q2 < q1) );

  Query q3("","",Term("p"),Term("q"),Term("foo"),Term("foo"),DLQuery(pa,pt1),AtomSet());

  CPPUNIT_ASSERT( !(q1 < q3) && !(q3 < q1) );

  pt1.clear();
  pt2.clear();

  pt1.push_back(Term("X"));
  pt1.push_back(Term("foo",true));

  pt2.push_back(Term("foo",true));
  pt2.push_back(Term("X"));

  Query q4("","",Term("p"),Term("q"),Term("foo"),Term("foo"),DLQuery(pa,pt1),AtomSet());
  Query q5("","",Term("p"),Term("q"),Term("foo"),Term("foo"),DLQuery(pa,pt2),AtomSet());

  CPPUNIT_ASSERT(q4.getDLQuery().isMixed());
  CPPUNIT_ASSERT(q5.getDLQuery().isMixed());
  CPPUNIT_ASSERT( !(q4 < q5) && (q5 < q4) );

  pt1.clear();
  pt2.clear();

  pt1.push_back(Term("X"));
  pt1.push_back(Term("X"));

  pt2.push_back(Term("X"));
  pt2.push_back(Term("Y"));

  Query q6("","",Term("p"),Term("q"),Term("foo"),Term("foo"),DLQuery(pa,pt1),AtomSet());
  Query q7("","",Term("p"),Term("q"),Term("foo"),Term("foo"),DLQuery(pa,pt2),AtomSet());

  CPPUNIT_ASSERT(q6.getDLQuery().isRetrieval());
  CPPUNIT_ASSERT(q7.getDLQuery().isRetrieval());
  CPPUNIT_ASSERT( !(q6 < q7) && !(q7 < q6) );

  pt1.clear();
  pt2.clear();

  pt1.push_back(Term("a"));
  pt1.push_back(Term("b"));

  pt2.push_back(Term("X"));
  pt2.push_back(Term("Y"));

  Query q8("","",Term("p"),Term("q"),Term("foo"),Term("foo"),DLQuery(pa,pt1),AtomSet());
  Query q9("","",Term("p"),Term("q"),Term("foo"),Term("foo"),DLQuery(pa,pt2),AtomSet());

  CPPUNIT_ASSERT(q8.getDLQuery().isBoolean());
  CPPUNIT_ASSERT(q9.getDLQuery().isRetrieval());
  CPPUNIT_ASSERT( !(q8 < q9) && (q9 < q8) );

  pt1.clear();
  pt2.clear();

  pt1.push_back(Term("X"));
  pt1.push_back(Term("a"));

  pt2.push_back(Term("Z"));
  pt2.push_back(Term("a"));

  Query q10("","",Term("p"),Term("q"),Term("foo"),Term("foo"),DLQuery(pa,pt1),AtomSet());
  Query q11("","",Term("p"),Term("q"),Term("foo"),Term("foo"),DLQuery(pa,pt2),AtomSet());

  CPPUNIT_ASSERT(q10.getDLQuery().isMixed());
  CPPUNIT_ASSERT(q11.getDLQuery().isMixed());
  CPPUNIT_ASSERT( !(q10 < q11) && !(q11 < q10) );
}
