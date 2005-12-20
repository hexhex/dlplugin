#include <cppunit/ui/text/TestRunner.h>
#include "TestRacerRunner.h"
#include "TestRacerStream.h"
#include "TestRacerBuilder.h"
#include "TestRacerParse.h"
#include "TestRacerDirector.h"
#include "TestRacerInterface.h"
#include "TestRacerTypes.h"
#include "TestRacerQuery.h"
#include "TestOWLParser.h"

using namespace dlvhex::racer;

int main(int, char**)
{
  CppUnit::TextUi::TestRunner runner;
  runner.addTest(TestRacerRunner::suite());
  runner.addTest(TestRacerStream::suite());
  runner.addTest(TestRacerBuilder::suite());
  runner.addTest(TestRacerParse::suite());
  runner.addTest(TestRacerDirector::suite());
  runner.addTest(TestRacerInterface::suite());
  runner.addTest(TestRacerTypes::suite());
  runner.addTest(TestRacerQuery::suite());
  runner.addTest(TestOWLParser::suite());
  return !runner.run();
}
