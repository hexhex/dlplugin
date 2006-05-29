/**
 * @file   TestOWLParser.cpp
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:02:29 2006
 * 
 * @brief  
 * 
 * 
 */

#include "TestOWLParser.h"

using namespace dlvhex::racer;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestOWLParser);


void
TestOWLParser::setUp()
{
  const char* ex = getenv("EXAMPLES");
  if (ex)
    {
      shop = std::string(ex) + std::string("/shop.owl");
      test = std::string(ex) + std::string("/test.owl");
    }
  else
    {
      shop = "shop.owl";
      test = "test.owl";
    }
}

void
TestOWLParser::runParserTest()
{
  OWLParser p(shop);
  std::string defaultNS;
  std::set<Term> indvs;
  p.parseIndividuals(indvs);

  std::cout << "(";
  std::copy(indvs.begin(), indvs.end(),
	    std::ostream_iterator<Term>(std::cout, ") ("));
  std::cout << ")" << std::endl;

  p.parseNamespace(defaultNS);
  CPPUNIT_ASSERT(defaultNS == "http://www.kr.tuwien.ac.at/staff/roman/shop#");

  OWLParser p2(test);
  indvs.clear();
  defaultNS.clear();
  p2.parseIndividuals(indvs);

  std::cout << "(";
  std::copy(indvs.begin(), indvs.end(),
	    std::ostream_iterator<Term>(std::cout, ") ("));
  std::cout << ")" << std::endl;

  p2.parseNamespace(defaultNS);
  CPPUNIT_ASSERT(defaultNS == "http://www.test.com/test#");
}
