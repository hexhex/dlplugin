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

#include "RacerDirector.h"
#include "RacerQuery.h"
#include "RacerRunner.h"

#include <dlvhex/Atom.h>
#include <dlvhex/Term.h>
#include <dlvhex/AtomSet.h>

#include <boost/shared_ptr.hpp>

#include <iterator>
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
      RacerRunner::instance()->run();

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



RacerCachingAtom::RacerCachingAtom(std::iostream& s,
				   RacerCachingDirector::RacerCache& c)
  : RacerExtAtom(s),
    cache(c)
{ }

RacerBaseDirector::shared_pointer
RacerCachingAtom::getCachedDirectors(RacerBaseDirector* cmd) const
{
  RacerCompositeDirector::shared_pointer comp(new RacerCompositeDirector(stream));

  comp->add(new RacerUNA(stream));
  comp->add(new RacerOpenOWL(stream));
  comp->add(new RacerTempABox(stream));
  comp->add(new RacerConceptRolePM(stream));

  comp->add(cmd); // the actual command to send

  const char* c = getenv("DEBUGCACHE");

  if (c != 0) // found environment variable DEBUGCACHE
    {
      int val;
      std::istringstream(c) >> val;

      if (val == 0) // no caching
	{
	  return comp;
	}
      else // debug query caching
	{
	  return RacerBaseDirector::shared_pointer(new RacerDebugCachingDirector
						   (cache, comp)
						   );
	}
    }

  // default action is query caching
  return RacerBaseDirector::shared_pointer(new RacerCachingDirector
					   (cache, comp)
					   );
}



RacerConcept::RacerConcept(std::iostream& s, RacerCachingDirector::RacerCache& c)
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
RacerConcept::getDirectors(const dlvhex::racer::Query& query) const
{
  if (query.getType() == dlvhex::racer::Query::Retrieval) // retrieval mode
    {
      return getCachedDirectors(new RacerConceptQuery(stream));
    }
  else if (query.getType() == dlvhex::racer::Query::Boolean) // boolean query mode
    {
      return getCachedDirectors(new RacerIsConceptQuery(stream));
    }
  else
    {
      throw PluginError("Wrong query type");
    }
}


RacerRole::RacerRole(std::iostream& s, RacerCachingDirector::RacerCache& c)
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
RacerRole::getDirectors(const dlvhex::racer::Query& query) const
{
  if (query.getType() == dlvhex::racer::Query::RelatedRetrieval) // retrieval mode
    {
      return getCachedDirectors(new RacerRoleQuery(stream));
    }
  else if (query.getType() == dlvhex::racer::Query::RelatedBoolean) // boolean query mode
    {
      return getCachedDirectors(new RacerIsRoleQuery(stream));
    }
  else if (query.getType() == dlvhex::racer::Query::LeftRetrieval
	   || query.getType() == dlvhex::racer::Query::RightRetrieval) // pattern retrieval mode
    {
      return getCachedDirectors(new RacerIndvFillersQuery(stream));
    }
  else
    {
      throw PluginError("Wrong query type");
    }
}





RacerConsistent::RacerConsistent(std::iostream& s)
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
RacerConsistent::getDirectors(const dlvhex::racer::Query&) const
{
  RacerCompositeDirector::shared_pointer comp(new RacerCompositeDirector(stream));

  comp->add(new RacerUNA(stream));
  comp->add(new RacerOpenOWL(stream));
  comp->add(new RacerTempABox(stream));
  comp->add(new RacerConceptRolePM(stream));
  comp->add(new RacerABoxConsistent(stream));

  return comp;
}




RacerDatatypeRole::RacerDatatypeRole(std::iostream& s,
				     RacerCachingDirector::RacerCache& c)
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
RacerDatatypeRole::getDirectors(const dlvhex::racer::Query& query) const
{
  if (query.getType() == dlvhex::racer::Query::RelatedRetrieval ||
      query.getType() == dlvhex::racer::Query::RightRetrieval)
    {
      ///@todo this is kind of a hack, maybe we should unify this stuff...
      RacerCompositeDirector* dir = new RacerCompositeDirector(stream);
      dir->add(new RacerDirector<RacerDataSubstrateMirrorBuilder,RacerIgnoreAnswer>(stream));
      dir->add(new RacerIndvDataFillersQuery(stream));
      return getCachedDirectors(dir); 
    }
  else
    {
      throw PluginError("Wrong query type");
    }
}
