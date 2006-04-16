/**
 * @file   TestRacerNRQL.cpp
 * @author Thomas Krennwallner
 * @date   Thu Apr 13 14:38:37 2006
 * 
 * @brief  
 * 
 * 
 */

#include "TestRacerNRQL.h"

using namespace dlvhex::racer;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestRacerNRQL);

void
TestRacerNRQL::runRacerRetrieveTest()
{
  std::stringstream sst;

  NRQLRetrieveUnderPremise nrql;

  nrql.addHead(new ABoxQueryVariable("X"));
  nrql.addHead(new ABoxQueryVariable("Y"));

  NRQLConjunction* c = new NRQLConjunction;
  c->addAtom(new NRQLQueryAtom
	     (new ConceptQuery
	      (new ABoxQueryConcept("foo"),
	       new ABoxQueryVariable("X")
	       )
	      )
	     );
  c->addAtom(new NRQLQueryAtom
	     (new RoleQuery
	      (new ABoxQueryRole("moo"),
	       new ABoxQueryVariable("X"),
	       new ABoxQueryVariable("Y")
	       )
	      )
	     );

  NRQLUnion* u = new NRQLUnion;
  u->addAtom(new NRQLQueryAtom
	     (new NegationQuery
	      (new ConceptQuery
	       (new ABoxQueryConcept("fu"),
		new ABoxQueryVariable("X")
		)
	       )
	      )
	     );

  u->addAtom(c);
  nrql.addBody(u);

  sst << nrql;
     
  std::string s = sst.str();

//   CPPUNIT_ASSERT(s == "(state (instance |file:shop#nic| |file:shop#Part|))\n");

  std::cout << "### " << s << std::endl;
}
    
void
TestRacerNRQL::runRacerTBoxRetrieveTest()
{
}


void
TestRacerNRQL::runRacerPremiseRetrieveTest()
{

}
