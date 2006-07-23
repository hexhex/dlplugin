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

  NRQLRetrieveUnderPremise nrql;

  nrql.addHead(new ABoxQueryVariable("X"));
  nrql.addHead(new ABoxQueryVariable("Y"));

  std::auto_ptr<NRQLConjunction> c(new NRQLConjunction);
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

  NRQLUnion::shared_pointer u(new NRQLUnion);
  u->addAtom(new NRQLQueryAtom
	     (new NegationQuery
	      (new ConceptQuery
	       (new ABoxQueryConcept("fu"),
		new ABoxQueryVariable("X")
		)
	       )
	      )
	     );

  u->addAtom(c.release());
  nrql.setBody(u);

  sst << nrql;
     
  std::string s = sst.str();

  CPPUNIT_ASSERT(s == "(retrieve-under-premise () (?X ?Y) (union (not (?X fu)) (and (?X foo) (?X ?Y moo))))");
}
    
void
TestRacerNRQL::runRacerTBoxRetrieveTest()
{

}


void
TestRacerNRQL::runRacerPremiseRetrieveTest()
{

}
