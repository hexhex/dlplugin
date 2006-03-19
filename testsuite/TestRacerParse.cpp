/**
 * @file   TestRacerParse.cpp
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:27:06 2006
 * 
 * @brief  
 * 
 * 
 */

#include "TestRacerParse.h"

using namespace dlvhex::racer;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestRacerParse);

void
TestRacerParse::runRacerSimpleAnswerTest()
{
  std::istringstream ss(":answer 1 \"NIL\" \"\"\n");

  RacerParse sa(ss);
  Answer a;

  CPPUNIT_ASSERT_NO_THROW( sa.parse(a) );
}
    
void
TestRacerParse::runRacerAnswerListTest()
{
  std::istringstream ss(":answer 1 \"(|file:foobar#myfoo1| |file:foobar#myfoo2|)\" \"\"\n");
  
  RacerParse al(ss);
  Answer a;

  CPPUNIT_ASSERT_NO_THROW( al.parse(a) );
  
  CPPUNIT_ASSERT((*a.getTuples())[0][0].getUnquotedString() == std::string("myfoo1"));
  CPPUNIT_ASSERT((*a.getTuples())[1][0].getUnquotedString() == std::string("myfoo2"));
}
