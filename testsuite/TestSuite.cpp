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
 * @file   TestSuite.cpp
 * @author Thomas Krennwallner
 * @date   Sun Aug 13 15:28:13 2006
 * 
 * @brief  TestSuite base class.
 * 
 * 
 */

#include "TestSuite.h"

#include <cppunit/CompilerOutputter.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <iosfwd>
#include <string>
#include <functional>
#include <iterator>
#include <cstdlib>

using namespace dlvhex::dl::test;

TestSuite::TestSuite()
 : CppUnit::TestFixture()
{
  const char* ex = getenv("EXAMPLES");
  if (ex)
    {
      examples = std::string(ex);
      shop = examples + "/shop.owl";
      shopuri = "file:" + examples + "/shop.owl";
      
      test = examples + "/test.owl";
      testuri = "file:" + examples + "/test.owl";
    }
  else
    {
      examples = "";
      shop = std::string("./shop.owl");
      shopuri = std::string("file:shop.owl");
      test = std::string("./test.owl");
      testuri = std::string("file:test.owl");
    }
}

void
TestSuite::output(const std::vector<Tuple>& tv)
{
  for(std::vector<Tuple>::const_iterator it = tv.begin();
      it != tv.end();
      it++)
    {
      // copy all elements of it to std::cout
      if (!it->empty())
	{
	  std::cout << "(";
	  std::copy(it->begin(),
		    it->end() - 1,
		    std::ostream_iterator<Term>(std::cout, ",")
		    );
	  std::cout << it->back() << ")" << std::endl;
	}
    }
}

int main(int /* argc */, char*[] /* argv */)
{
  // Get the top level suite from the registry
  CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

  // Adds the test to the list of test to run
  CppUnit::TextUi::TestRunner runner;
  runner.addTest(suite);

  CppUnit::BriefTestProgressListener listener;
  runner.eventManager().addListener( &listener );

  // Change the default outputter to a compiler error format outputter
  runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(), std::cerr));
  //runner.setOutputter(new CppUnit::TextOutputter(&runner.result(), std::cerr));
    
  // Run the tests.
  bool wasSucessful = runner.run();

  // Return error code 1 if the one of test failed.
  return wasSucessful ? 0 : 1;
}


// Local Variables:
// mode: C++
// End:
