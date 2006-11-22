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

#include <sstream>

using namespace dlvhex::dl;
using namespace dlvhex::dl::test;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestDLOptimizer);

void
TestDLOptimizer::runDLOptimize()
{
  return; ///@todo TextDLOptimizer::runDLOptimize is turned off

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
  ea1->findPluginAtom();
  ExternalAtom* ea2 = new ExternalAtom("dlCQ2", out2, in2, 0);
  ea2->findPluginAtom();
  ExternalAtom* ea3 = new ExternalAtom("dlCQ3", out3, in3, 0);
  ea3->findPluginAtom();

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
  b.insert(&l3);

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
      gb.run(p1, nodegraph);
    }
  catch (GeneralError& e)
    {
      std::cerr << e.getErrorMsg() << std::endl << std::endl;
      
      exit(1);
    }

  AtomSet edb;

  DLOptimizer opt;

  opt.optimize(nodegraph, edb);

  CPPUNIT_ASSERT(r == newr);
}
