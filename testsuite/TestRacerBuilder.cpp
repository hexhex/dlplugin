/**
 * @file   TestRacerBuilder.cpp
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:06:24 2006
 * 
 * @brief  
 * 
 * 
 */

#include "RacerBuilder.h"
#include "Query.h"

#include "TestRacerBuilder.h"

#include <sstream>
#include <iostream>
#include <string>


using namespace dlvhex::racer;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestRacerBuilder);

void
TestRacerBuilder::runRacerPosIndBuilderTest()
{
  std::stringstream sst;
      
  AtomSet pc;
  AtomPtr ap(new Atom("plusC(\"Part\",\"nic\")"));
  pc.insert(ap);
  Interpretation ints(pc);

  Query q("foo.owl","file:shop#",Term("plusC"),Term(""),Term(""),Term(""),
	  DLQuery(Term(),Tuple()), ints);

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
  AtomPtr ap(new Atom("minusC(\"Part\",\"nic\")"));
  mc.insert(ap);
  Interpretation ints(mc);

  Query q("foo.owl","file:shop#",Term(""),Term("minusC"),Term(""),Term(""),
	  DLQuery(Term(),Tuple()), ints);
      
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
  AtomPtr ap(new Atom("plusR(\"Part\",\"nic\",\"sic\")"));
  pr.insert(ap);
  Interpretation ints(pr);

  Query q("foo.owl","file:shop#",Term(""),Term(""),Term("plusR"),Term(""),
	  DLQuery(Term(),Tuple()), ints);

  RacerStateBuilder pib(sst);
  pib.buildCommand(q);
      
  std::string s = sst.str();

  CPPUNIT_ASSERT(s == "(state (related |file:shop#nic| |file:shop#sic| |file:shop#Part|))\n");
}
