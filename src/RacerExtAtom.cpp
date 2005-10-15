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

#include <dlvhex/errorHandling.h>
#include <dlvhex/Atom.h>
#include <dlvhex/Term.h>

#include <assert.h>


using namespace dlvhex::racer;


RacerExtAtom::RacerExtAtom(std::iostream& s, RacerCachingDirector::RacerCache& c)
  : stream(s),
    cache(c)
{ }

RacerExtAtom::~RacerExtAtom()
{ }


RacerBaseDirector::QueryCtxPtr
RacerExtAtom::setupQuery(const FACTSETVECTOR& in) const
{
  RacerBaseDirector::QueryCtxPtr qctx(new QueryCtx);

  Query& q = qctx->getQuery();

  // in[0] contains the KB predicate
  GAtom onto = *in[0].begin();
  std::string ontostr = onto.getArgument(1).getUnquotedString();
  q.setOntology(ontostr);

  ///@todo read nspace from owl document
#if 1
  q.setNamespace("http://www.kr.tuwien.ac.at/staff/roman/shop#");
#else
  // get namespace from owl document
  OWLParser p("file:" + ontostr);
  p.parseNamespace(q);
#endif // "1"

  q.setQuery(*in[5].begin());
  q.setPlusConcept(in[1]);
  q.setMinusConcept(in[2]);
  q.setPlusRole(in[3]);
  //   q.setMinusRole(in[4]);

  return qctx;
}

template<class Director, class Cacher>
RacerBaseDirector::DirectorPtr
RacerExtAtom::getDirectors() const
{
  RacerBaseDirector::DirectorPtr una   (new RacerUNA(stream));
  RacerBaseDirector::DirectorPtr owl   (new RacerOpenOWL(stream, una));
  RacerBaseDirector::DirectorPtr tmp   (new RacerTempABox(stream, owl));
  RacerBaseDirector::DirectorPtr crpm  (new RacerConceptRolePM(stream, tmp));

  RacerBaseDirector::DirectorPtr query (new Director(stream, crpm));
  RacerBaseDirector::DirectorPtr cacher(new Cacher(cache, query));

  return cacher;
}

void
RacerExtAtom::retrieve(FACTSETVECTOR& in, TUPLEVECTOR& out) throw(PluginError)
{
  assert(in.size() == 6);

  if (in[5].empty())
    {
      // is query empty? -> ignore request
      return;
    }

  try
    {
      RacerBaseDirector::DirectorPtr dirs = getRetrievalDirectors();

      RacerBaseDirector::QueryCtxPtr qctx = setupQuery(in);

      qctx = dirs->query(qctx);

      out = qctx->getAnswer().getTuples();

      // we're done, so don't delete QueryCtx pointer -> it's cached
      qctx.release();
    }
  catch (RacerError& e)
    {
      throw PluginError(e.what());
    }
}

bool
RacerExtAtom::query(FACTSETVECTOR& in, Tuple&) throw(PluginError)
{
  assert(in.size() == 6);

  if (in[5].empty())
    {
      ///@todo is query empty? -> return??
      return false;
    }

  try
    {
      RacerBaseDirector::DirectorPtr dirs = getQueryDirectors();

      RacerBaseDirector::QueryCtxPtr qctx = setupQuery(in);

      qctx = dirs->query(qctx);

      bool ret = qctx->getAnswer().getAnswer();

      // we're done, so don't delete QueryCtx -> it's cached
      qctx.release();

      return ret;
    }
  catch (RacerError& e)
    {
      throw PluginError(e.what());
    }
}





RacerConcept::RacerConcept(std::iostream& s, RacerCachingDirector::RacerCache& c)
  : RacerExtAtom(s, c)
{ }

RacerConcept::~RacerConcept()
{ }

RacerBaseDirector::DirectorPtr
RacerConcept::getRetrievalDirectors() const
{
  return getDirectors<RacerConceptQuery, RacerTermCache>();
}

RacerBaseDirector::DirectorPtr
RacerConcept::getQueryDirectors() const
{
  return getDirectors<RacerIsConceptQuery, RacerBooleanCache>();
}


RacerRole::RacerRole(std::iostream& s, RacerCachingDirector::RacerCache& c)
  : RacerExtAtom(s, c)
{ }

RacerRole::~RacerRole()
{ }

RacerBaseDirector::DirectorPtr
RacerRole::getRetrievalDirectors() const
{
  return getDirectors<RacerRoleQuery, RacerTermCache>();
}

RacerBaseDirector::DirectorPtr
RacerRole::getQueryDirectors() const
{
  return getDirectors<RacerIsRoleQuery, RacerBooleanCache>();
}
