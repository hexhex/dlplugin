// -*- C++ -*-

#ifndef _TESTRACERBUILDER_H
#define _TESTRACERBUILDER_H

#include <sstream>
#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "RacerBuilder.h"

using namespace std;

namespace dlvex_racer {


  /**
   * @brief TestCases for RacerBuilder and its children.
   *
   * @test Builds various RACER commands and checks if they are valid.
   */
  class TestRacerBuilder : public CppUnit::TestCase
  { 
  public: 
    void runRacerPosIndBuilderTest()
    {
      stringstream sst;
      
      GAtomSet pc;
      pc.insert(GAtom("plusC(\"Part\",\"nic\")"));

      Query q;

      q.setPlusConcept(pc);
      q.setNamespace("file:shop#");

      RacerStateBuilder pib(sst);
      pib.buildCommand(q);
      
      string s = sst.str();

      CPPUNIT_ASSERT(s == "(state (instance |file:shop#nic| |file:shop#Part|))\n");
    }
    
    void runRacerNegIndBuilderTest()
    {
      stringstream sst;
      
      GAtomSet mc;
      mc.insert(GAtom("minusC(\"Part\",\"nic\")"));

      Query q;

      q.setMinusConcept(mc);
      q.setNamespace("file:shop#");

      RacerStateBuilder pib(sst);
      pib.buildCommand(q);
      
      string s = sst.str();

      CPPUNIT_ASSERT(s == "(state (instance |file:shop#nic| (not |file:shop#Part|)))\n");
    }
    
    void runRacerPosPairBuilderTest()
    {
      stringstream sst;

      GAtomSet pr;
      pr.insert(GAtom("plusR(\"Part\",\"nic\",\"sic\")"));

      Query q;

      q.setPlusRole(pr);
      q.setNamespace("file:shop#");
      
      RacerStateBuilder pib(sst);
      pib.buildCommand(q);
      
      string s = sst.str();

      CPPUNIT_ASSERT(s == "(state (related |file:shop#nic| |file:shop#sic| |file:shop#Part|))\n");
    }
    
    static CppUnit::Test *suite()
    {
      CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestRacerBuilder");
      
      suiteOfTests->addTest(new CppUnit::TestCaller<TestRacerBuilder>
			    ( 
			     "RacerPosIndBuilder", 
			     &TestRacerBuilder::runRacerPosIndBuilderTest
			     )
			    );
      
      suiteOfTests->addTest(new CppUnit::TestCaller<TestRacerBuilder>
			    ( 
			     "RacerNegIndBuilder", 
			     &TestRacerBuilder::runRacerNegIndBuilderTest
			     )
			    );
      
      suiteOfTests->addTest(new CppUnit::TestCaller<TestRacerBuilder>
			    ( 
			     "RacerPosPairBuilder", 
			     &TestRacerBuilder::runRacerPosPairBuilderTest
			     )
			    );
      
      return suiteOfTests;
    }
    
  };

} /* namespace dlvex_racer */

#endif /* _TESTRACERBUILDER_H */
