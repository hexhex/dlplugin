/**
 * @file   TestRacerInterface.cpp
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:22:04 2006
 * 
 * @brief  
 * 
 * 
 */

#include "RacerInterface.h"

#include "TestRacerInterface.h"

#include <sstream>
#include <iostream>
#include <string>
#include <map>
#include <iterator>


using namespace dlvhex::racer;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestRacerInterface);

void
TestRacerInterface::setUp()
{
  PluginInterface* pi = PLUGINIMPORTFUNCTION();
  RacerInterface* ri = dynamic_cast<RacerInterface*>(pi);
  std::vector<std::string> v;
  ri->setOptions(false, v, std::cout);
}

void
TestRacerInterface::runRacerConsistentTest()
{
  PluginInterface* pi = PLUGINIMPORTFUNCTION();
  
  PluginInterface::AtomFunctionMap m;
  pi->getAtoms(m);
  
  AtomSet in;
  
  Tuple parms;
  parms.push_back(Term(shop));
  parms.push_back(Term("\"plusC\""));
  parms.push_back(Term("\"minusC\""));
  parms.push_back(Term("\"plusR\""));
  parms.push_back(Term("\"minusR\""));
  
  // atomic mode
  Tuple pat;
      
  PluginAtom::Answer a;
  PluginAtom::Query q1(in, parms, pat);
  
  CPPUNIT_ASSERT_NO_THROW( m["dlConsistent"]->retrieve(q1, a) );
  CPPUNIT_ASSERT(a.getTuples()->size() == 1);

  AtomSet a1,a2,a3,a4;
  
  AtomPtr mc0(new Atom("\"minusC\"(\"Part\",\"cpu\")"));
  a2.insert(mc0);

  in.insert(a1);
  in.insert(a2);
  in.insert(a3);
  in.insert(a4);

  PluginAtom::Query q2(in, parms, pat);

  CPPUNIT_ASSERT_NO_THROW( m["dlConsistent"]->retrieve(q2, a) );
  CPPUNIT_ASSERT(a.getTuples()->size() == 0);
}

void
TestRacerInterface::runRacerConceptTest()
{
  PluginInterface* pi = PLUGINIMPORTFUNCTION();
  
  PluginInterface::AtomFunctionMap m;
  pi->getAtoms(m);
  
  AtomSet a1,a2,a3,a4;
  
  AtomPtr pc0(new Atom("\"plusC\"(\"Part\",\"moo1\")"));
  AtomPtr pc1(new Atom("\"plusC\"(\"Part\",\"moo2\")"));
  AtomPtr pc2(new Atom("\"plusC\"(\"Part\",\"moo3\")"));
  AtomPtr pc3(new Atom("\"plusC\"(\"Part\",\"moo4\")"));
  a1.insert(pc0);
  a1.insert(pc1);
  a1.insert(pc2);
  a1.insert(pc3);
  
  AtomPtr mc0(new Atom("\"minusC\"(\"Part\",\"moo5\")"));
  a2.insert(mc0);
  
  AtomPtr pr0(new Atom("\"plusR\"(\"provides\",\"s1\",\"moo4\")"));
  a3.insert(pr0);
  
  AtomSet in;
  in.insert(a1);
  in.insert(a2);
  in.insert(a3);
  in.insert(a4);
  
  Tuple parms;
  parms.push_back(Term(shop));
  parms.push_back(Term("\"plusC\""));
  parms.push_back(Term("\"minusC\""));
  parms.push_back(Term("\"plusR\""));
  parms.push_back(Term("\"minusR\""));
  parms.push_back(Term("\"Part\""));
  
  // retrieval mode
  Tuple pat;
  pat.push_back(Term("X"));
      
  PluginAtom::Answer a;
  PluginAtom::Query q(in, parms, pat);
  
  CPPUNIT_ASSERT_NO_THROW( m["dlC"]->retrieve(q, a) );
  
  output(*a.getTuples());
}
    
