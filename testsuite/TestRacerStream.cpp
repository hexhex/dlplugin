/* dlvhex-dlplugin -- Integration of Answer-Set Programming and Description Logics.
 *
 * Copyright (C) 2005, 2006, 2007  Thomas Krennwallner
 * 
 * This file is part of dlvhex-dlplugin.
 *
 * dlvhex-dlplugin is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * dlvhex-dlplugin is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dlvhex-dlplugin; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


/**
 * @file   TestRacerStream.cpp
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:37:32 2006
 * 
 * @brief  TestCases for TCPIOStream and TCPStreamBuf
 * 
 * 
 */

#include "TCPStream.h"

#include "TestRacerStream.h"

#include <iosfwd>

using namespace dlvhex::util;
using namespace dlvhex::dl::test;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestRacerStream);


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


// Local Variables:
// mode: C++
// End:
