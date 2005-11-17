// -*- C++ -*-

#ifndef _TESTRACERSTREAM_H
#define _TESTRACERSTREAM_H

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "TCPStream.h"
#include "RacerRunner.h"

namespace dlvhex {
namespace racer {

  /**
   * @brief TestCases for TCPIOStream and TCPStreamBuf
   *
   * @test Sends the (all-individual) command to RACER with help of
   * TCPIOStream and TCPStreamBuf and checks whether we receive an
   * answer.
   */
  class TestRacerStream : public CppUnit::TestCase
  { 
  public:
    virtual void setUp()
    {
      RacerRunner::instance()->run();
    }

    void runRacerStreamBufTest()
    {
      TCPStreamBuf rsb("localhost", 8088, 1);
      
      char b[] = "(all-individuals)\n";
      int n = rsb.sputn(b, sizeof b);
      
      CPPUNIT_ASSERT(n == sizeof b);
      
      n = rsb.pubsync();
      
      CPPUNIT_ASSERT(n == 0);
      
      char c;
      do
	{
	  c = rsb.snextc();
	}
      while (c != '\n');
    }
    
    void runRacerIOStreamTest()
    {
      TCPIOStream rsIO("localhost", 8088);
      
      rsIO << "(all-individuals)" << std::endl;
      
      CPPUNIT_ASSERT(! rsIO.fail());
      
      char answer[BUFSIZ];
      rsIO.getline(answer, BUFSIZ);
      
      std::string answ(answer);
      
      CPPUNIT_ASSERT(answ.find("answer") != std::string::npos);
    }
    
    static CppUnit::Test *suite()
    {
      CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestRacerStream");
      
      suiteOfTests->addTest(new CppUnit::TestCaller<TestRacerStream>
			    ( 
			     "TCPIOStream", 
			     &TestRacerStream::runRacerIOStreamTest
			     )
			    );
      
      suiteOfTests->addTest(new CppUnit::TestCaller<TestRacerStream>
			    ( 
			     "TCPStreamBuf", 
			     &TestRacerStream::runRacerStreamBufTest
			     )
			    );
      
      return suiteOfTests;
    }
    
  };

} // namespace racer
} // namespace dlvhex

#endif /* _TESTRACERSTREAM_H */
