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

  void
  rewriteBody(const RuleBody_t& body, RuleBody_t& body1) const
  {
    BodyRewriter br;

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
	      }
	    else if (fn.find("dlCQ") == 0)
	      {
		const Tuple* in = new Tuple(ea->getInputTerms());
		const Tuple* out = new Tuple(ea->getArguments());
		
		br.add(new CQAtomRewriter(in, out));
	      }
	    else
	      {
		body1.insert(l); // non-dl extatom
	      }
	  }
	else
	  {
	    body1.insert(l); // non-extatom or naf atom
	  }
      }
	  
	  
    const RuleBody_t* dl_body = br.getBody();
	  
    body1.insert(dl_body->begin(), dl_body->end());
	  
    delete dl_body;
  }


public:
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
    // rewrite the body of rule r
    //

    RawPrintVisitor rpv(std::cerr);
    std::cerr << "optimizing: ";
    r->accept(rpv);

    RuleBody_t body1;

    rewriteBody(r->getBody(), body1);

    const_cast<Rule*>(r)->setBody(body1);

    std::cerr << " to ";
    r->accept(rpv);
    std::cerr << std::endl;
  }


  virtual void
  visitWeakConstraint(const WeakConstraint* wc)
  {
    //
    // rewrite the body weak constraint wc
    //

    RuleBody_t body1;

    rewriteBody(wc->getBody(), body1);

    const_cast<WeakConstraint*>(wc)->setBody(body1);
  }

};


void
DLOptimizer::optimize(NodeGraph& dg, AtomSet& edb)
{
  //
  // and now optimize it
  //

  return; ///@todo DLOptimizer::optimize is turned off

  RewritingVisitor rv;
  
  const std::vector<AtomNodePtr>& nodes = dg.getNodes();

  for (std::vector<AtomNodePtr>::const_iterator it = nodes.begin();
       it != nodes.end(); ++it)
    {
      const std::vector<Rule*>& rules = (*it)->getRules();

      for (std::vector<Rule*>::const_iterator it2 = rules.begin();
	   it2 != rules.end(); ++it2)
	{
	  (*it2)->accept(rv);
	}
    }
}
