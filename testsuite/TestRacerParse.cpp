/**
 * @file   TestRacerParse.cpp
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:27:06 2006
 * 
 * @brief  TestCases for RacerAnswer Parser.
 * 
 * 
 */

#include "RacerAnswerDriver.h"

#include "TestRacerParse.h"

#include <sstream>
#include <iosfwd>
#include <string>

using namespace dlvhex::dl;
using namespace dlvhex::dl::racer;
using namespace dlvhex::dl::test;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestRacerParse);

void
TestRacerParse::runRacerSimpleAnswerTest()
{
  std::istringstream ss(":answer 1 \"NIL\" \"\"\n");

  RacerAnswerDriver sa(ss);
  Answer a(0);

  CPPUNIT_ASSERT_NO_THROW( sa.parse(a) );
}
    
void
TestRacerParse::runRacerAnswerListTest()
{
  std::istringstream ss(":answer 1 \"(|file://foobar#myfoo1| |file://foobar#myfoo2|)\" \"\"\n");
  
  RacerAnswerDriver al(ss);
  Answer a(0);

  CPPUNIT_ASSERT_NO_THROW( al.parse(a) );
  
  CPPUNIT_ASSERT((*a.getTuples())[0][0].getUnquotedString() == std::string("file://foobar#myfoo1"));
  CPPUNIT_ASSERT((*a.getTuples())[1][0].getUnquotedString() == std::string("file://foobar#myfoo2"));
}
