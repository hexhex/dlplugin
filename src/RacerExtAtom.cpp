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

/// builds nothing and parses nothing. Mainly for testing purposes.
typedef QueryDirector<RacerNullBuilder, RacerNullParser> RacerNullDirector;
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

QueryCompositeDirector::shared_pointer
RacerExtAtom::getComposite(const dlvhex::dl::Query& query) const
{
  QueryCompositeDirector::shared_pointer comp(new QueryCompositeDirector(stream));

  if (!Registry::getUNA()) // only set UNA once
    {
      // turn on unique name assumption
      comp->add(new QueryDirector<RacerFunAdapterBuilder<RacerUNACmd>,
		RacerIgnoreAnswer>(stream)
	);

      Registry::setUNA(true);
    }

  if (Registry::getOpenURI() != query.getOntology()->getURI()) // only load ontology if its new
    {
      comp->add(new RacerOpenOWL(stream));

      // import all referenced ontologies
      comp->add(new QueryDirector<RacerFunAdapterBuilder<RacerImportOntologiesCmd>,
		RacerIgnoreAnswer>(stream)
	);

      Registry::setOpenURI(query.getOntology()->getURI());
    }

  // create a temporary ABox called DEFAULT
  comp->add(new QueryDirector<RacerFunAdapterBuilder<RacerCloneABoxCmd>,
	    RacerIgnoreAnswer>(stream)
    );

  comp->add(new RacerConceptRolePM(stream));

  return comp;
}


RacerCachingAtom::RacerCachingAtom(std::iostream& s, BaseCache& c)
  : RacerExtAtom(s),
    cache(c)
{ }

QueryBaseDirector::shared_pointer
RacerCachingAtom::getCachedDirectors(const dlvhex::dl::Query& q, QueryBaseDirector* cmd) const
{
  QueryCompositeDirector::shared_pointer comp = getComposite(q);

  comp->add(cmd); // the actual command to send

  unsigned level = Registry::getVerbose();

  if (level == 0) // no caching
    {
      return comp;
    }
  else // default action is query caching
    {
      return QueryBaseDirector::shared_pointer(new QueryCachingDirector
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

QueryBaseDirector::shared_pointer
RacerConceptAtom::getDirectors(const dlvhex::dl::Query& query) const
{
  const DLQuery& dlq = query.getDLQuery();

  if (dlq.isRetrieval()) // retrieval mode
    {
      return getCachedDirectors(query, new RacerConceptQuery(stream));
    }
  else if (dlq.isBoolean()) // boolean query mode
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

QueryBaseDirector::shared_pointer
RacerRoleAtom::getDirectors(const dlvhex::dl::Query& query) const
{
  const DLQuery& dlq = query.getDLQuery();

  if (dlq.isRetrieval()) // retrieval mode
    {
      return getCachedDirectors(query, new RacerRoleQuery(stream));
    }
  else if (dlq.isBoolean()) // boolean query mode
    {
      return getCachedDirectors(query, new RacerIsRoleQuery(stream));
    }
  else if (dlq.isMixed()) // pattern retrieval mode
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

QueryBaseDirector::shared_pointer
RacerConsistentAtom::getDirectors(const dlvhex::dl::Query& q) const
{
  QueryCompositeDirector::shared_pointer comp = getComposite(q);

  // ask whether ABox is consistent
  comp->add(new QueryDirector<RacerFunAdapterBuilder<RacerABoxConsistentCmd>,
	    RacerAnswerDriver>(stream)
    );

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

QueryBaseDirector::shared_pointer
RacerDatatypeRoleAtom::getDirectors(const dlvhex::dl::Query& query) const
{
  const DLQuery& dlq = query.getDLQuery();

  if (dlq.isRetrieval() || dlq.isMixed())
    {
      ///@todo this is kind of a hack, maybe we should unify this stuff...
      QueryCompositeDirector* dir = new QueryCompositeDirector(stream);

      if (!Registry::getDataSubstrateMirroring())
	{
	  // enable data substrate mirroring
	  dir->add(new QueryDirector<RacerFunAdapterBuilder<RacerDataSubstrateMirroringCmd>,
		   RacerIgnoreAnswer>(stream)
		   );

	  Registry::setDataSubstrateMirroring(true);
	}

      // pose datatype role query
      dir->add
	(new QueryDirector<RacerAdapterBuilder<NRQLRetrieve<NRQLDatatypeBuilder> >,
	 RacerAnswerDriver>(stream)
	);

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

QueryBaseDirector::shared_pointer
RacerCQAtom::getDirectors(const dlvhex::dl::Query& query) const
{
  ///@todo right now we don't cache conjunctive queries

  QueryCompositeDirector::shared_pointer comp = getComposite(query);

  // pose a conjunctive query
  comp->add(new QueryDirector<RacerAdapterBuilder<NRQLRetrieve<NRQLConjunctionBuilder> >,
	    RacerAnswerDriver>(stream)
    );

  return comp;
}
