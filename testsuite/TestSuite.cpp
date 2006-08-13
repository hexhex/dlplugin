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
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <iosfwd>
#include <string>
#include <functional>
#include <iterator>

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

  // Change the default outputter to a compiler error format outputter
  runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(), std::cerr));
    
  // Run the tests.
  bool wasSucessful = runner.run();

  // Return error code 1 if the one of test failed.
  return wasSucessful ? 0 : 1;
}
