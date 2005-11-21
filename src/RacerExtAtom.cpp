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

#include <dlvhex/errorHandling.h>
#include <dlvhex/Atom.h>
#include <dlvhex/Term.h>
#include <dlvhex/Interpretation.h>

#include <assert.h>

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
  RacerRunner::instance()->run();

  RacerBaseDirector::QueryCtxPtr qctx(new QueryCtx);

  dlvhex::racer::Query& q = qctx->getQuery();

  // inputtuple[0] contains the KB URI constant
  std::string ontostr = query.getInputTuple()[0].getUnquotedString();
  q.setOntology(ontostr);


  ///@todo read nspace from owl document
#if 1
  q.setNamespace("http://www.kr.tuwien.ac.at/staff/roman/shop#");
#else
  // get namespace from owl document
  OWLParser p("file:" + ontostr);
  p.parseNamespace(q);
#endif // "1"

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

template<class Director, class Cacher>
RacerBaseDirector::DirectorPtr
RacerCachingAtom::getCachedDirectors(RacerCompositeDirector* dir) const
{
  dir->add(new RacerUNA(stream));
  dir->add(new RacerOpenOWL(stream));
  dir->add(new RacerTempABox(stream));
  dir->add(new RacerConceptRolePM(stream));

  dir->add(new Director(stream));

  RacerBaseDirector::DirectorPtr comp(dir);
  RacerBaseDirector::DirectorPtr cacher(new Cacher(cache, comp));

  return cacher;
}

void
RacerCachingAtom::retrieve(const PluginAtom::Query& query,
			   PluginAtom::Answer& answer) throw(PluginError)
{
  try
    {
      RacerBaseDirector::DirectorPtr dirs = getDirectors(query);

      RacerBaseDirector::QueryCtxPtr qctx = setupQuery(query);

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
RacerConcept::getDirectors(const PluginAtom::Query& query) const
{
  if (query.getPatternTuple().size() != 1)
    {
      throw PluginError("Pattertuple size mismatch.");
    }

  const Term& x = query.getPatternTuple()[0];

  if (x.isVariable()) // retrieval mode
    {
      return getCachedDirectors<RacerConceptQuery,RacerTermCache>
	(new RacerRetrieveComposite(stream));
    }
  else // boolean query mode
    {
      return getCachedDirectors<RacerIsConceptQuery,RacerBooleanCache>
	(new RacerQueryComposite(stream));
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
RacerRole::getDirectors(const PluginAtom::Query& query) const
{
  if (query.getPatternTuple().size() != 2)
    {
      throw PluginError("Pattertuple size mismatch.");
    }

  const Term& x = query.getPatternTuple()[0];
  const Term& y = query.getPatternTuple()[1];

  if (x.isVariable() && y.isVariable()) // retrieval mode
    {
      return getCachedDirectors<RacerRoleQuery,RacerTermCache>
	(new RacerRetrieveComposite(stream,
				    RacerRetrieveComposite::RELATED
				    )
	 );
    }
  else if (!x.isVariable() && !y.isVariable()) // boolean query mode
    {
      return getCachedDirectors<RacerIsRoleQuery, RacerBooleanCache>
	(new RacerQueryComposite(stream));
    }
  else // pattern retrieval mode
    {
      return getCachedDirectors<RacerIndvFillersQuery, RacerTermCache>
	(new RacerRetrieveComposite(stream));
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
RacerConsistent::getDirectors(const PluginAtom::Query&) const
{
  RacerQueryComposite* dir = new RacerQueryComposite(stream);

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
      RacerBaseDirector::DirectorPtr dirs = getDirectors(query);

      RacerBaseDirector::QueryCtxPtr qctx = setupQuery(query);

      qctx = dirs->query(qctx);

      answer = qctx->getAnswer();
    }
  catch (RacerError& e)
    {
      throw PluginError(e.what());
    }
}
