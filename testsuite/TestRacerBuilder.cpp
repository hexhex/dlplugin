/**
 * @file   TestRacerBuilder.cpp
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:06:24 2006
 * 
 * @brief  
 * 
 * 
 */

#include "TestRacerBuilder.h"

using namespace dlvhex::racer;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestRacerBuilder);

void
TestRacerBuilder::runRacerPosIndBuilderTest()
{
  std::stringstream sst;
      
  AtomSet pc;
  pc.insert(Atom("plusC(\"Part\",\"nic\")"));
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
    
void
TestRacerBuilder::runRacerNegIndBuilderTest()
{
  std::stringstream sst;
      
  AtomSet mc;
  mc.insert(Atom("minusC(\"Part\",\"nic\")"));
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
    
void
TestRacerBuilder::runRacerPosPairBuilderTest()
{
  std::stringstream sst;

  AtomSet pr;
  pr.insert(Atom("plusR(\"Part\",\"nic\",\"sic\")"));
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
