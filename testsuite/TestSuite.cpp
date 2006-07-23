#include "TestSuite.h"

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <iosfwd>
#include <string>
#include <functional>
#include <iterator>

using namespace dlvhex::racer;

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
