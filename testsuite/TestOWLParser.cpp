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
TestOWLParser::output(const std::vector<Tuple>& tv)
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
  Query q;
  Answer a;
  OWLParser p(shop);
  p.parseIndividuals(a);
  output(*a.getTuples());
  p.parseNamespace(q);
  CPPUNIT_ASSERT(q.getNamespace() == "http://www.kr.tuwien.ac.at/staff/roman/shop#");

  OWLParser p2(test);
  a.setTuples(std::vector<Tuple>());
  p2.parseIndividuals(a);
  output(*a.getTuples());
  p2.parseNamespace(q);
  CPPUNIT_ASSERT(q.getNamespace() == "http://www.test.com/test#");
}
