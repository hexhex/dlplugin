/**
 * @file   TestRacerDirector.cpp
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:13:26 2006
 * 
 * @brief  
 * 
 * 
 */

#include "TestRacerDirector.h"

using namespace dlvhex::racer;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestRacerDirector);


void
TestRacerDirector::output(const std::vector<Tuple>& tv)
{
  std::cout << std::endl;
  for(std::vector<Tuple>::const_iterator it = tv.begin();
      it != tv.end();
      it++)
    {
      Tuple t = *it;
      std::cout << "(";
      // copy all elements of t to cout
      std::copy(t.begin(), t.end(),
		std::ostream_iterator<Term>(std::cout, ") ("));
      std::cout << ")" << std::endl;
    }
}

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
  
  RacerBaseDirector::QueryCtxPtr q(new QueryCtx);
  q->getQuery().setNamespace("http://www.kr.tuwien.ac.at/staff/roman/shop#");
  
  AtomSet pc;
  pc.insert(Atom("plusC(\"Part\",\"foo\")"));
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
  
  RacerBaseDirector::QueryCtxPtr q(new QueryCtx);
  
  CPPUNIT_ASSERT_NO_THROW( q = aiq.query(q) );
  CPPUNIT_ASSERT(q->getAnswer().getTuples()->size() > 0);
  
  const std::vector<Tuple> *tv = q->getAnswer().getTuples();
  output(*tv);
}
