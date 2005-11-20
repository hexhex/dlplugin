// -*- C++ -*-

#ifndef _TESTRACERPARSE_H
#define _TESTRACERPARSE_H

#include <sstream>
#include <iostream>
#include <string>
#include <exception>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "RacerParse.h"

namespace dlvhex {
namespace racer {

  /**
   * @brief TestCases for RacerParse and its children.
   *
   * @test Tests the RacerSimpleAnswer and RacerAnswerList parsers and
   * performs some sanity checks on the result.
   */
  class TestRacerParse : public CppUnit::TestCase
  { 
  public: 
    void runRacerSimpleAnswerTest()
    {
      std::stringstream ss;
      ss << ":answer 1 \"NIL\" \"\"";

      RacerSimpleAnswer sa(ss);
      Answer a;

      CPPUNIT_ASSERT_NO_THROW( sa.parse(a) );
    }
    
    void runRacerAnswerListTest()
    {
      std::stringstream ss;
      ss << ":answer 1 \"(|file:foobar#myfoo1| |file:foobar#myfoo2|)\" \"\"";

      RacerAnswerList al(ss);
      Answer a;

      CPPUNIT_ASSERT_NO_THROW( al.parse(a) );

      CPPUNIT_ASSERT((*a.getTuples())[0][0].getUnquotedString() == std::string("myfoo1"));
      CPPUNIT_ASSERT((*a.getTuples())[1][0].getUnquotedString() == std::string("myfoo2"));
    }
    
    static CppUnit::Test *suite()
    {
      CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestRacerParse");
      
      suiteOfTests->addTest(new CppUnit::TestCaller<TestRacerParse>
			    ( 
			     "RacerSimpleAnswer", 
			     &TestRacerParse::runRacerSimpleAnswerTest
			     )
			    );
      
      suiteOfTests->addTest(new CppUnit::TestCaller<TestRacerParse>
			    ( 
			     "RacerAnswerList", 
			     &TestRacerParse::runRacerAnswerListTest
			     )
			    );
      
      return suiteOfTests;
    }
    
  };

} // namespace racer
} // namespace dlvhex


#endif /* _TESTRACERPARSE_H */
