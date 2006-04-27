/**
 * @file   TestDLRewriter.cpp
 * @author Thomas Krennwallner
 * @date   Thu Apr 27 13:28:39 2006
 * 
 * @brief  
 * 
 * 
 */

#include "TestDLRewriter.h"

#include <fstream>
#include <sstream>

using namespace dlvhex::racer;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestDLRewriter);

void
TestDLRewriter::setUp()
{
}

void
TestDLRewriter::runDLRewrite()
{
  std::fstream fs("../../examples/tweety_bird.dlp");
  std::ostringstream os;

  DLRewriter dr(fs, os);
  dr.setUri("file:../../examples/tweety_bird.owl");
  dr.setStreams(&fs,&os);
  dr.rewrite();

  std::cout << "## " << os.str() << std::endl;
}
