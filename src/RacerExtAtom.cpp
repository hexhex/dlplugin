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

#include "OWLParser.h"
#include "RacerDirector.h"
#include "RacerQuery.h"
#include "RacerRunner.h"

#include <dlvhex/Atom.h>
#include <dlvhex/Term.h>
#include <dlvhex/AtomSet.h>

#include <iterator>
#include <iostream>

using namespace dlvhex::racer;


RacerExtAtom::RacerExtAtom(std::iostream& s)
  : stream(s)
{ }

RacerExtAtom::~RacerExtAtom()
{ }


RacerBaseDirector::QueryCtxPtr
RacerExtAtom::setupQuery(const PluginAtom::Query& query) const
{
  // just try to run RACER
  RacerRunner::instance()->run();

  RacerBaseDirector::QueryCtxPtr qctx(new QueryCtx);

  dlvhex::racer::Query& q = qctx->getQuery();

  // inputtuple[0] contains the KB URI constant
  std::string ontostr = query.getInputTuple()[0].getUnquotedString();
  q.setOntology(ontostr);

  // get namespace from owl document
  OWLParser p(ontostr);
  p.parseNamespace(q);

  // set query if input tuple contains a query atom
  if (query.getInputTuple().size() > 5)
    {
      q.setQuery(query.getInputTuple()[5]);
    }

  q.setInterpretation(query.getInterpretation());
  q.setPatternTuple(query.getPatternTuple());
  q.setPlusC(query.getInputTuple()[1]);
  q.setMinusC(query.getInputTuple()[2]);
  q.setPlusR(query.getInputTuple()[3]);
  q.setMinusR(query.getInputTuple()[4]);

  return qctx;
}





RacerCachingAtom::RacerCachingAtom(std::iostream& s,
				   RacerCachingDirector::RacerCache& c)
  : RacerExtAtom(s),
    cache(c)
{ }

RacerBaseDirector::DirectorPtr
RacerCachingAtom::getCachedDirectors(RacerBaseDirector* cmd) const
{
  RacerCompositeDirector* dir = new RacerCompositeDirector(stream);

  dir->add(new RacerUNA(stream));
  dir->add(new RacerOpenOWL(stream));
  dir->add(new RacerTempABox(stream));
  dir->add(new RacerConceptRolePM(stream));

  dir->add(cmd); // the actual command to send

  RacerBaseDirector::DirectorPtr comp(dir);
  RacerBaseDirector::DirectorPtr cacher;

  if (getenv("DEBUGCACHE") != 0)
    {
      cacher = RacerBaseDirector::DirectorPtr(new RacerDebugCachingDirector(cache,
									    comp
									    )
					      );
    }
  else
    {
      cacher = RacerBaseDirector::DirectorPtr(new RacerCachingDirector(cache,
								       comp)
					      );
    }

  return cacher;
}

void
RacerCachingAtom::retrieve(const PluginAtom::Query& query,
			   PluginAtom::Answer& answer) throw(PluginError)
{
  try
    {
      RacerBaseDirector::QueryCtxPtr qctx = setupQuery(query);

      RacerBaseDirector::DirectorPtr dirs = getDirectors(qctx->getQuery());

      qctx = dirs->query(qctx);

      answer = qctx->getAnswer();

      // we're done, so don't delete QueryCtx pointer -> it's cached
      qctx.release();
    }
  catch (RacerError& e)
    {
      throw PluginError(e.what());
    }
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

RacerBaseDirector::DirectorPtr
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

RacerBaseDirector::DirectorPtr
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

RacerBaseDirector::DirectorPtr
RacerConsistent::getDirectors(const dlvhex::racer::Query&) const
{
  RacerCompositeDirector* dir = new RacerCompositeDirector(stream);

  dir->add(new RacerUNA(stream));
  dir->add(new RacerOpenOWL(stream));
  dir->add(new RacerTempABox(stream));
  dir->add(new RacerConceptRolePM(stream));
  dir->add(new RacerABoxConsistent(stream));

  return RacerBaseDirector::DirectorPtr(dir);
}

void
RacerConsistent::retrieve(const PluginAtom::Query& query,
			  PluginAtom::Answer& answer) throw(PluginError)
{
  try
    {
      RacerBaseDirector::QueryCtxPtr qctx = setupQuery(query);

      RacerBaseDirector::DirectorPtr dirs = getDirectors(qctx->getQuery());

      qctx = dirs->query(qctx);

      answer = qctx->getAnswer();
    }
  catch (RacerError& e)
    {
      throw PluginError(e.what());
    }
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

RacerBaseDirector::DirectorPtr
RacerDatatypeRole::getDirectors(const dlvhex::racer::Query& query) const
{
  if (query.getType() == dlvhex::racer::Query::RelatedRetrieval) // retrieval mode
    {
      return RacerBaseDirector::DirectorPtr(); //getCachedDirectors(new RacerRoleQuery(stream));
    }
  else if (query.getType() == dlvhex::racer::Query::RightRetrieval) // pattern retrieval mode
    {
      return RacerBaseDirector::DirectorPtr(); //getCachedDirectors(new RacerIndvFillersQuery(stream));
    }
  else
    {
      throw PluginError("Wrong query type");
    }
}
