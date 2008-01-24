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
#include <dlvhex/PluginContainer.h>

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


void
DLOptimizer::optimize(NodeGraph& dg, AtomSet& edb)
{
  if (!getRewriting())
    {
      return;
    }

  //
  // and now optimize it
  //

  const std::vector<Rule*>& rules = dg.getProgram();

  Program p;

  for (std::vector<Rule*>::const_iterator it = rules.begin();
       it != rules.end(); ++it)
    {
      const RuleBody_t& body = (*it)->getBody();
      RuleBody_t newbody;
      BodyRewriter rewriter;

      //
      // dispatch the body and get the dl-part for the optimization
      //

      for (RuleBody_t::const_iterator bit = body.begin(); bit != body.end(); ++bit)
      {
	Literal* l = *bit;

	if (typeid(*l->getAtom()) == typeid(ExternalAtom) && !l->isNAF())
	  {
	    ExternalAtom* ea = dynamic_cast<ExternalAtom*>(l->getAtom().get());
	    
	    const std::string& fn = ea->getFunctionName();
	    
	    if (fn == "dlC" || fn == "dlR" || fn == "dlDR" || fn.find("dlCQ") == 0)
	      {
		rewriter.add(new ExtAtomRewriter(l->getAtom()));
	      }
	    else
	      {
		newbody.insert(l); // non-dl extatom
	      }
	  }
	else
	  {
	    newbody.insert(l); // non-extatom or naf atom
	  }
      }
      
      // and now rewrite the dl-body of rewriter and add the rewritten
      // body to newbody
      rewriter.bodyOptimizer(newbody);

      // set new body and add the rule to the program
      (*it)->setBody(newbody);
      p.addRule(*it);
    }

  //
  // rebuild the graph with the program
  //
  PluginContainer* container = PluginContainer::instance("");
  GraphBuilder gb;
  gb.run(p, dg, *container);
}


// Local Variables:
// mode: C++
// End:
