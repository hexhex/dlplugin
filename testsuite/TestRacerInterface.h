// -*- C++ -*-

#ifndef _TESTRACERINTERFACE_H
#define _TESTRACERINTERFACE_H

#include <sstream>
#include <iostream>
#include <string>
#include <map>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "RacerInterface.h"

namespace dlvhex {
namespace racer {


  /**
   * @brief TestCases for RacerDirector. 
   *
   * @test Imports the callback functions and runs some tests on them.
   */
  class TestRacerInterface : public CppUnit::TestCase
  {
  private:
    std::string shop;
    std::string shopuri;

  public: 
    virtual void setUp()
    {
      const char* ex = getenv("EXAMPLES");
      if (ex)
	{
	  shop =
	    std::string(ex) +
	    std::string("/shop.owl");
	  shopuri = "file:" + std::string(ex) + "/shop.owl";
	}
      else
	{
	  shop = std::string("./shop.owl");
	  shopuri = std::string("file:shop.owl");
	}
    }

    void output(const std::vector<Tuple>& tv)
    {
      std::cout << std::endl;
      for(std::vector<Tuple>::const_iterator it = tv.begin();
	  it != tv.end();
	  it++)
	{
	  Tuple t = *it;
	  std::cout << "(";
	  // copy all elements of t to cout
	  std::copy(t.begin(), t.end(),
		    std::ostream_iterator<Term>(std::cout, ") ("));
	  std::cout << ")" << std::endl;
	}
    }

    void runRacerConceptTest()
    {
      PluginInterface* pi = PLUGINIMPORTFUNCTION();

      PluginInterface::AtomFunctionMap m;
      pi->getAtoms(m);

      GAtomSet a1,a2,a3,a4;

      GAtom pc0("\"plusC\"(\"Part\",\"moo1\")");
      GAtom pc1("\"plusC\"(\"Part\",\"moo2\")");
      GAtom pc2("\"plusC\"(\"Part\",\"moo3\")");
      GAtom pc3("\"plusC\"(\"Part\",\"moo4\")");
      a1.insert(pc0);
      a1.insert(pc1);
      a1.insert(pc2);
      a1.insert(pc3);

      GAtom mc0("\"minusC\"(\"Part\",\"moo5\")");
      a2.insert(mc0);

      GAtom pr0("\"plusR\"(\"provides\",\"s1\",\"moo4\")");
      a3.insert(pr0);

      Interpretation in;
      in.add(a1);
      in.add(a2);
      in.add(a3);
      in.add(a4);

      Tuple parms;
      parms.push_back(Term(shop));
      parms.push_back(Term("\"plusC\""));
      parms.push_back(Term("\"minusC\""));
      parms.push_back(Term("\"plusR\""));
      parms.push_back(Term());
      parms.push_back(Term("\"Part\""));

      std::vector<Tuple> out;

      CPPUNIT_ASSERT_NO_THROW( m["dlC"]->retrieve(in, parms, out) );

      output(out);
    }
    
    void runRacerRoleTest()
    {
      PluginInterface* pi = PLUGINIMPORTFUNCTION();

      PluginInterface::AtomFunctionMap m;
      pi->getAtoms(m);

      GAtomSet a1,a2,a3,a4;

      GAtom pc0("\"plusC\"(\"Part\",\"moo1\")");
      GAtom pc1("\"plusC\"(\"Part\",\"moo2\")");
      GAtom pc2("\"plusC\"(\"Part\",\"moo3\")");
      GAtom pc3("\"plusC\"(\"Part\",\"moo4\")");
      a1.insert(pc0);
      a1.insert(pc1);
      a1.insert(pc2);
      a1.insert(pc3);

      GAtom mc0("\"minusC\"(\"Part\",\"moo5\")");
      a2.insert(mc0);

      GAtom pr0("\"plusR\"(\"provides\",\"s1\",\"moo4\")");
      a3.insert(pr0);

      Interpretation in;
      in.add(a1);
      in.add(a2);
      in.add(a3);
      in.add(a4);

      Tuple parms;
      parms.push_back(Term(shop));
      parms.push_back(Term("\"plusC\""));
      parms.push_back(Term("\"minusC\""));
      parms.push_back(Term("\"plusR\""));
      parms.push_back(Term());
      parms.push_back(Term("\"provides\""));

      std::vector<Tuple> out;

      CPPUNIT_ASSERT_NO_THROW( m["dlR"]->retrieve(in, parms, out) );

      output(out);
    }

