/**
 * @file   TestRacerNRQL.cpp
 * @author Thomas Krennwallner
 * @date   Thu Apr 13 14:38:37 2006
 * 
 * @brief  TestCases for Racer's nRQL.
 * 
 * 
 */

#include "RacerNRQL.h"
#include "Query.h"
#include "TestRacerNRQL.h"

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

  Query q(Ontology::createOntology(test),Term(""),Term(""),Term(""),Term(""),DLQuery(as,tup),AtomSet());

  NRQLRetrieve<NRQLConjunctionBuilder> nrql(q);

  sst << nrql;
     
  std::string s = sst.str();
 
  std::cout << s << std::endl;

  CPPUNIT_ASSERT(s == "(retrieve ($?X $?Y) (and ($?X $?Y |http://www.test.com/test#moo|) ($?X |http://www.test.com/test#foo|)))");
}
    
void
TestRacerNRQL::runRacerTBoxRetrieveTest()
{

}


void
TestRacerNRQL::runRacerPremiseRetrieveTest()
{

}
