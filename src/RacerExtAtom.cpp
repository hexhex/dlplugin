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
#include <dlvhex/Interpretation.h>

#include <assert.h>

#include <iterator>
#include <iostream>

using namespace dlvhex::racer;


RacerExtAtom::RacerExtAtom(std::iostream& s, RacerCachingDirector::RacerCache& c)
  : stream(s),
    cache(c)
{ }

RacerExtAtom::~RacerExtAtom()
{ }


RacerBaseDirector::QueryCtxPtr
RacerExtAtom::setupQuery(const Interpretation& in,
			 const Tuple& parms,
			 const Tuple& indv) const
{
  RacerBaseDirector::QueryCtxPtr qctx(new QueryCtx);

  Query& q = qctx->getQuery();

  const GAtomSet& ints = in.getAtomSet();

//   std::cout << std::endl << "Parms: ";
//   std::copy(parms.begin(), parms.end(), std::ostream_iterator<Term>(std::cout, " "));
//   std::cout << std::endl << "Indvs: ";
//   std::copy(indv.begin(), indv.end(), std::ostream_iterator<Term>(std::cout, " "));
//   std::cout << std::endl << "Ints: ";
//   std::copy(ints.begin(), ints.end(), std::ostream_iterator<GAtom>(std::cout, " % "));
//   std::cout << std::endl;

  // parms[0] contains the KB URI constant
  std::string ontostr = parms[0].getUnquotedString();
  q.setOntology(ontostr);

  // parms[5] contains the query constant
  q.setQuery(parms[5]);

  // query individuals
  q.setIndividuals(indv);

  ///@todo read nspace from owl document
#if 1
  q.setNamespace("http://www.kr.tuwien.ac.at/staff/roman/shop#");
#else
  // get namespace from owl document
  OWLParser p("file:" + ontostr);
  p.parseNamespace(q);
#endif // "1"

  //
  // spread interpretation into the appropriate GAtomSet
  //

  GAtomSet pc;
  GAtomSet mc;
  GAtomSet pr;
  GAtomSet mr;

  for (GAtomSet::const_iterator it = ints.begin();
       it != ints.end(); it++)
    {
      const GAtom& a = *it;
      const Term pred = a.getArgument(0);

      if (pred == parms[1]) // plusC
	{
	  pc.insert(a);
	}
      else if (pred == parms[2]) // minusC
	{
	  mc.insert(a);
	}
      else if (pred == parms[3]) // plusR
	{
	  pr.insert(a);
	}
      else if (pred == parms[4]) // minusR
	{
	  mr.insert(a);
	}
      else
	{
	  // just ignore unknown stuff...
	  // std::cerr << "WTF?" << std::endl;
	}
    }

  q.setPlusConcept(pc);
  q.setMinusConcept(mc);
  q.setPlusRole(pr);
  //q.setMinusRole(mr); ///@todo minusR not implemented yet

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
RacerExtAtom::retrieve(const Interpretation& in,
		       const Tuple& parms,
		       std::vector<Tuple>& out) throw(PluginError)
{
  try
    {
      RacerBaseDirector::DirectorPtr dirs = getRetrievalDirectors();

      RacerBaseDirector::QueryCtxPtr qctx = setupQuery(in, parms, Tuple());

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
RacerExtAtom::query(const Interpretation& in,
		    const Tuple& parms,
		    Tuple& indv) throw(PluginError)
{
  try
    {
      RacerBaseDirector::DirectorPtr dirs = getQueryDirectors();

      RacerBaseDirector::QueryCtxPtr qctx = setupQuery(in, parms, indv);

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
{
  //
  // &racerC[kb,plusC,minusC,plusR,minusR,query](X)
  //

  setOutputArity(1);

  addInputConstant();  // query
  addInputPredicate(); // minusR
  addInputPredicate(); // plusR
  addInputPredicate(); // minusC
  addInputPredicate(); // plusC
  addInputConstant();  // kb URI
}

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
{
  //
  // &racerR[kb,plusC,minusC,plusR,minusR,query](X,Y)
  //

  setOutputArity(2);

  addInputConstant();  // query
  addInputPredicate(); // minusR
  addInputPredicate(); // plusR
  addInputPredicate(); // minusC
  addInputPredicate(); // plusC
  addInputConstant();  // kb URI
}

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