    void runRacerIsConceptTest()
    {
      PluginInterface* pi = PLUGINIMPORTFUNCTION();

      PluginInterface::AtomFunctionMap m;
      pi->getAtoms(m);

      GAtomSet a1,a2,a3,a4;

      GAtom pc0("\"plusC\"(\"Part\",\"moo1\")");
      GAtom pc1("\"plusC\"(\"Part\",\"moo2\")");
      GAtom pc2("\"plusC\"(\"Part\",\"moo3\")");
      GAtom pc3("\"plusC\"(\"Part\",\"moo4\")");
      a1.insert(pc0);
      a1.insert(pc1);
      a1.insert(pc2);
      a1.insert(pc3);

      GAtom mc0("\"minusC\"(\"Part\",\"moo5\")");
      a2.insert(mc0);

      GAtom pr0("\"plusR\"(\"provides\",\"s1\",\"moo4\")");
      a3.insert(pr0);

      Interpretation in;
      in.add(a1);
      in.add(a2);
      in.add(a3);
      in.add(a4);

      Tuple parms;
      parms.push_back(Term(shop));
      parms.push_back(Term("\"plusC\""));
      parms.push_back(Term("\"minusC\""));
      parms.push_back(Term("\"plusR\""));
      parms.push_back(Term());
      parms.push_back(Term("\"Part\""));

      Tuple indv;
      indv.push_back(Term("\"cpu\""));

      bool ret;

      CPPUNIT_ASSERT_NO_THROW( ret = m["dlC"]->query(in, parms, indv) );
      std::cout << "Got: " << ret << std::endl;

      CPPUNIT_ASSERT_NO_THROW( ret = m["dlC"]->query(in, parms, indv) );
      std::cout << "Got: " << ret << std::endl;
    }

    void runRacerIsRoleTest()
    {
      PluginInterface* pi = PLUGINIMPORTFUNCTION();

      PluginInterface::AtomFunctionMap m;
      pi->getAtoms(m);

      GAtomSet a1,a2,a3,a4;

      GAtom pc0("\"plusC\"(\"Part\",\"moo1\")");
      GAtom pc1("\"plusC\"(\"Part\",\"moo2\")");
      GAtom pc2("\"plusC\"(\"Part\",\"moo3\")");
      GAtom pc3("\"plusC\"(\"Part\",\"moo4\")");
      a1.insert(pc0);
      a1.insert(pc1);
      a1.insert(pc2);
      a1.insert(pc3);

      GAtom mc0("\"minusC\"(\"Part\",\"moo5\")");
      a2.insert(mc0);

      GAtom pr0("\"plusR\"(\"provides\",\"s1\",\"moo4\")");
      a3.insert(pr0);

      Interpretation in;
      in.add(a1);
      in.add(a2);
      in.add(a3);
      in.add(a4);

      Tuple parms;
      parms.push_back(Term(shop));
      parms.push_back(Term("\"plusC\""));
      parms.push_back(Term("\"minusC\""));
      parms.push_back(Term("\"plusR\""));
      parms.push_back(Term());
      parms.push_back(Term("\"provides\""));

      Tuple indv;
      indv.push_back(Term("\"s1\""));
      indv.push_back(Term("\"moo4\""));

      bool ret;

      CPPUNIT_ASSERT_NO_THROW( ret = m["dlR"]->query(in, parms, indv) );
      std::cout << "Got: " << ret << std::endl;

      CPPUNIT_ASSERT_NO_THROW( ret = m["dlR"]->query(in, parms, indv) );
      std::cout << "Got: " << ret << std::endl;
    }


    void runGetUniverseTest()
    {
      PluginInterface* pi = PLUGINIMPORTFUNCTION();

      std::list<Term> lterm;

      pi->getUniverse(shopuri, lterm);

      std::cout << "(";
      std::copy(lterm.begin(), lterm.end(),
		std::ostream_iterator<Term>(std::cout, ") ("));
      std::cout << ")" << std::endl;
    }

    static CppUnit::Test *suite()
    {
      CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestRacerInterface");
      
      suiteOfTests->addTest(new CppUnit::TestCaller<TestRacerInterface>
			    ( 
			     "RacerConcept", 
			     &TestRacerInterface::runRacerConceptTest
			     )
			    );
      
      suiteOfTests->addTest(new CppUnit::TestCaller<TestRacerInterface>
			    ( 
			     "RacerRole", 
			     &TestRacerInterface::runRacerRoleTest
			     )
			    );

      suiteOfTests->addTest(new CppUnit::TestCaller<TestRacerInterface>
			    ( 
			     "RacerIsConcept", 
			     &TestRacerInterface::runRacerIsConceptTest
			     )
			    );

      suiteOfTests->addTest(new CppUnit::TestCaller<TestRacerInterface>
			    ( 
			     "RacerIsRole", 
			     &TestRacerInterface::runRacerIsRoleTest
			     )
			    );

      suiteOfTests->addTest(new CppUnit::TestCaller<TestRacerInterface>
			    ( 
			     "GetUniverse", 
			     &TestRacerInterface::runGetUniverseTest
			     )
			    );

      return suiteOfTests;
    }
    
  };

} // namespace racer
} // namespace dlvhex

#endif /* _TESTRACERBUILDER_H */
