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
      
  AtomSet pc;
  AtomPtr ap(new Atom("plusC(\"Part\",\"nic\")"));
  pc.insert(ap);
  Interpretation ints(pc);

  Query q;

  q.setQuery(Term("q"));
  q.setPatternTuple(Tuple(1, Term("X")));
  q.setInterpretation(ints);
  q.setPlusC(Term("plusC"));
  q.setNamespace("file:shop#");
  
  RacerNRQLBasePtr nrql
    (RacerNRQLBasePtr
     (new RacerNRQLBody
      (RacerNRQLBasePtr
       (new RacerNRQLHead
	(RacerNRQLBasePtr
	 (new RacerNRQLRetrieve(sst)
	  )
	 )
	)
       )
      )
     );
  

  nrql->retrieve(q);
      
  std::string s = sst.str();

//   CPPUNIT_ASSERT(s == "(state (instance |file:shop#nic| |file:shop#Part|))\n");

  std::cout << "### " << s << std::endl;
}
    
void
TestRacerNRQL::runRacerTBoxRetrieveTest()
{
  std::stringstream sst;
      
  AtomSet pc;
  AtomPtr ap(new Atom("plusC(\"Part\",\"nic\")"));
  pc.insert(ap);
  Interpretation ints(pc);

  Query q;

  q.setQuery(Term("q"));
  q.setPatternTuple(Tuple(1, Term("X")));
  q.setInterpretation(ints);
  q.setPlusC(Term("plusC"));
  q.setNamespace("file:shop#");
  
  RacerNRQLBasePtr nrql
    (RacerNRQLBasePtr
     (new RacerNRQLBody
      (RacerNRQLBasePtr
       (new RacerNRQLHead
	(RacerNRQLBasePtr
	 (new RacerNRQLTBoxRetrieve(sst)
	  )
	 )
	)
       )
      )
     );
  

  nrql->retrieve(q);
      
  std::string s = sst.str();

//   CPPUNIT_ASSERT(s == "(state (instance |file:shop#nic| |file:shop#Part|))\n");

  std::cout << "### " << s << std::endl;
}


void
TestRacerNRQL::runRacerPremiseRetrieveTest()
{
  std::stringstream sst;
      
  AtomSet pc;
  AtomPtr ap(new Atom("plusC(\"Part\",\"nic\")"));
  pc.insert(ap);
  Interpretation ints(pc);

  Query q;

  q.setQuery(Term("q"));
  q.setPatternTuple(Tuple(4, Term("X")));
  q.setInterpretation(ints);
  q.setPlusC(Term("plusC"));
  q.setNamespace("file:shop#");
  
  RacerNRQLBasePtr nrql
    (RacerNRQLBasePtr
     (new RacerNRQLBody
      (RacerNRQLBasePtr
       (new RacerNRQLHead
	(RacerNRQLBasePtr
	 (new RacerNRQLPremise
	  (RacerNRQLBasePtr
	   (new RacerNRQLRetrieve(sst)
	    )
	   )
	  )
	 )
	)
       )
      )
     );
  

  nrql->retrieve(q);
      
  std::string s = sst.str();

//   CPPUNIT_ASSERT(s == "(state (instance |file:shop#nic| |file:shop#Part|))\n");

  std::cout << "### " << s << std::endl;
}
