/* -*- C++ -*- */

/**
 * @file   DLOptimizer.cpp
 * @author Thomas Krennwallner
 * @date   Tue Jul 25 09:23:09 2006
 * 
 * @brief  Optimizes the DL part of HEX programs.
 * 
 * 
 */

#include "DLOptimizer.h"
#include "HexDLRewriter.h"
#include "Registry.h"
#include "DLError.h"

#include <dlvhex/Program.h>
#include <dlvhex/GraphBuilder.h>
#include <dlvhex/DependencyGraph.h>
#include <dlvhex/ComponentFinder.h>
#include <dlvhex/BoostComponentFinder.h>
#include <dlvhex/PrintVisitor.h>

using namespace dlvhex::dl;


DLOptimizer::DLOptimizer()
  : PluginOptimizer(),
    doRewriting(true)
{ }


DLOptimizer::DLOptimizer(const DLOptimizer& d)
  : PluginOptimizer(),
    doRewriting(d.doRewriting)
{ }


DLOptimizer&
DLOptimizer::operator= (const DLOptimizer&)
{
  // noop
  return *this;
}


DLOptimizer::~DLOptimizer()
{ }


void
DLOptimizer::setRewriting(bool yesno)
{
  doRewriting = yesno;
}


bool
DLOptimizer::getRewriting()
{
  return doRewriting;
}


/**
 * 
 * 
 */
class RewritingVisitor : public BaseVisitor
{
private:
  Program& rewritten;

  const RuleBody_t*
  rewriteBody(const RuleBody_t& body) const
  {
    BodyRewriter br;

    RuleBody_t* body1 = new RuleBody_t;

    for (RuleBody_t::const_iterator it = body.begin(); it != body.end(); ++it)
      {
	Literal* l = *it;

	///@todo maybe we should use the visiting method to dispatch
	///the type
	
	if (typeid(*l->getAtom()) == typeid(ExternalAtom) && !l->isNAF())
	  {
	    ExternalAtom* ea = dynamic_cast<ExternalAtom*>(l->getAtom().get());
	    
	    const std::string& fn = ea->getFunctionName();
	    
	    if (fn == "dlC" || fn == "dlR" || fn == "dlDR")
	      {
		const std::string* n = new std::string(fn);
		const Tuple* in = new Tuple(ea->getInputTerms());
		const Tuple* out = new Tuple(ea->getArguments());
		
		br.add(new SimpleDLAtomRewriter(n, in, out));
		
		///@todo this burns the readers eyes, but for now it
		///prevents memory leaks
		delete l;
	      }
	    else if (fn.find("dlCQ") == 0)
	      {
		const Tuple* in = new Tuple(ea->getInputTerms());
		const Tuple* out = new Tuple(ea->getArguments());
		
		br.add(new CQAtomRewriter(in, out));
		
		///@todo this burns the readers eyes, but for now it
		///prevents memory leaks
		delete l;
	      }
	    else
	      {
		body1->insert(l); // non-dl extatom
	      }
	  }
	else
	  {
	    body1->insert(l); // non-extatom or naf atom
	  }
      }
	  
	  
    const RuleBody_t* dl_body = br.getBody();
	  
    body1->insert(dl_body->begin(), dl_body->end());
	  
    delete dl_body;

    return body1;
  }


public:
  RewritingVisitor(Program& r)
    : rewritten(r)
  { }
  

  // these methods are no-ops (as for now)

  virtual void
  visitAtomSet(const AtomSet*)
  { }
  virtual void
  visitLiteral(const Literal*)
  { }
  virtual void
  visitAtom(const Atom*)
  { }
  virtual void
  visitExternalAtom(const ExternalAtom*)
  { }
  virtual void
  visitBuiltinPredicate(const BuiltinPredicate*)
  { }
  virtual void
  visitAggregateAtom(const AggregateAtom*)
  { }
  virtual void
  visitLiteral(const Atom*)
  { }


  virtual void
  visitRule(const Rule* r)
  {
    //
    // rewrite the body the rule
    //

    const RuleHead_t& head = r->getHead();
    const RuleBody_t& body = r->getBody();

    const RuleBody_t* body1 = rewriteBody(body);

    rewritten.addRule(new Rule(head, *body1));

    delete body1;
  }


  virtual void
  visitWeakConstraint(const WeakConstraint* wc)
  {
    //
    // rewrite the body the weak constraint
    //

    const RuleBody_t& body = wc->getBody();

    const RuleBody_t* body1 = rewriteBody(body);

    rewritten.addRule(new WeakConstraint(*body1, wc->getWeight(), wc->getLevel()));

    delete body1;
  }

};


void
DLOptimizer::optimize(NodeGraph& dg, AtomSet& edb)
{
  return;
#if 0
  //
  // and now optimize it
  //

  Program prog;

  //
  // build dependency graph
  //

//   GraphBuilder gb;
//   BoostComponentFinder cf;

//   DependencyGraph dg(prog, &gb, &cf);

//   Subgraph* sg = 0;

//   while ((sg = dg.getNextSubgraph()))
//     {
//       sg->dump(std::cerr);

//       std::vector<Component*> leaves;
      
//       sg->getUnsolvedLeaves(leaves);
      
//       for (std::vector<Component*>::const_iterator it = leaves.begin(); it != leaves.end(); ++it)
// 	{
// 	  Program p = (*it)->getBottom();
// 	  std::cerr << "Component found: " << std::endl;
// 	  for (Program::const_iterator pit = p.begin(); pit != p.end(); ++pit)
// 	    {
// 	      std::cerr << *pit << std::endl;
// 	    }
// 	  std::cerr << std::endl;
// 	}
//     }

  //
  // rewrite the rules
  //

  Program rewritten;
  RewritingVisitor rv(rewritten);

  for (Program::const_iterator it = prog.begin(); it != prog.end(); ++it)
    {
      ///@todo does not take unused predicates in \lambda in account

      const Rule* r = *it;

      if (!getRewriting()) // don't rewrite the rule
	{
	  rewritten.addRule(r);
	}
      else // rewrite the rule
	{
	  r->accept(rv);

	  ///@todo this burns the readers eyes, but for now it
	  ///prevents memory leaks
	  delete r;
	}
    }
#endif // 0
}
