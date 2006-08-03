/**
 * @file   TestRacerDirector.cpp
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:13:26 2006
 * 
 * @brief  
 * 
 * 
 */

#include "RacerDirector.h"
#include "RacerRunner.h"
#include "TCPStream.h"
#include "QueryCtx.h"
#include "TestRacerDirector.h"

#include <iostream>
#include <string>
#include <functional>
#include <iterator>


using namespace dlvhex::racer;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestRacerDirector);

void
TestRacerDirector::setUp()
{
  RacerRunner::instance()->run();
}

void
TestRacerDirector::runRacerPlusConceptTest()
{
  TCPIOStream rsIO("localhost", 8088);
      
  RacerConceptRolePM pcd(rsIO);
  
  AtomSet pc;
  AtomPtr ap(new Atom("plusC(\"Part\",\"foo\")"));
  pc.insert(ap);
  Interpretation ints(pc);
  
  Tuple in;
  in.push_back(Term(shop));
  in.push_back(Term("plusC"));
  in.push_back(Term(""));
  in.push_back(Term(""));
  in.push_back(Term(""));
  in.push_back(Term(""));

  Tuple out;

  QueryCtx::shared_pointer q(new QueryCtx(PluginAtom::Query(ints, in, out)));
  
  CPPUNIT_ASSERT_NO_THROW( q = pcd.query(q) );
}
    
void
TestRacerDirector::runRacerAllIndividualsTest()
{
  TCPIOStream rsIO("localhost", 8088);
  
  RacerAllIndQuery aiq(rsIO);
  
  QueryCtx::shared_pointer q(new QueryCtx);
  
  CPPUNIT_ASSERT_NO_THROW( q = aiq.query(q) );
  CPPUNIT_ASSERT(q->getAnswer().getTuples()->size() > 0);
  
  const std::vector<Tuple> *tv = q->getAnswer().getTuples();
  output(*tv);
}
