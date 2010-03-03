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
#include "Answer.h"

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
TestRacerParse::runRacerErrorTest()
{
  std::istringstream ss(":error 1 Illegal syntax: help \"foo\"\n");

  RacerAnswerDriver sa(ss);
  Answer a(0);

  CPPUNIT_ASSERT_THROW( sa.parse(a), dlvhex::dl::DLParsingError );
}
    
void
TestRacerParse::runRacerAnswerListTest()
{
  // 2 individuals
  {
    std::istringstream ss(":answer 1 \"(|file://foobar#myfoo1| |file://foobar#myfoo2|)\" \"\"\n");
    
    RacerAnswerDriver al(ss);
    Answer a(0);

    CPPUNIT_ASSERT_NO_THROW( al.parse(a) );
    
    std::vector<dlvhex::Tuple> &at = *a.getTuples();
    CPPUNIT_ASSERT(at.size() == 2);
    CPPUNIT_ASSERT(at[0][0].getUnquotedString() == std::string("<file://foobar#myfoo1>"));
    CPPUNIT_ASSERT(at[1][0].getUnquotedString() == std::string("<file://foobar#myfoo2>"));
  }

  // 1 pair
  {
    std::istringstream ss(":answer 1 \"((|file://foobar#myfoo1| |file://foobar#myfoo2|))\" \"\"\n");
    
    RacerAnswerDriver al(ss);
    Answer a(0);

    CPPUNIT_ASSERT_NO_THROW( al.parse(a) );
    
    std::vector<dlvhex::Tuple> &at = *a.getTuples();
    CPPUNIT_ASSERT(at.size() == 1);
    CPPUNIT_ASSERT(at[0].size() == 2);
    CPPUNIT_ASSERT(at[0][0].getUnquotedString() == std::string("<file://foobar#myfoo1>"));
    CPPUNIT_ASSERT(at[0][1].getUnquotedString() == std::string("<file://foobar#myfoo2>"));
  }

  // 2 pairs
  {
    std::istringstream ss(":answer 1 \"( (|file://foobar#myfoo1| |file://foobar#myfoo2|) (|file://foobar#myfoo3| |file://foobar#myfoo4|) )\" \"\"\n");
    
    RacerAnswerDriver al(ss);
    Answer a(0);

    CPPUNIT_ASSERT_NO_THROW( al.parse(a) );
    
    std::vector<dlvhex::Tuple> &at = *a.getTuples();
    CPPUNIT_ASSERT(at.size() == 2);
    CPPUNIT_ASSERT(at[0].size() == 2);
    CPPUNIT_ASSERT(at[0][0].getUnquotedString() == std::string("<file://foobar#myfoo1>"));
    CPPUNIT_ASSERT(at[0][1].getUnquotedString() == std::string("<file://foobar#myfoo2>"));
    CPPUNIT_ASSERT(at[1].size() == 2);
    CPPUNIT_ASSERT(at[1][0].getUnquotedString() == std::string("<file://foobar#myfoo3>"));
    CPPUNIT_ASSERT(at[1][1].getUnquotedString() == std::string("<file://foobar#myfoo4>"));
  }

  // 1 tuple with 1 variable
  {
    std::istringstream ss(":answer 4 \"(((* VAR |file://foobar#myfoo1|)))\" \"\"\n");
    
    RacerAnswerDriver al(ss);
    Answer a(0);

    CPPUNIT_ASSERT_NO_THROW( al.parse(a) );
    
    std::vector<dlvhex::Tuple> &at = *a.getTuples();
    CPPUNIT_ASSERT(at.size() == 1);
    CPPUNIT_ASSERT(at[0][0].getUnquotedString() == std::string("<file://foobar#myfoo1>"));
  }

  // 1 tuple with 3 variables
  {
    std::istringstream ss(":answer 3 \"(((* foo |file://foobar#myfoo1|) ($? Bar |file://foobar#myfoo2|) ($?* X |file://foobar#myfoo3|)))\" \"\"\n");
    
    RacerAnswerDriver al(ss);
    Answer a(0);

    CPPUNIT_ASSERT_NO_THROW( al.parse(a) );
    
    std::vector<dlvhex::Tuple> &at = *a.getTuples();
    CPPUNIT_ASSERT(at.size() == 1);
    CPPUNIT_ASSERT(at[0].size() == 3);
    CPPUNIT_ASSERT(at[0][0].getUnquotedString() == std::string("<file://foobar#myfoo1>"));
    CPPUNIT_ASSERT(at[0][1].getUnquotedString() == std::string("<file://foobar#myfoo2>"));
    CPPUNIT_ASSERT(at[0][2].getUnquotedString() == std::string("<file://foobar#myfoo3>"));
  }

  // 3 tuple with 1 variable
  {
    std::istringstream ss(":answer 3 \"(((* foo |file://foobar#myfoo1|)) (($? Bar |file://foobar#myfoo2|)) (($?* X |file://foobar#myfoo3|)))\" \"\"\n");
    
    RacerAnswerDriver al(ss);
    Answer a(0);

    CPPUNIT_ASSERT_NO_THROW( al.parse(a) );
    
    std::vector<dlvhex::Tuple> &at = *a.getTuples();
    CPPUNIT_ASSERT(at.size() == 3);
    CPPUNIT_ASSERT(at[0].size() == 1);
    CPPUNIT_ASSERT(at[0][0].getUnquotedString() == std::string("<file://foobar#myfoo1>"));
    CPPUNIT_ASSERT(at[1].size() == 1);
    CPPUNIT_ASSERT(at[1][0].getUnquotedString() == std::string("<file://foobar#myfoo2>"));
    CPPUNIT_ASSERT(at[2].size() == 1);
    CPPUNIT_ASSERT(at[2][0].getUnquotedString() == std::string("<file://foobar#myfoo3>"));
  }
}


// Local Variables:
// mode: C++
// End:
