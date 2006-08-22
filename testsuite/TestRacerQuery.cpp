/**
 * @file   TestRacerQuery.cpp
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:31:18 2006
 * 
 * @brief  TestCases for Query.
 * 
 * 
 */


#include "TestRacerQuery.h"

#include "Query.h"
#include "KBManager.h"

using namespace dlvhex::dl;
using namespace dlvhex::dl::test;

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
  AtomSet ints1(is1);

  AtomPtr ap21(new Atom("p(b,y)"));
  AtomPtr ap22(new Atom("p(a,x)"));
  AtomPtr ap23(new Atom("q(c,z)"));
  AtomSet is2;
  is2.insert(ap21);
  is2.insert(ap22);
  is2.insert(ap23);
  AtomSet ints2(is2);

  Tuple pt1;
  Tuple pt2;
  pt1.push_back(Term("foo",true));
  pt2.push_back(Term("foo",true));

  Term pa("a");

  KBManager kb("DEFAULT");
  DLQuery::shared_pointer dlq1(new DLQuery(Ontology::createOntology(shop),pa,pt1));
  Query q1(kb,dlq1, Term("p"),Term("q"),Term("foo"),Term("foo"),ints1);
  DLQuery::shared_pointer dlq2(new DLQuery(Ontology::createOntology(shop),pa,pt2));
  Query q2(kb,dlq2, Term("p"),Term("q"),Term("foo"),Term("foo"),ints2);

  const AtomSet& i = q1.getProjectedInterpretation();
  const AtomSet& j = q2.getProjectedInterpretation();

  CPPUNIT_ASSERT(   q1 == q2  );
  CPPUNIT_ASSERT( std::includes(i.begin(),i.end(),j.begin(),j.end()) );
  CPPUNIT_ASSERT( std::includes(j.begin(),j.end(),i.begin(),i.end()) );
}

void
TestRacerQuery::runRacerQuerySubsetTest()
{
  AtomPtr ap11(new Atom("p(b,y)"));
  AtomPtr ap12(new Atom("q(c,z)"));
  AtomSet is1;
  is1.insert(ap11);
  is1.insert(ap12);
  AtomSet ints1(is1);
  
  AtomPtr ap21(new Atom("p(a,x)"));
  AtomPtr ap22(new Atom("p(b,y)"));
  AtomPtr ap23(new Atom("q(c,z)"));
  AtomSet is2;
  is2.insert(ap21);
  is2.insert(ap22);
  is2.insert(ap23);
  AtomSet ints2(is2);

  Tuple pt1;
  Tuple pt2;
  pt1.push_back(Term("foo",true));
  pt2.push_back(Term("foo",true));
  
  Term pa("a");

  KBManager kb("DEFAULT");
  DLQuery::shared_pointer dlq1(new DLQuery(Ontology::createOntology(shop),pa,pt1));
  Query q1(kb,dlq1, Term("p"),Term("q"),Term("foo"),Term("foo"),ints1);
  DLQuery::shared_pointer dlq2(new DLQuery(Ontology::createOntology(shop),pa,pt2));
  Query q2(kb,dlq2, Term("p"),Term("q"),Term("foo"),Term("foo"),ints2);
  
  const AtomSet& i = q1.getProjectedInterpretation();
  const AtomSet& j = q2.getProjectedInterpretation();

  CPPUNIT_ASSERT(   q1 == q2  );
  CPPUNIT_ASSERT( !std::includes(i.begin(),i.end(),j.begin(),j.end()) );
  CPPUNIT_ASSERT( std::includes(j.begin(),j.end(),i.begin(),i.end()) );
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
  AtomSet ints1(is1);
  
  AtomPtr ap21(new Atom("p(b,y)"));
  AtomPtr ap22(new Atom("q(c,z)"));
  AtomSet is2;
  is2.insert(ap21);
  is2.insert(ap22);
  AtomSet ints2(is2);
  
  Tuple pt1;
  Tuple pt2;
  pt1.push_back(Term("foo",true));
  pt2.push_back(Term("foo",true));

  Term pa("a");
  
  KBManager kb("DEFAULT");
  DLQuery::shared_pointer dlq1(new DLQuery(Ontology::createOntology(shop),pa,pt1));
  Query q1(kb,dlq1, Term("p"),Term("q"),Term("foo"),Term("foo"),ints1);
  DLQuery::shared_pointer dlq2(new DLQuery(Ontology::createOntology(shop),pa,pt2));
  Query q2(kb,dlq2, Term("p"),Term("q"),Term("foo"),Term("foo"),ints2);
  
  const AtomSet& i = q1.getProjectedInterpretation();
  const AtomSet& j = q2.getProjectedInterpretation();

  CPPUNIT_ASSERT(   q1 == q2  );
  CPPUNIT_ASSERT( std::includes(i.begin(),i.end(),j.begin(),j.end()) );
  CPPUNIT_ASSERT( !std::includes(j.begin(),j.end(),i.begin(),i.end()) );
}
    
