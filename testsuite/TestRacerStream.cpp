/**
 * @file   TestRacerStream.cpp
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:37:32 2006
 * 
 * @brief  
 * 
 * 
 */

#include "TestRacerStream.h"

using namespace dlvhex::racer;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestRacerStream);


void
TestRacerStream::setUp()
{
  RacerRunner::instance()->run();
}

void
TestRacerStream::runRacerStreamBufTest()
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

void
TestRacerStream::runRacerIOStreamTest()
{
  TCPIOStream rsIO("localhost", 8088);
  
  rsIO << "(all-individuals)" << std::endl;
  
  CPPUNIT_ASSERT(! rsIO.fail());
  
  char answer[BUFSIZ];
  rsIO.getline(answer, BUFSIZ);
  
  std::string answ(answer);
  
  CPPUNIT_ASSERT(answ.find("answer") != std::string::npos);
}
