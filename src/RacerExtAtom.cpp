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
/// request a list of individuals from a specified concept
typedef QueryDirector<RacerConceptInstancesBuilder, RacerAnswerDriver> RacerConceptQuery;
/// request a list of pairs from a specified role
typedef QueryDirector<RacerRoleIndividualsBuilder, RacerAnswerDriver> RacerRoleQuery;
/// ask whether a given individual is member of a specified concept
typedef QueryDirector<RacerIsConceptMemberBuilder, RacerAnswerDriver> RacerIsConceptQuery;
/// ask whether a given pair is member of a specified role
typedef QueryDirector<RacerIsRoleMemberBuilder, RacerAnswerDriver> RacerIsRoleQuery;
/// request a list of individuals which are fillers of a role for a
/// specified individual
typedef QueryDirector<RacerIndividualFillersBuilder, RacerAnswerDriver> RacerIndvFillersQuery;


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

  const KBManager::KBList& v = kbManager.getOpenKB();

  std::vector<std::string>::const_iterator f = std::find(v.begin(), v.end(),
							 query.getOntology()->getRealURI());
  
  if (f == v.end())
    {
      comp->add(new RacerOpenOWL(stream));

      // import all referenced ontologies
      comp->add(new QueryDirector<RacerFunAdapterBuilder<RacerImportOntologiesCmd>,
		RacerIgnoreAnswer>(stream)
	);
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


RacerCachingAtom::RacerCachingAtom(std::iostream& s, RacerKBManager& k, BaseCache& c)
  : RacerExtAtom(s,k),
    cache(c)
{ }

QueryBaseDirector::shared_pointer
RacerCachingAtom::cacheQuery(QueryCompositeDirector::shared_pointer comp) const
{
  unsigned level = Registry::getVerbose();

  if (level == 0) // no caching
    {
      return comp;
    }
  else // default action is query caching
    {
      return QueryBaseDirector::shared_pointer(new QueryCachingDirector(this->cache, comp));
    }
}



RacerConceptAtom::RacerConceptAtom(std::iostream& s, RacerKBManager& k, BaseCache& c)
  : RacerCachingAtom(s, k, c)
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

QueryBaseDirector::shared_pointer
RacerConceptAtom::getDirectors(const dlvhex::dl::Query& query) const
{
  const DLQuery& dlq = query.getDLQuery();

  QueryCompositeDirector::shared_pointer comp(new QueryCompositeDirector(stream));

  setupRacer(comp);
  openOntology(query, comp);
  increaseABox(query, comp);

  if (dlq.isRetrieval()) // retrieval mode
    {
      comp->add(new RacerConceptQuery(stream));
    }
  else if (dlq.isBoolean()) // boolean query mode
    {
      comp->add(new RacerIsConceptQuery(stream));
    }
  else
    {
      throw PluginError("Wrong query type");
    }

  return cacheQuery(comp);
}


RacerRoleAtom::RacerRoleAtom(std::iostream& s, RacerKBManager& k, BaseCache& c)
  : RacerCachingAtom(s,k,c)
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

QueryBaseDirector::shared_pointer
RacerRoleAtom::getDirectors(const dlvhex::dl::Query& query) const
{
  const DLQuery& dlq = query.getDLQuery();

  QueryCompositeDirector::shared_pointer comp(new QueryCompositeDirector(stream));

  setupRacer(comp);
  openOntology(query, comp);
  increaseABox(query, comp);

  if (dlq.isRetrieval()) // retrieval mode
    {
      comp->add(new RacerRoleQuery(stream));
    }
  else if (dlq.isBoolean()) // boolean query mode
    {
      comp->add(new RacerIsRoleQuery(stream));
    }
  else if (dlq.isMixed()) // pattern retrieval mode
    {
      comp->add(new RacerIndvFillersQuery(stream));
    }
  else
    {
      throw PluginError("Wrong query type");
    }

  return cacheQuery(comp);
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




RacerDatatypeRoleAtom::RacerDatatypeRoleAtom(std::iostream& s, RacerKBManager& k, BaseCache& c)
  : RacerCachingAtom(s,k,c)
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

void
RacerDatatypeRoleAtom::setupRacer(QueryCompositeDirector::shared_pointer& comp) const
{
  RacerExtAtom::setupRacer(comp);
  
  if (!Registry::getDataSubstrateMirroring())
    {
      // enable data substrate mirroring
      comp->add(new QueryDirector<RacerFunAdapterBuilder<RacerDataSubstrateMirroringCmd>,
		RacerIgnoreAnswer>(stream)
	);
      
      Registry::setDataSubstrateMirroring(true);
    }
}


QueryBaseDirector::shared_pointer
RacerDatatypeRoleAtom::getDirectors(const dlvhex::dl::Query& query) const
{
  const DLQuery& dlq = query.getDLQuery();

  QueryCompositeDirector::shared_pointer comp(new QueryCompositeDirector(stream));

  setupRacer(comp);
  openOntology(query, comp);

  // we don't have to increase the ABox here, we use retrieve-under-premise

  if (dlq.isRetrieval() || dlq.isMixed())
    {
      // pose datatype role query
      comp->add
	(new QueryDirector<RacerAdapterBuilder<NRQLRetrieveUnderPremise<NRQLDatatypeBuilder> >,
	 RacerAnswerDriver>(stream)
	 );
    }
  else
    {
      throw PluginError("Wrong query type");
    }

  return cacheQuery(comp);
}




RacerCQAtom::RacerCQAtom(std::iostream& s, RacerKBManager& k, BaseCache& c, unsigned n)
  : RacerCachingAtom(s,k,c)
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

QueryBaseDirector::shared_pointer
RacerCQAtom::getDirectors(const dlvhex::dl::Query& query) const
{
  QueryCompositeDirector::shared_pointer comp(new QueryCompositeDirector(stream));

  setupRacer(comp);
  openOntology(query, comp);

  // we don't have to increase the ABox here, we use retrieve-under-premise

  // pose a conjunctive query
  comp->add
    (new QueryDirector<RacerAdapterBuilder<NRQLRetrieveUnderPremise<NRQLConjunctionBuilder> >,
     RacerAnswerDriver>(stream)
    );

  ///@todo right now we don't cache conjunctive queries
  return comp;
}