void
TestRacerInterface::runRacerRoleTest()
{
  PluginInterface* pi = PLUGINIMPORTFUNCTION();

  PluginInterface::AtomFunctionMap m;
  pi->getAtoms(m);

  AtomSet a1,a2,a3,a4;

  AtomPtr pc0(new Atom("\"plusC\"(\"Part\",\"moo1\")"));
  AtomPtr pc1(new Atom("\"plusC\"(\"Part\",\"moo2\")"));
  AtomPtr pc2(new Atom("\"plusC\"(\"Part\",\"moo3\")"));
  AtomPtr pc3(new Atom("\"plusC\"(\"Part\",\"moo4\")"));
  a1.insert(pc0);
  a1.insert(pc1);
  a1.insert(pc2);
  a1.insert(pc3);
  
  AtomPtr mc0(new Atom("\"minusC\"(\"Part\",\"moo5\")"));
  a2.insert(mc0);
  
  AtomPtr pr0(new Atom("\"plusR\"(\"provides\",\"s1\",\"moo4\")"));
  a3.insert(pr0);
  
  AtomSet in;
  in.insert(a1);
  in.insert(a2);
  in.insert(a3);
  in.insert(a4);
  
  Tuple parms;
  parms.push_back(Term(shop));
  parms.push_back(Term("\"plusC\""));
  parms.push_back(Term("\"minusC\""));
  parms.push_back(Term("\"plusR\""));
  parms.push_back(Term("\"minusR\""));
  parms.push_back(Term("\"provides\""));
  
  // retrieval mode
  Tuple pat;
  pat.push_back(Term("X"));
  pat.push_back(Term("Y"));
  
  PluginAtom::Answer a;
  PluginAtom::Query q(in, parms, pat);
  
  CPPUNIT_ASSERT_NO_THROW( m["dlR"]->retrieve(q, a) );
  
  output(*a.getTuples());
}

void
TestRacerInterface::runRacerIsConceptTest()
{
  PluginInterface* pi = PLUGINIMPORTFUNCTION();
  
  PluginInterface::AtomFunctionMap m;
  pi->getAtoms(m);
  
  AtomSet a1,a2,a3,a4;
  
  AtomPtr pc0(new Atom("\"plusC\"(\"Part\",\"moo1\")"));
  AtomPtr pc1(new Atom("\"plusC\"(\"Part\",\"moo2\")"));
  AtomPtr pc2(new Atom("\"plusC\"(\"Part\",\"moo3\")"));
  AtomPtr pc3(new Atom("\"plusC\"(\"Part\",\"moo4\")"));
  a1.insert(pc0);
  a1.insert(pc1);
  a1.insert(pc2);
  a1.insert(pc3);
  
  AtomPtr mc0(new Atom("\"minusC\"(\"Part\",\"moo5\")"));
  a2.insert(mc0);
  
  AtomPtr pr0(new Atom("\"plusR\"(\"provides\",\"s1\",\"moo4\")"));
  a3.insert(pr0);
  
  AtomSet in;
  in.insert(a1);
  in.insert(a2);
  in.insert(a3);
  in.insert(a4);
  
  Tuple parms;
  parms.push_back(Term(shop));
  parms.push_back(Term("\"plusC\""));
  parms.push_back(Term("\"minusC\""));
  parms.push_back(Term("\"plusR\""));
  parms.push_back(Term("\"minusR\""));
  parms.push_back(Term("\"Part\""));
  
  // query mode
  Tuple pat;
  pat.push_back(Term("\"cpu\""));
  
  PluginAtom::Answer a;
  PluginAtom::Query q(in, parms, pat);
  
  CPPUNIT_ASSERT_NO_THROW( m["dlC"]->retrieve(q, a) );
  CPPUNIT_ASSERT( a.getTuples()->size() == 1 ); // 1 => cpu \in Part
      
  CPPUNIT_ASSERT_NO_THROW( m["dlC"]->retrieve(q, a) );
  CPPUNIT_ASSERT( a.getTuples()->size() == 1 ); // 1 => cpu \in Part
}

