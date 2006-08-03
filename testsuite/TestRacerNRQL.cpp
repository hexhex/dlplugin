/**
 * @file   TestRacerNRQL.cpp
 * @author Thomas Krennwallner
 * @date   Thu Apr 13 14:38:37 2006
 * 
 * @brief  
 * 
 * 
 */

#include "RacerNRQL.h"
#include "Query.h"
#include "TestRacerNRQL.h"

#include <sstream>
#include <iosfwd>
#include <string>


using namespace dlvhex::racer;

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

  Query q("","",Term(""),Term(""),Term(""),Term(""),DLQuery(as,tup),Interpretation());

  NRQLRetrieve<NRQLConjunctionBuilder> nrql(q);

  sst << nrql;
     
  std::string s = sst.str();
 
  std::cout << s << std::endl;

  CPPUNIT_ASSERT(s == "(retrieve ($?X $?Y) (and ($?X $?Y moo) ($?X foo)))");
}
    
void
TestRacerNRQL::runRacerTBoxRetrieveTest()
{

}


void
TestRacerNRQL::runRacerPremiseRetrieveTest()
{

}
