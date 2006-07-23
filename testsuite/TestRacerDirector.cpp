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
  
  QueryCtx::shared_pointer q(new QueryCtx);
  q->getQuery().setNamespace("http://www.kr.tuwien.ac.at/staff/roman/shop#");
  
  AtomSet pc;
  AtomPtr ap(new Atom("plusC(\"Part\",\"foo\")"));
  pc.insert(ap);
  Interpretation ints(pc);
  
  q->getQuery().setInterpretation(ints);
  q->getQuery().setPlusC(Term("plusC"));
  
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