void
TestRacerInterface::runRacerIsRoleTest()
{
  PluginInterface* pi = PLUGINIMPORTFUNCTION();
  
  PluginInterface::AtomFunctionMap m;
  pi->getAtoms(m);

  AtomSet a1,a2,a3,a4;

  AtomPtr pc0(new Atom("\"plusC\"(\"Part\",\"moo1\")"));
  AtomPtr pc1(new Atom("\"plusC\"(\"Part\",\"moo2\")"));
  AtomPtr pc2(new Atom("\"plusC\"(\"Part\",\"moo3\")"));
  AtomPtr pc3(new Atom("\"plusC\"(\"Part\",\"moo4\")"));
  a1.insert(pc0);
  a1.insert(pc1);
  a1.insert(pc2);
  a1.insert(pc3);
  
  AtomPtr mc0(new Atom("\"minusC\"(\"Part\",\"moo5\")"));
  a2.insert(mc0);
  
  AtomPtr pr0(new Atom("\"plusR\"(\"provides\",\"s1\",\"moo4\")"));
  a3.insert(pr0);
  
  AtomSet in;
  in.insert(a1);
  in.insert(a2);
  in.insert(a3);
  in.insert(a4);
  
  Tuple parms;
  parms.push_back(Term(shop));
  parms.push_back(Term("\"plusC\""));
  parms.push_back(Term("\"minusC\""));
  parms.push_back(Term("\"plusR\""));
  parms.push_back(Term("\"minusR\""));
  parms.push_back(Term("\"provides\""));
  
  // query mode
  Tuple pat;
  pat.push_back(Term("\"s1\""));
  pat.push_back(Term("\"moo4\""));
  
  PluginAtom::Answer a;
  PluginAtom::Query q(in, parms, pat);
  
  CPPUNIT_ASSERT_NO_THROW( m["dlR"]->retrieve(q, a) );
  CPPUNIT_ASSERT( a.getTuples()->size() == 1); // 1 => (s1,moo4) \in provides
  
  CPPUNIT_ASSERT_NO_THROW( m["dlR"]->retrieve(q, a) );
  CPPUNIT_ASSERT( a.getTuples()->size() == 1); // 1 => (s1,moo4) \in provides
}

void
TestRacerInterface::runRacerRoleFillersTest()
{
  PluginInterface* pi = PLUGINIMPORTFUNCTION();

  PluginInterface::AtomFunctionMap m;
  pi->getAtoms(m);

  AtomSet a1,a2,a3,a4;

  AtomPtr pc0(new Atom("\"plusC\"(\"Part\",\"moo1\")"));
  AtomPtr pc1(new Atom("\"plusC\"(\"Part\",\"moo2\")"));
  AtomPtr pc2(new Atom("\"plusC\"(\"Part\",\"moo3\")"));
  AtomPtr pc3(new Atom("\"plusC\"(\"Part\",\"moo4\")"));
  a1.insert(pc0);
  a1.insert(pc1);
  a1.insert(pc2);
  a1.insert(pc3);
  
  AtomPtr mc0(new Atom("\"minusC\"(\"Part\",\"moo5\")"));
  a2.insert(mc0);
  
  AtomPtr pr0(new Atom("\"plusR\"(\"provides\",\"s1\",\"moo4\")"));
  a3.insert(pr0);

  AtomSet in;
  in.insert(a1);
  in.insert(a2);
  in.insert(a3);
  in.insert(a4);

  Tuple parms;
  parms.push_back(Term(shop));
  parms.push_back(Term("\"plusC\""));
  parms.push_back(Term("\"minusC\""));
  parms.push_back(Term("\"plusR\""));
  parms.push_back(Term("\"minusR\""));
  parms.push_back(Term("\"provides\""));

  // pattern retrieval mode
  Tuple pat;
  pat.push_back(Term("X"));
  pat.push_back(Term("\"cpu\""));

  PluginAtom::Answer ans1;
  PluginAtom::Query q1(in, parms, pat);

  CPPUNIT_ASSERT_NO_THROW( m["dlR"]->retrieve(q1, ans1) );
  CPPUNIT_ASSERT( ans1.getTuples()->size() == 3); // cpu provided in s1,s3,s5
  
  output(*ans1.getTuples());
      
  // pattern retrieval mode
  pat.clear();
  pat.push_back(Term("\"s8\""));
  pat.push_back(Term("Y"));

  PluginAtom::Answer ans2;
  PluginAtom::Query q2(in, parms, pat);

  CPPUNIT_ASSERT_NO_THROW( m["dlR"]->retrieve(q2, ans2) );
  CPPUNIT_ASSERT( ans2.getTuples()->size() == 4); // s8 provides dvdrom,cdrom,soundcard,memory
  
  output(*ans2.getTuples());

  // and now try it with a different variable name -> should be in cache
  pat.clear();
  pat.push_back(Term("Y"));
  pat.push_back(Term("\"cpu\""));
  
  PluginAtom::Answer ans3;
  PluginAtom::Query q3(in, parms, pat);

  CPPUNIT_ASSERT_NO_THROW( m["dlR"]->retrieve(q3, ans3) );
  CPPUNIT_ASSERT( ans3.getTuples()->size() == 3); // cpu provided in s1,s3,s5
  
  output(*ans3.getTuples());
}


