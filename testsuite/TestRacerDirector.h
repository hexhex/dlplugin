// -*- C++ -*-

#ifndef _TESTRACERDIRECTOR_H
#define _TESTRACERDIRECTOR_H

#include <iostream>
#include <string>
#include <functional>
#include <iterator>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "RacerDirector.h"
#include "RacerRunner.h"

namespace dlvhex {
namespace racer {

  /**
   * @brief TestCases for RacerDirector.
   *
   * @test Opens a connection to RACER with TCPIOStream, sends some
   * commands and performs some sanity checks on the result.
   */
  class TestRacerDirector : public CppUnit::TestCase
  {
  protected:
    void output(const std::vector<Tuple>& tv)
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

  public:
    virtual void setUp()
    {
      RacerRunner::instance()->run();
    }

    void runRacerPlusConceptTest()
    {
      TCPIOStream rsIO;
      rsIO.open("localhost", 8088);
      
      RacerConceptRolePM pcd(rsIO);

      RacerBaseDirector::QueryCtxPtr q(new QueryCtx);
      q->getQuery().setNamespace("http://www.kr.tuwien.ac.at/staff/roman/shop#");

      GAtomSet pc;
      pc.insert(GAtom("plusC(\"Part\",\"foo\")"));
      q->getQuery().setPlusConcept(pc);

      CPPUNIT_ASSERT_NO_THROW( q = pcd.query(q) );
      rsIO.close();
    }
    
    void runRacerAllIndividualsTest()
    {
      TCPIOStream rsIO;
      rsIO.open("localhost", 8088);

      RacerAllIndQuery aiq(rsIO);

      RacerBaseDirector::QueryCtxPtr q(new QueryCtx);
      
      CPPUNIT_ASSERT_NO_THROW( q = aiq.query(q) );
      CPPUNIT_ASSERT(q->getAnswer().getTuples().size() > 0);
      
      rsIO.close();

      std::vector<Tuple> tv = q->getAnswer().getTuples();
      output(tv);
    }
    
    static CppUnit::Test *suite()
    {
      CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestRacerDirector");
      
      suiteOfTests->addTest(new CppUnit::TestCaller<TestRacerDirector>
			    ( 
			     "RacerPlusConcept", 
			     &TestRacerDirector::runRacerPlusConceptTest
			     )
			    );
      
      suiteOfTests->addTest(new CppUnit::TestCaller<TestRacerDirector>
			    ( 
			     "RacerAllIndividuals", 
			     &TestRacerDirector::runRacerAllIndividualsTest
			     )
			    );
      
      return suiteOfTests;
    }
    
  };

} // namespace racer
} // namespace dlvhex


#endif /* _TESTRACERDIRECTOR_H */
