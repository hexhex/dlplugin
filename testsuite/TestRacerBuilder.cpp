/* dlvhex-dlplugin -- Integration of Answer-Set Programming and Description Logics.
 *
 * Copyright (C) 2005, 2006, 2007  Thomas Krennwallner
 * 
 * This file is part of dlvhex-dlplugin.
 *
 * dlvhex-dlplugin is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * dlvhex-dlplugin is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dlvhex-dlplugin; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


/**
 * @file   TestRacerBuilder.cpp
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:06:24 2006
 * 
 * @brief  Builds various RACER commands and checks if they are valid.
 * 
 * 
 */


#include "TestRacerBuilder.h"

#include "RacerBuilder.h"
#include "Query.h"
#include "RacerKBManager.h"

#include <sstream>
#include <iostream>
#include <string>


using namespace dlvhex::dl;
using namespace dlvhex::dl::racer;
using namespace dlvhex::dl::test;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestRacerBuilder);

void
TestRacerBuilder::runRacerPosIndBuilderTest()
{
  std::stringstream sst;
      
  AtomSet pc;
  AtomPtr ap(new Atom("plusC(\"Part\",\"nic\")"));
  pc.insert(ap);
  AtomSet ints(pc);
  RacerKBManager kb(sst, "DEFAULT");
  DLQuery::shared_pointer dlq(new DLQuery(Ontology::createOntology(shop),Term(),Tuple()));
  Query q(kb,dlq,Term("plusC"),Term(""),Term(""),Term(""),ints);

  RacerStateBuilder pib(sst);
  pib.buildCommand(q);
      
  std::string s = sst.str();

  CPPUNIT_ASSERT(s == "(state (add-concept-assertion DEFAULT |http://www.kr.tuwien.ac.at/staff/roman/shop#nic| |http://www.kr.tuwien.ac.at/staff/roman/shop#Part|))\n");
}
    
void
TestRacerBuilder::runRacerNegIndBuilderTest()
{
  std::stringstream sst;
      
  AtomSet mc;
  AtomPtr ap(new Atom("minusC(\"Part\",\"nic\")"));
  mc.insert(ap);
  AtomSet ints(mc);
  RacerKBManager kb(sst, "DEFAULT");
  DLQuery::shared_pointer dlq(new DLQuery(Ontology::createOntology(shop),Term(),Tuple()));
  Query q(kb,dlq,Term(""),Term("minusC"),Term(""),Term(""),ints);
	  
      
  RacerStateBuilder pib(sst);
  pib.buildCommand(q);
      
  std::string s = sst.str();

  CPPUNIT_ASSERT(s == "(state (add-concept-assertion DEFAULT |http://www.kr.tuwien.ac.at/staff/roman/shop#nic| (not |http://www.kr.tuwien.ac.at/staff/roman/shop#Part|)))\n");
}
    
void
TestRacerBuilder::runRacerPosPairBuilderTest()
{
  std::stringstream sst;

  AtomSet pr;
  AtomPtr ap(new Atom("plusR(\"Part\",\"nic\",\"sic\")"));
  pr.insert(ap);
  AtomSet ints(pr);
  RacerKBManager kb(sst, "DEFAULT");
  DLQuery::shared_pointer dlq(new DLQuery(Ontology::createOntology(shop),Term(),Tuple()));
  Query q(kb,dlq,Term(""),Term(""),Term("plusR"),Term(""),ints);

  RacerStateBuilder pib(sst);
  pib.buildCommand(q);
      
  std::string s = sst.str();

  CPPUNIT_ASSERT(s == "(state (add-role-assertion DEFAULT |http://www.kr.tuwien.ac.at/staff/roman/shop#nic| |http://www.kr.tuwien.ac.at/staff/roman/shop#sic| |http://www.kr.tuwien.ac.at/staff/roman/shop#Part|))\n");
}


// Local Variables:
// mode: C++
// End:
