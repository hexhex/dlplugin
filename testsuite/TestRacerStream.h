// -*- C++ -*-

#ifndef _TESTRACERSTREAM_H
#define _TESTRACERSTREAM_H

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "TCPStream.h"

using namespace std;

namespace dlvex_racer {

  /**
   * @brief TestCases for TCPIOStream and TCPStreamBuf
   *
   * @test Sends the (all-individual) command to RACER with help of
   * TCPIOStream and TCPStreamBuf and checks whether we receive an
   * answer.
   *
   * @attention RACER must be running.
   */
  class TestRacerStream : public CppUnit::TestCase
  { 
  public: 
    void runRacerStreamBufTest()
    {
      ACE_SOCK_Stream rs;
      ACE_INET_Addr addr(8088, "localhost");
      ACE_SOCK_Connector conn;
      conn.connect(rs, addr);

      TCPStreamBuf rsb(rs, 1);
      
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
      
      rs.close();
    }
    
    void runRacerIOStreamTest()
    {
      TCPIOStream rsIO;
      rsIO.open("localhost", 8088);
      
      rsIO << "(all-individuals)" << endl;
      
      CPPUNIT_ASSERT(! rsIO.fail());
      
      char answer[BUFSIZ];
      rsIO.getline(answer, BUFSIZ);
      
      string answ(answer);
      
      CPPUNIT_ASSERT(answ.find("answer") != std::string::npos);
      
      rsIO.close();
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

} /* namespace dlvex_racer */

#endif /* _TESTRACERSTREAM_H */
