/**
 * @file   TestRacerTypes.cpp
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:41:07 2006
 * 
 * @brief  
 * 
 * 
 */

#include "TestRacerTypes.h"

using namespace dlvhex::racer;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestRacerTypes);

void
TestRacerTypes::runRacerSetTest()
{
  AtomSet is;
      
  is.insert(Atom("p(b)"));
  is.insert(Atom("p(b)"));
  is.insert(Atom("q(a)"));
  is.insert(Atom("q(a,c)"));
  is.insert(Atom("q(a,b)"));
  is.insert(Atom("p(a)"));

  AtomSet::const_iterator it = is.begin();
  CPPUNIT_ASSERT(*it++ == std::string("p(a)"));
  CPPUNIT_ASSERT(*it++ == std::string("p(b)"));
  CPPUNIT_ASSERT(*it++ == std::string("q(a)"));
  CPPUNIT_ASSERT(*it++ == std::string("q(a,b)"));
  CPPUNIT_ASSERT(*it++ == std::string("q(a,c)"));
}
