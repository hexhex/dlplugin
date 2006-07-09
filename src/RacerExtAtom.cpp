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
#include "RacerInterface.h"
#include "RacerDirector.h"
#include "RacerQuery.h"
#include "Cache.h"

#include "dlvhex/Atom.h"
#include "dlvhex/Term.h"
#include "dlvhex/AtomSet.h"
#include "dlvhex/Error.h"

#include <boost/shared_ptr.hpp>

#include <iterator>
#include <iosfwd>

using namespace dlvhex::racer;


RacerExtAtom::RacerExtAtom(std::iostream& s,
			   Registry& regs)
  : stream(s),
    registry(regs)
{ }

RacerExtAtom::~RacerExtAtom()
{ }


void
RacerExtAtom::retrieve(const PluginAtom::Query& query,
		       PluginAtom::Answer& answer) throw(PluginError)
{
  try
    {
      // just try to run RACER
      TheRacerInterface::instance()->startRacer();

      QueryCtx::shared_pointer qctx(new QueryCtx(query));

      RacerBaseDirector::shared_pointer dirs = getDirectors(qctx->getQuery());

      qctx = dirs->query(qctx);

      answer = qctx->getAnswer();
    }
  catch (RacerError& e)
    {
      throw PluginError(e.what());
    }
}

RacerCompositeDirector::shared_pointer
RacerExtAtom::getComposite(const dlvhex::racer::Query& query) const
{
  RacerCompositeDirector::shared_pointer comp(new RacerCompositeDirector(stream));

  if (!registry.getUNA()) // only set UNA once
    {
      comp->add(new RacerUNA(stream));
      registry.setUNA(true);
    }

  if (registry.getOpenURL() != query.getOntology()) // only load ontology if its new
    {
      comp->add(new RacerOpenOWL(stream));
      comp->add(new RacerDirector<RacerImportOntologies,RacerIgnoreAnswer>(stream));
      registry.setOpenURL(query.getOntology());
    }

  comp->add(new RacerTempABox(stream));
  comp->add(new RacerConceptRolePM(stream));

  return comp;
}


RacerCachingAtom::RacerCachingAtom(std::iostream& s, BaseCache& c, Registry& regs)
  : RacerExtAtom(s, regs),
    cache(c)
{ }

RacerBaseDirector::shared_pointer
RacerCachingAtom::getCachedDirectors(const dlvhex::racer::Query& q, RacerBaseDirector* cmd) const
{
  RacerCompositeDirector::shared_pointer comp = getComposite(q);

  comp->add(cmd); // the actual command to send

  unsigned level = registry.getVerbose();

  if (level == 0) // no caching
    {
      return comp;
    }
  else // default action is query caching
    {
      return RacerBaseDirector::shared_pointer(new RacerCachingDirector
					       (cache, comp)
					       );
    }
}



RacerConcept::RacerConcept(std::iostream& s, BaseCache& c, Registry& regs)
  : RacerCachingAtom(s, c, regs)
{
  //
  // &dlC[kb,plusC,minusC,plusR,minusR,query](X)
  //

  setOutputArity(1);

  addInputConstant();  // kb URI
  addInputPredicate(); // plusC
  addInputPredicate(); // minusC
  addInputPredicate(); // plusR
  addInputPredicate(); // minusR
  addInputConstant();  // query
}

RacerBaseDirector::shared_pointer
RacerConcept::getDirectors(const dlvhex::racer::Query& query) const
{
  if (query.isRetrieval()) // retrieval mode
    {
      return getCachedDirectors(query, new RacerConceptQuery(stream));
    }
  else if (query.isBoolean()) // boolean query mode
    {
      return getCachedDirectors(query, new RacerIsConceptQuery(stream));
    }
  else
    {
      throw PluginError("Wrong query type");
    }
}


RacerRole::RacerRole(std::iostream& s, BaseCache& c, Registry& regs)
  : RacerCachingAtom(s, c, regs)
{
  //
  // &dlR[kb,plusC,minusC,plusR,minusR,query](X,Y)
  //

  setOutputArity(2);

  addInputConstant();  // kb URI
  addInputPredicate(); // plusC
  addInputPredicate(); // minusC
  addInputPredicate(); // plusR
  addInputPredicate(); // minusR
  addInputConstant();  // query
}

RacerBaseDirector::shared_pointer
RacerRole::getDirectors(const dlvhex::racer::Query& query) const
{
  if (query.isRetrieval()) // retrieval mode
    {
      return getCachedDirectors(query, new RacerRoleQuery(stream));
    }
  else if (query.isBoolean()) // boolean query mode
    {
      return getCachedDirectors(query, new RacerIsRoleQuery(stream));
    }
  else if (query.isMixed()) // pattern retrieval mode
    {
      return getCachedDirectors(query, new RacerIndvFillersQuery(stream));
    }
  else
    {
      throw PluginError("Wrong query type");
    }
}





RacerConsistent::RacerConsistent(std::iostream& s,
				 Registry& regs)
  : RacerExtAtom(s, regs)
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

RacerBaseDirector::shared_pointer
RacerConsistent::getDirectors(const dlvhex::racer::Query& q) const
{
  RacerCompositeDirector::shared_pointer comp = getComposite(q);

  comp->add(new RacerABoxConsistent(stream));

  return comp;
}




RacerDatatypeRole::RacerDatatypeRole(std::iostream& s, BaseCache& c, Registry& regs)
  : RacerCachingAtom(s, c, regs)
{
  //
  // &dlDR[kb,plusC,minusC,plusR,minusR,query](X,Y)
  //

  setOutputArity(2);

  addInputConstant();  // kb URI
  addInputPredicate(); // plusC
  addInputPredicate(); // minusC
  addInputPredicate(); // plusR
  addInputPredicate(); // minusR
  addInputConstant();  // query
}

RacerBaseDirector::shared_pointer
RacerDatatypeRole::getDirectors(const dlvhex::racer::Query& query) const
{
  if (query.isRetrieval() || query.isMixed())
    {
      ///@todo this is kind of a hack, maybe we should unify this stuff...
      RacerCompositeDirector* dir = new RacerCompositeDirector(stream);

      if (!registry.getDataSubstrateMirroring())
	{
	  dir->add(new RacerDirector<RacerDataSubstrateMirrorBuilder,RacerIgnoreAnswer>(stream));
	  registry.setDataSubstrateMirroring(true);
	}

      dir->add(new RacerIndvDataFillersQuery(stream));
      return getCachedDirectors(query, dir); 
    }
  else
    {
      throw PluginError("Wrong query type");
    }
}
