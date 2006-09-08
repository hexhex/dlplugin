/* -*- C++ -*- */

/**
 * @file   RacerExtAtom.cpp
 * @author Thomas Krennwallner
 * @date   Mon Sep  5 13:37:11 2005
 * 
 * @brief  External atoms for dlvhex.
 * 
 * 
 */

#include "RacerExtAtom.h"
#include "RacerBuilder.h"
#include "RacerAnswerDriver.h"
#include "QueryDirector.h"
#include "QueryCtx.h"
#include "Query.h"
#include "Cache.h"
#include "Registry.h"
#include "RacerNRQL.h"
#include "RacerNRQLBuilder.h"
#include "RacerKBManager.h"

#include <dlvhex/Atom.h>
#include <dlvhex/Term.h>
#include <dlvhex/AtomSet.h>
#include <dlvhex/Error.h>
#include <dlvhex/PluginInterface.h>

#include <boost/shared_ptr.hpp>

#include <iosfwd>

using namespace dlvhex::dl;
using namespace dlvhex::dl::racer;

//
// following typedefs are here for easy of use
//

/// request to open an OWL document
typedef QueryDirector<RacerOpenOWLBuilder, RacerAnswerDriver> RacerOpenOWL;

/// extend ABox by a given set of individuals/pairs
typedef QueryDirector<RacerStateBuilder, RacerAnswerDriver> RacerConceptRolePM;


RacerExtAtom::RacerExtAtom(std::iostream& s, RacerKBManager& k)
  : stream(s), kbManager(k)
{ }


void
RacerExtAtom::retrieve(const PluginAtom::Query& query,
		       PluginAtom::Answer& answer) throw(PluginError)
{
  try
    {
      QueryCtx::shared_pointer qctx(new QueryCtx(query, kbManager));

      QueryBaseDirector::shared_pointer dirs = getDirectors(qctx->getQuery());

      qctx = dirs->query(qctx);

      if (Registry::getVerbose() > 0 && !qctx->getAnswer().getWarningMessage().empty())
	{
	  std::cerr << "Warning: " << qctx->getAnswer().getWarningMessage() << std::endl;
	}

      answer = qctx->getAnswer();
    }
  catch (std::exception& e)
    {
      throw PluginError(e.what());
    }
}

void
RacerExtAtom::setupRacer(QueryCompositeDirector::shared_pointer& comp) const
{
  if (!Registry::getUNA()) // only set UNA once
    {
      // turn on unique name assumption
      comp->add(new QueryDirector<RacerFunAdapterBuilder<RacerUNACmd>,
		RacerIgnoreAnswer>(stream)
	);

      Registry::setUNA(true);
    }
}

void
RacerExtAtom::openOntology(const dlvhex::dl::Query& query,
			   QueryCompositeDirector::shared_pointer& comp) const
{
  // check if Racer has an open KB with the name of the real URI of
  // the query's ontology, we can reuse it

  const std::string& kbname = query.getDLQuery()->getOntology()->getRealURI().getString();

  if (!kbManager.isOpenKB(kbname))
    {
      // update opened KBs
      kbManager.updateOpenKB();
  
      if (!kbManager.isOpenKB(kbname)) // only open OWL after we updated the open KBs
	{
	  comp->add(new RacerOpenOWL(stream));

	  // import all referenced ontologies
	  comp->add(new QueryDirector<RacerFunAdapterBuilder<RacerImportOntologiesCmd>,
		    RacerIgnoreAnswer>(stream)
		    );
	}
    }
}


void
RacerExtAtom::increaseABox(const dlvhex::dl::Query& /* query */,
			   QueryCompositeDirector::shared_pointer& comp) const
{
  // create a temporary ABox for the (state) command
  comp->add(new QueryDirector<RacerFunAdapterBuilder<RacerCloneABoxCmd>,
	    RacerIgnoreAnswer>(stream)
    );

  // add concept and role assertions via (state) command
  comp->add(new RacerConceptRolePM(stream));
}


RacerConsistentAtom::RacerConsistentAtom(std::iostream& s, RacerKBManager& k)
  : RacerExtAtom(s,k)
{
  //
  // &dlConsistent[kb,plusC,minusC,plusR,minusR]()
  //

  setOutputArity(0);

  addInputConstant();  // kb URI
  addInputPredicate(); // plusC
  addInputPredicate(); // minusC
  addInputPredicate(); // plusR
  addInputPredicate(); // minusR
}

QueryBaseDirector::shared_pointer
RacerConsistentAtom::getDirectors(const dlvhex::dl::Query& q) const
{
  QueryCompositeDirector::shared_pointer comp(new QueryCompositeDirector(stream));

  setupRacer(comp);
  openOntology(q, comp);
  increaseABox(q, comp);

  // ask whether ABox is consistent
  comp->add
    (new QueryDirector<RacerFunAdapterBuilder<RacerABoxConsistentCmd>,RacerAnswerDriver>(stream)
     );

  return comp;
}