void
TestRacerQuery::runRacerQueryLessThanTest()
{
  Tuple pt1;
  Tuple pt2;
  pt1.push_back(Term("foo",true));
  pt2.push_back(Term("foo",true));

  Term pa("a");

  KBManager kb("DEFAULT");

  DLQuery::shared_pointer dlq1(new DLQuery(Ontology::createOntology(shop),pa,pt1));
  Query q1(kb,dlq1, Term("p"),Term("q"),Term("foo"),Term("foo"),AtomSet());
  
  Term pb("b");

  DLQuery::shared_pointer dlq2(new DLQuery(Ontology::createOntology(shop),pb,pt2));
  Query q2(kb,dlq2, Term("p"),Term("q"),Term("foo"),Term("foo"),AtomSet());

  CPPUNIT_ASSERT(q1.getDLQuery()->isBoolean());
  CPPUNIT_ASSERT(q2.getDLQuery()->isBoolean());
  CPPUNIT_ASSERT( (q1 < q2) && !(q2 < q1) );

  DLQuery::shared_pointer dlq3(new DLQuery(Ontology::createOntology(shop),pa,pt1));
  Query q3(kb,dlq3, Term("p"),Term("q"),Term("foo"),Term("foo"),AtomSet());

  CPPUNIT_ASSERT( !(q1 < q3) && !(q3 < q1) );

  pt1.clear();
  pt2.clear();

  pt1.push_back(Term("X"));
  pt1.push_back(Term("foo",true));

  pt2.push_back(Term("foo",true));
  pt2.push_back(Term("X"));

  DLQuery::shared_pointer dlq4(new DLQuery(Ontology::createOntology(shop),pa,pt1));
  Query q4(kb,dlq4, Term("p"),Term("q"),Term("foo"),Term("foo"),AtomSet());
  DLQuery::shared_pointer dlq5(new DLQuery(Ontology::createOntology(shop),pa,pt2));
  Query q5(kb,dlq5, Term("p"),Term("q"),Term("foo"),Term("foo"),AtomSet());

  CPPUNIT_ASSERT(q4.getDLQuery()->isMixed());
  CPPUNIT_ASSERT(q5.getDLQuery()->isMixed());
  CPPUNIT_ASSERT( !(q4 < q5) && (q5 < q4) );

  pt1.clear();
  pt2.clear();

  pt1.push_back(Term("X"));
  pt1.push_back(Term("X"));

  pt2.push_back(Term("X"));
  pt2.push_back(Term("Y"));

  DLQuery::shared_pointer dlq6(new DLQuery(Ontology::createOntology(shop),pa,pt1));
  Query q6(kb,dlq6, Term("p"),Term("q"),Term("foo"),Term("foo"),AtomSet());
  DLQuery::shared_pointer dlq7(new DLQuery(Ontology::createOntology(shop),pa,pt2));
  Query q7(kb,dlq7, Term("p"),Term("q"),Term("foo"),Term("foo"),AtomSet());

  CPPUNIT_ASSERT(q6.getDLQuery()->isRetrieval());
  CPPUNIT_ASSERT(q7.getDLQuery()->isRetrieval());
  CPPUNIT_ASSERT( !(q6 < q7) && !(q7 < q6) );

  pt1.clear();
  pt2.clear();

  pt1.push_back(Term("a"));
  pt1.push_back(Term("b"));

  pt2.push_back(Term("X"));
  pt2.push_back(Term("Y"));

  DLQuery::shared_pointer dlq8(new DLQuery(Ontology::createOntology(shop),pa,pt1));
  Query q8(kb,dlq8, Term("p"),Term("q"),Term("foo"),Term("foo"),AtomSet());
  DLQuery::shared_pointer dlq9(new DLQuery(Ontology::createOntology(shop),pa,pt2));
  Query q9(kb,dlq9, Term("p"),Term("q"),Term("foo"),Term("foo"),AtomSet());

  CPPUNIT_ASSERT(q8.getDLQuery()->isBoolean());
  CPPUNIT_ASSERT(q9.getDLQuery()->isRetrieval());
  CPPUNIT_ASSERT( !(q8 < q9) && (q9 < q8) );

  pt1.clear();
  pt2.clear();

  pt1.push_back(Term("X"));
  pt1.push_back(Term("a"));

  pt2.push_back(Term("Z"));
  pt2.push_back(Term("a"));

  DLQuery::shared_pointer dlq10(new DLQuery(Ontology::createOntology(shop),pa,pt1));
  Query q10(kb,dlq10,  Term("p"),Term("q"),Term("foo"),Term("foo"),AtomSet());
  DLQuery::shared_pointer dlq11(new DLQuery(Ontology::createOntology(shop),pa,pt2));
  Query q11(kb,dlq11,  Term("p"),Term("q"),Term("foo"),Term("foo"),AtomSet());

  CPPUNIT_ASSERT(q10.getDLQuery()->isMixed());
  CPPUNIT_ASSERT(q11.getDLQuery()->isMixed());
  CPPUNIT_ASSERT( !(q10 < q11) && !(q11 < q10) );
}
