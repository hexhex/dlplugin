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
#include "Registry.h"

#include "dlvhex/Atom.h"
#include "dlvhex/Term.h"
#include "dlvhex/AtomSet.h"
#include "dlvhex/Error.h"

#include <boost/shared_ptr.hpp>

#include <iosfwd>

using namespace dlvhex::racer;


RacerExtAtom::RacerExtAtom(std::iostream& s)
  : stream(s)
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
  catch (std::exception& e)
    {
      throw PluginError(e.what());
    }
}

RacerCompositeDirector::shared_pointer
RacerExtAtom::getComposite(const dlvhex::racer::Query& query) const
{
  RacerCompositeDirector::shared_pointer comp(new RacerCompositeDirector(stream));

  if (!Registry::getUNA()) // only set UNA once
    {
      comp->add(new RacerUNA(stream));
      Registry::setUNA(true);
    }

  if (Registry::getOpenURL() != query.getOntology()) // only load ontology if its new
    {
      comp->add(new RacerOpenOWL(stream));
      comp->add(new RacerDirector<RacerImportOntologies,RacerIgnoreAnswer>(stream));
      Registry::setOpenURL(query.getOntology());
    }

  comp->add(new RacerTempABox(stream));
  comp->add(new RacerConceptRolePM(stream));

  return comp;
}


RacerCachingAtom::RacerCachingAtom(std::iostream& s, BaseCache& c)
  : RacerExtAtom(s),
    cache(c)
{ }

RacerBaseDirector::shared_pointer
RacerCachingAtom::getCachedDirectors(const dlvhex::racer::Query& q, RacerBaseDirector* cmd) const
{
  RacerCompositeDirector::shared_pointer comp = getComposite(q);

  comp->add(cmd); // the actual command to send

  unsigned level = Registry::getVerbose();

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



RacerConceptAtom::RacerConceptAtom(std::iostream& s, BaseCache& c)
  : RacerCachingAtom(s, c)
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
RacerConceptAtom::getDirectors(const dlvhex::racer::Query& query) const
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


RacerRoleAtom::RacerRoleAtom(std::iostream& s, BaseCache& c)
  : RacerCachingAtom(s, c)
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
RacerRoleAtom::getDirectors(const dlvhex::racer::Query& query) const
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





RacerConsistentAtom::RacerConsistentAtom(std::iostream& s)
  : RacerExtAtom(s)
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
RacerConsistentAtom::getDirectors(const dlvhex::racer::Query& q) const
{
  RacerCompositeDirector::shared_pointer comp = getComposite(q);

  comp->add(new RacerABoxConsistent(stream));

  return comp;
}




RacerDatatypeRoleAtom::RacerDatatypeRoleAtom(std::iostream& s, BaseCache& c)
  : RacerCachingAtom(s, c)
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
RacerDatatypeRoleAtom::getDirectors(const dlvhex::racer::Query& query) const
{
  if (query.isRetrieval() || query.isMixed())
    {
      ///@todo this is kind of a hack, maybe we should unify this stuff...
      RacerCompositeDirector* dir = new RacerCompositeDirector(stream);

      if (!Registry::getDataSubstrateMirroring())
	{
	  dir->add(new RacerDirector<RacerDataSubstrateMirrorBuilder,RacerIgnoreAnswer>(stream));
	  Registry::setDataSubstrateMirroring(true);
	}

      dir->add(new RacerIndvDataFillersQuery(stream));
      return getCachedDirectors(query, dir); 
    }
  else
    {
      throw PluginError("Wrong query type");
    }
}




RacerCQAtom::RacerCQAtom(std::iostream& s, BaseCache& c, unsigned n)
  : RacerCachingAtom(s, c)
{
  //
  // &dlCQn[kb,plusC,minusC,plusR,minusR,query](X_1,...,X_n)
  //

  setOutputArity(n);

  addInputConstant();  // kb URI
  addInputPredicate(); // plusC
  addInputPredicate(); // minusC
  addInputPredicate(); // plusR
  addInputPredicate(); // minusR
  addInputConstant();  // query
}

RacerBaseDirector::shared_pointer
RacerCQAtom::getDirectors(const dlvhex::racer::Query& query) const
{
  ///@todo right now we don't cache conjunctive queries

  RacerCompositeDirector::shared_pointer comp = getComposite(query);

  comp->add(new RacerConjunctiveQuery(stream));

  return comp;
}
