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
 * @file   TestRacerTypes.cpp
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:41:07 2006
 * 
 * @brief  TestCase for some types.
 * 
 * 
 */

#include <dlvhex/Atom.h>
#include <dlvhex/AtomSet.h>

#include "TestRacerTypes.h"

using namespace dlvhex::dl;
using namespace dlvhex::dl::test;

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

  CPPUNIT_ASSERT(is.size() == 5);
}


// Local Variables:
// mode: C++
// End:
