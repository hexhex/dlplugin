/**
 * @file   TestOWLParser.cpp
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:02:29 2006
 * 
 * @brief  Tests parsing an OWL document.
 * 
 * 
 */

#include "OWLParser.h"

#include "TestOWLParser.h"

#include <iosfwd>
#include <string>
#include <functional>
#include <iterator>

using namespace dlvhex::dl::test;
using dlvhex::dl::OWLParser;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestOWLParser);

void
TestOWLParser::runParserTest()
{
  OWLParser p(shop);
  std::string defaultNS;
  ABox abox;
  p.parseABox(abox);

  if (!abox.getIndividuals()->empty())
    {
      std::cout << "{";
      std::copy(abox.getIndividuals()->begin(),
		--abox.getIndividuals()->end(),
		std::ostream_iterator<Term>(std::cout, ",")
		);
      std::cout << *(--abox.getIndividuals()->end()) << "}" << std::endl;
    }

  p.parseNamespace(defaultNS);
  CPPUNIT_ASSERT(defaultNS == "http://www.kr.tuwien.ac.at/staff/roman/shop#");

  OWLParser p2(test);
  ABox abox2;
  defaultNS.clear();
  p2.parseABox(abox2);

  if (!abox2.getIndividuals()->empty())
    {
      std::cout << "{";
      std::copy(abox2.getIndividuals()->begin(),
		--abox2.getIndividuals()->end(),
		std::ostream_iterator<Term>(std::cout, ",")
		);
      std::cout << *(--abox2.getIndividuals()->end()) << "}" << std::endl;
    }

  p2.parseNamespace(defaultNS);
  CPPUNIT_ASSERT(defaultNS == "http://www.test.com/test#");
}
