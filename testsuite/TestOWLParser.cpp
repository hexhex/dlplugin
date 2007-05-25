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


// Local Variables:
// mode: C++
// End:
