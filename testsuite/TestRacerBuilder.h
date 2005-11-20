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


namespace dlvhex {
namespace racer {


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
      std::stringstream sst;
      
      GAtomSet pc;
      pc.insert(GAtom("plusC(\"Part\",\"nic\")"));
      Interpretation ints(pc);

      Query q;

      q.setInterpretation(ints);
      q.setPlusC(Term("plusC"));
      q.setNamespace("file:shop#");

      RacerStateBuilder pib(sst);
      pib.buildCommand(q);
      
      std::string s = sst.str();

      CPPUNIT_ASSERT(s == "(state (instance |file:shop#nic| |file:shop#Part|))\n");
    }
    
    void runRacerNegIndBuilderTest()
    {
      std::stringstream sst;
      
      GAtomSet mc;
      mc.insert(GAtom("minusC(\"Part\",\"nic\")"));
      Interpretation ints(mc);

      Query q;

      q.setInterpretation(ints);
      q.setMinusC(Term("minusC"));
      q.setNamespace("file:shop#");

      RacerStateBuilder pib(sst);
      pib.buildCommand(q);
      
      std::string s = sst.str();

      CPPUNIT_ASSERT(s == "(state (instance |file:shop#nic| (not |file:shop#Part|)))\n");
    }
    
    void runRacerPosPairBuilderTest()
    {
      std::stringstream sst;

      GAtomSet pr;
      pr.insert(GAtom("plusR(\"Part\",\"nic\",\"sic\")"));
      Interpretation ints(pr);

      Query q;

      q.setInterpretation(ints);
      q.setPlusR(Term("plusR"));
      q.setNamespace("file:shop#");
      
      RacerStateBuilder pib(sst);
      pib.buildCommand(q);
      
      std::string s = sst.str();

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

} // namespace racer
} // namespace dlvhex

#endif /* _TESTRACERBUILDER_H */
