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
 * @file   TestDLRewriter.cpp
 * @author Thomas Krennwallner
 * @date   Thu Apr 27 13:28:39 2006
 * 
 * @brief  Tests the DL Rewriter facility.
 * 
 * 
 */

#include "TestDLOptimizer.h"

#include "DLOptimizer.h"

#include <dlvhex/GraphBuilder.h>
#include <dlvhex/Error.h>
#include <dlvhex/PluginContainer.h>

#include <sstream>

using namespace dlvhex::dl;
using namespace dlvhex::dl::test;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestDLOptimizer);

void
TestDLOptimizer::runDLOptimize()
{
  // initialize the PluginContainer with our own plugin
  PluginContainer* container = PluginContainer::instance("");

  AtomPtr h1(new Atom("p(X,Y)"));
  RuleHead_t h;
  h.insert(h1);

  Tuple out1;
  Tuple out2;
  Tuple out3;
  Tuple in1;
  Tuple in2;
  Tuple in3;

  out1.push_back(Term("X"));
  out1.push_back(Term("Y"));

  out2.push_back(Term("Y"));
  out2.push_back(Term("Z"));

  out3.push_back(Term("X"));
  out3.push_back(Term("Y"));
  out3.push_back(Term("Z"));

  in1.push_back(Term("my.owl", true));
  in2.push_back(Term("my.owl", true));
  in3.push_back(Term("my.owl", true));

  in1.push_back(Term("a"));
  in2.push_back(Term("a"));
  in3.push_back(Term("a"));

  in1.push_back(Term("a"));
  in2.push_back(Term("a"));
  in3.push_back(Term("a"));

  in1.push_back(Term("a"));
  in2.push_back(Term("a"));
  in3.push_back(Term("a"));

  in1.push_back(Term("a"));
  in2.push_back(Term("a"));
  in3.push_back(Term("a"));

  in1.push_back(Term("Q1(X),Q2(Y)", true));
  in2.push_back(Term("Q3(Y),Q4(Z)", true));
  in3.push_back(Term("Q1(X),Q2(Y),Q3(Y),Q4(Z)", true));

  ExternalAtom* ea1 = new ExternalAtom("dlCQ2", out1, in1, 0);
  ExternalAtom* ea2 = new ExternalAtom("dlCQ2", out2, in2, 0);
  ExternalAtom* ea3 = new ExternalAtom("dlCQ3", out3, in3, 0);

  AtomPtr b1(ea1);
  AtomPtr b2(ea2);
  AtomPtr b3(ea3);

  Literal l1(b1);
  Literal l2(b2);
  Literal l3(b3);

  RuleBody_t b;
  b.insert(&l1);
  b.insert(&l2);

  RuleBody_t newb;
  newb.insert(&l3);

  Rule r(h, b);
  Rule newr(h, newb);

  Program p1;
  p1.addRule(&r);


  //
  // The GraphBuilder creates nodes and dependency edges from the raw program.
  //
  GraphBuilder gb;

  NodeGraph nodegraph;

  try
    {
      gb.run(p1, nodegraph, *container);
    }
  catch (GeneralError& e)
    {
      std::cerr << e.getErrorMsg() << std::endl << std::endl;
      
      exit(1);
    }

  AtomSet edb;

  DLOptimizer opt;

  std::cerr << "##############" << std::endl;
  std::cerr << "old r: " << r << std::endl;

  opt.optimize(nodegraph, edb);

  std::cerr << "new r: " << r << std::endl;
  std::cerr << "and newr: " << newr << std::endl;
  std::cerr << "##############" << std::endl;

  ///@todo assertion not there
  //CPPUNIT_ASSERT(r == newr);
}


// Local Variables:
// mode: C++
// End:
