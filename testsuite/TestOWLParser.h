// -*- C++ -*-

#ifndef _TESTOWLPARSER_H
#define _TESTOWLPARSER_H

#include <iostream>
#include <string>
#include <functional>
#include <iterator>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "OWLParser.h"


namespace dlvhex {
namespace racer {

  /**
   * @brief TestCases for OWLParser
   *
   * @test Tests parsing an OWL document.
   */
  class TestOWLParser : public CppUnit::TestCase
  {
  private:
    std::string shop;

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
      const char* ex = getenv("EXAMPLES");
      if (ex)
	{
	  shop = std::string(ex) + std::string("/shop.owl");
	}
      else
	{
	  shop = "shop.owl";
	}
    }

    void runParserTest()
    {
      Query q;
      Answer a;
      OWLParser p("file:" + shop);
      p.parseIndividuals(a);
      output(*a.getTuples());
#if 0
      p.parseNamespace(q);
      CPPUNIT_ASSERT(q.getNamespace() == "http://www.kr.tuwien.ac.at/staff/roman/shop#");
#endif
    }
    
    static CppUnit::Test *suite()
    {
      CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestOWLParser");
      
      suiteOfTests->addTest(new CppUnit::TestCaller<TestOWLParser>
			    ( 
			     "OWLParser", 
			     &TestOWLParser::runParserTest
			     )
			    );
      return suiteOfTests;
    }
    
  };

} // namespace racer
} // namespace dlvhex


#endif /* _TESTRACERDIRECTOR_H */
