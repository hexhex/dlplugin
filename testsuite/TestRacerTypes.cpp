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
  AtomPtr ap1(new Atom("p(b)"));
  AtomPtr ap2(new Atom("p(b)"));
  AtomPtr ap3(new Atom("r(a)"));
  AtomPtr ap4(new Atom("q(a,c)"));
  AtomPtr ap5(new Atom("q(a,b)"));
  AtomPtr ap6(new Atom("p(a)"));

  AtomSet is;
      
  is.insert(ap1);
  is.insert(ap2);
  is.insert(ap3);
  is.insert(ap4);
  is.insert(ap5);
  is.insert(ap6);

  AtomSet::const_iterator it = is.begin();
  CPPUNIT_ASSERT(*it++ == std::string("p(a)"));
  CPPUNIT_ASSERT(*it++ == std::string("p(b)"));
  CPPUNIT_ASSERT(*it++ == std::string("q(a,b)"));
  CPPUNIT_ASSERT(*it++ == std::string("q(a,c)"));
  CPPUNIT_ASSERT(*it++ == std::string("r(a)"));
}