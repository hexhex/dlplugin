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
 * @file   TestRacerNRQL.cpp
 * @author Thomas Krennwallner
 * @date   Thu Apr 13 14:38:37 2006
 * 
 * @brief  TestCases for Racer's nRQL.
 * 
 * 
 */

#include "TestRacerNRQL.h"

#include "KBManager.h"
#include "RacerNRQL.h"
#include "Query.h"

#include <sstream>
#include <iosfwd>
#include <string>


using namespace dlvhex::dl;
using namespace dlvhex::dl::racer;
using namespace dlvhex::dl::test;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestRacerNRQL);

void
TestRacerNRQL::runRacerRetrieveTest()
{
  std::stringstream sst;

  Tuple tup;
  tup.push_back(Term("X"));
  tup.push_back(Term("Y"));

  AtomSet as;
  AtomPtr ap1(new Atom("foo(X)"));
  AtomPtr ap2(new Atom("moo(X,Y)"));
  as.insert(ap1);
  as.insert(ap2);

  KBManager kb("DEFAULT");
  DLQuery::shared_pointer dlq(new DLQuery(Ontology::createOntology(test),as,tup));
  Query q(kb,dlq,Term(""),Term(""),Term(""),Term(""),AtomSet());

  NRQLRetrieve<NRQLConjunctionBuilder> nrql(q);

  sst << nrql;
     
  std::string s = sst.str();
 
  std::cout << s << std::endl;

  CPPUNIT_ASSERT(s == "(retrieve ($?X $?Y) (and ($?X $?Y |http://www.test.com/test#moo|) ($?X |http://www.test.com/test#foo|)) :abox DEFAULT)");
}
    
void
TestRacerNRQL::runRacerTBoxRetrieveTest()
{

}


void
TestRacerNRQL::runRacerPremiseRetrieveTest()
{
  std::stringstream sst;

  Tuple tup;
  tup.push_back(Term("X"));
  tup.push_back(Term("Y"));

  AtomSet as;
  AtomPtr ap1(new Atom("foo(X)"));
  AtomPtr ap2(new Atom("moo(X,Y)"));
  as.insert(ap1);
  as.insert(ap2);

  AtomSet ints;
  AtomPtr ap4(new Atom("pc(foo,a)"));
  AtomPtr ap5(new Atom("pr(moo,a,b)"));
  ints.insert(ap4);
  ints.insert(ap5);

  KBManager kb("DEFAULT");
  DLQuery::shared_pointer dlq(new DLQuery(Ontology::createOntology(test),as,tup));
  Query q(kb,dlq,Term("pc"),Term(""),Term("pr"),Term(""),ints);

  NRQLRetrieveUnderPremise<NRQLConjunctionBuilder> nrql(q);

  sst << nrql;
     
  std::string s = sst.str();
 
  std::cout << s << std::endl;

  URI u(testuri, true); // absolute pathname
  CPPUNIT_ASSERT(s == "(retrieve-under-premise ((related |http://www.test.com/test#a| |http://www.test.com/test#b| |http://www.test.com/test#moo|) (instance |http://www.test.com/test#a| |http://www.test.com/test#foo|)) ($?X $?Y) (and ($?X $?Y |http://www.test.com/test#moo|) ($?X |http://www.test.com/test#foo|)) :abox |" + u.getString() + "|)");
}


// Local Variables:
// mode: C++
// End:
