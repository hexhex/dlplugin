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

using namespace std;

namespace dlvex_racer {


  /**
   * @brief TestCases for RacerDirector. 
   *
   * @test Imports the callback functions and runs some tests on them.
   */
  class TestRacerInterface : public CppUnit::TestCase
  {
  private:
    std::string shop;

  public: 
    virtual void setUp()
    {
      const char* ex = getenv("EXAMPLES");
      if (ex)
	{
	  shop =
	    std::string("kb(\"") +
	    std::string(ex) +
	    std::string("/shop.owl\")");
	}
      else
	{
	  shop = std::string("kb(\"./shop.owl\")");
	}
    }

    void output(const PluginAtom::TUPLEVECTOR& tv)
    {
      std::cout << std::endl;
      for(PluginAtom::TUPLEVECTOR::const_iterator it = tv.begin();
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

      GAtomSet a0,a1,a2,a3,a4,a5;

      GAtom kb0(shop);
      a0.insert(kb0);

      GAtom pc0("plusC(\"Part\",\"moo1\")");
      GAtom pc1("plusC(\"Part\",\"moo2\")");
      GAtom pc2("plusC(\"Part\",\"moo3\")");
      GAtom pc3("plusC(\"Part\",\"moo4\")");
      a1.insert(pc0);
      a1.insert(pc1);
      a1.insert(pc2);
      a1.insert(pc3);

      GAtom mc0("minusC(\"Part\",\"moo5\")");
      a2.insert(mc0);

      GAtom pr0("plusR(\"provides\",\"s1\",\"moo4\")");
      a3.insert(pr0);

      GAtom q0("retrieve(\"Part\")");
      a5.insert(q0);

      PluginAtom::FACTSETVECTOR in;
      in.push_back(a0);
      in.push_back(a1);
      in.push_back(a2);
      in.push_back(a3);
      in.push_back(a4);
      in.push_back(a5);

      PluginAtom::TUPLEVECTOR out;

      CPPUNIT_ASSERT_NO_THROW( m["racerC"]->retrieve(in, out) );

      output(out);
    }
    
    void runRacerRoleTest()
    {
      PluginInterface* pi = PLUGINIMPORTFUNCTION();

      PluginInterface::AtomFunctionMap m;
      pi->getAtoms(m);

      GAtomSet a0,a1,a2,a3,a4,a5;

      GAtom kb0(shop);
      a0.insert(kb0);

      GAtom pc0("plusC(\"Part\",\"moo1\")");
      GAtom pc1("plusC(\"Part\",\"moo2\")");
      GAtom pc2("plusC(\"Part\",\"moo3\")");
      GAtom pc3("plusC(\"Part\",\"moo4\")");
      a1.insert(pc0);
      a1.insert(pc1);
      a1.insert(pc2);
      a1.insert(pc3);

      GAtom mc0("minusC(\"Part\",\"moo5\")");
      a2.insert(mc0);

      GAtom pr0("plusR(\"provides\",\"s1\",\"moo4\")");
      a3.insert(pr0);

      GAtom q0("retrieve(\"provides\")");
      a5.insert(q0);

      PluginAtom::FACTSETVECTOR in;
      in.push_back(a0);
      in.push_back(a1);
      in.push_back(a2);
      in.push_back(a3);
      in.push_back(a4);
      in.push_back(a5);

      PluginAtom::TUPLEVECTOR out;

      CPPUNIT_ASSERT_NO_THROW( m["racerR"]->retrieve(in, out) );

      output(out);
    }

    void runRacerIsConceptTest()
    {
      PluginInterface* pi = PLUGINIMPORTFUNCTION();

      PluginInterface::AtomFunctionMap m;
      pi->getAtoms(m);

      GAtomSet a0,a1,a2,a3,a4,a5;

      GAtom kb0(shop);
      a0.insert(kb0);

      GAtom pc0("plusC(\"Part\",\"moo1\")");
      GAtom pc1("plusC(\"Part\",\"moo2\")");
      GAtom pc2("plusC(\"Part\",\"moo3\")");
      GAtom pc3("plusC(\"Part\",\"moo4\")");
      a1.insert(pc0);
      a1.insert(pc1);
      a1.insert(pc2);
      a1.insert(pc3);

      GAtom mc0("minusC(\"Part\",\"moo5\")");
      a2.insert(mc0);

      GAtom pr0("plusR(\"provides\",\"s1\",\"moo4\")");
      a3.insert(pr0);

      GAtom q0("query(\"Part\",\"cpu\")");
      a5.insert(q0);

      PluginAtom::FACTSETVECTOR in;
      in.push_back(a0);
      in.push_back(a1);
      in.push_back(a2);
      in.push_back(a3);
      in.push_back(a4);
      in.push_back(a5);

      Tuple out;
      bool ret;

      CPPUNIT_ASSERT_NO_THROW( ret = m["racerC"]->query(in, out) );
      std::cout << "Got: " << ret << std::endl;

      CPPUNIT_ASSERT_NO_THROW( ret = m["racerC"]->query(in, out) );
      std::cout << "Got: " << ret << std::endl;
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

      return suiteOfTests;
    }
    
  };

} /* namespace dlvex_racer */

#endif /* _TESTRACERBUILDER_H */