void
TestRacerInterface::runRacerDatatypeRoleFillersTest()
{
  PluginInterface* pi = PLUGINIMPORTFUNCTION();

  PluginInterface::AtomFunctionMap m;
  pi->getAtoms(m);

  AtomSet a1,a2,a3,a4;

  AtomSet in;
  in.insert(a1);
  in.insert(a2);
  in.insert(a3);
  in.insert(a4);

  Tuple parms;
  parms.push_back(Term(test));
  parms.push_back(Term("\"a\""));
  parms.push_back(Term("\"b\""));
  parms.push_back(Term("\"c\""));
  parms.push_back(Term("\"d\""));
  parms.push_back(Term("\"hasTestValue\""));

  // pattern retrieval mode
  Tuple pat;
  pat.push_back(Term("\"test1\""));
  pat.push_back(Term("X"));

  PluginAtom::Answer ans1;
  PluginAtom::Query q1(in, parms, pat);

  CPPUNIT_ASSERT_NO_THROW( m["dlDR"]->retrieve(q1, ans1) );
  CPPUNIT_ASSERT( ans1.getTuples()->size() == 2); // test1 hasTestValue 1,2
  
  output(*ans1.getTuples());

  // retrieval mode
  pat.clear();
  pat.push_back(Term("X"));
  pat.push_back(Term("Y"));

  PluginAtom::Answer ans2;
  PluginAtom::Query q2(in, parms, pat);

  CPPUNIT_ASSERT_NO_THROW( m["dlDR"]->retrieve(q2, ans2) );
  CPPUNIT_ASSERT( ans2.getTuples()->size() == 3); // (test1 1) (test1 2) (test2 2)
  
  output(*ans2.getTuples());
}


void
TestRacerInterface::runRacerConjQueryTest()
{
  PluginInterface* pi = PLUGINIMPORTFUNCTION();

  PluginInterface::AtomFunctionMap m;
  pi->getAtoms(m);

  AtomSet a1,a2,a3,a4;

  AtomSet in;
  in.insert(a1);
  in.insert(a2);
  in.insert(a3);
  in.insert(a4);

  Tuple parms;
  parms.push_back(Term(examples + "/wires.owl"));
  parms.push_back(Term("\"a\""));
  parms.push_back(Term("\"b\""));
  parms.push_back(Term("\"c\""));
  parms.push_back(Term("\"d\""));
  parms.push_back(Term("\"http://wires#wired(X,Z1),http://wires#wired(Z1,Z2),http://wires#wired(Z2,Z3),http://wires#wired(Z3,Y)\"", true));

  Tuple pat;
  pat.push_back(Term("X"));
  pat.push_back(Term("Y"));

  PluginAtom::Answer ans1;
  PluginAtom::Query q1(in, parms, pat);

  CPPUNIT_ASSERT_NO_THROW( m["dlCQ2"]->retrieve(q1, ans1) );
  CPPUNIT_ASSERT( ans1.getTuples()->size() == 15);
  
  output(*ans1.getTuples());
}

void
TestRacerInterface::runGetUniverseTest()
{
  PluginInterface* pi = PLUGINIMPORTFUNCTION();
  
  std::list<Term> lterm;
  
  pi->getUniverse(shopuri, lterm);
  
  if (!lterm.empty())
    {
      std::cout << "{";
      std::copy(lterm.begin(),
		--lterm.end(),
		std::ostream_iterator<Term>(std::cout, ",")
		);
      std::cout << lterm.back() << "}" << std::endl;
    }

  CPPUNIT_ASSERT( lterm.size() == 20 );
  
  lterm.clear();

  std::string kruri = "http://www.kr.tuwien.ac.at/staff/roman/swlp/examples/shop.owl";
  pi->getUniverse(kruri, lterm);
  
  if (!lterm.empty())
    {
      std::cout << "{";
      std::copy(lterm.begin(),
		--lterm.end(),
		std::ostream_iterator<Term>(std::cout, ",")
		);
      std::cout << lterm.back() << "}" << std::endl;
    }

  CPPUNIT_ASSERT( lterm.size() == 20 );

}
