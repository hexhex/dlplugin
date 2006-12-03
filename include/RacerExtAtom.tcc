/* -*- C++ -*- */

/**
 * @file   RacerExtAtom.tcc
 * @author Thomas Krennwallner
 * @date   Tue Aug 22 15:55:02 2006
 * 
 * @brief  External atoms for dlvhex.
 * 
 * 
 */


#ifndef _RACEREXTATOM_TCC
#define _RACEREXTATOM_TCC

#include "RacerBuilder.h"
#include "RacerAnswerDriver.h"
#include "Registry.h"
#include "QueryDirector.h"
#include "QueryCtx.h"
#include "Query.h"
#include "Cache.h"
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

namespace dlvhex {
namespace dl {
namespace racer {
  
  //
  // following typedefs are here for easy of use
  //
  
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


  template <class GetCache>
  RacerCachingAtom<GetCache>::RacerCachingAtom(std::iostream& s, RacerKBManager& k)
    : RacerExtAtom(s,k), getCache()
  { }


  template <class GetCache>
  QueryBaseDirector::shared_pointer
  RacerCachingAtom<GetCache>::cacheQuery(QueryCompositeDirector::shared_pointer comp) const
  {
    // use the QueryCachingDirector as proxy for the QueryCompositeDirector
    return QueryBaseDirector::shared_pointer(new QueryCachingDirector(getCache(), comp));
  }


  template <class GetCache>
  RacerConceptAtom<GetCache>::RacerConceptAtom(std::iostream& s, RacerKBManager& k)
    : RacerCachingAtom<GetCache>(s, k)
  {
    //
    // &dlC[kb,plusC,minusC,plusR,minusR,query](X)
    //

    this->setOutputArity(1);

    this->addInputConstant();  // kb URI
    this->addInputPredicate(); // plusC
    this->addInputPredicate(); // minusC
    this->addInputPredicate(); // plusR
    this->addInputPredicate(); // minusR
    this->addInputConstant();  // query
  }
  

  template <class GetCache>
  QueryBaseDirector::shared_pointer
  RacerConceptAtom<GetCache>::getDirectors(const dlvhex::dl::Query& query) const
  {
    const DLQuery::shared_pointer& dlq = query.getDLQuery();

    QueryCompositeDirector::shared_pointer comp(new QueryCompositeDirector(this->stream));
    
    this->setupRacer(comp);
    this->openOntology(query, comp);
    this->increaseABox(query, comp);
    
    if (dlq->isRetrieval()) // retrieval mode
      {
	comp->add(new RacerConceptQuery(this->stream));
      }
    else if (dlq->isBoolean()) // boolean query mode
      {
	comp->add(new RacerIsConceptQuery(this->stream));
      }
    else
      {
	throw PluginError("DLQuery has wrong query type, expected retrieval or boolean query");
      }

    return this->cacheQuery(comp);
  }


  template <class GetCache>
  RacerRoleAtom<GetCache>::RacerRoleAtom(std::iostream& s, RacerKBManager& k)
    : RacerCachingAtom<GetCache>(s,k)
  {
    //
    // &dlR[kb,plusC,minusC,plusR,minusR,query](X,Y)
    //
    
    this->setOutputArity(2);

    this->addInputConstant();  // kb URI
    this->addInputPredicate(); // plusC
    this->addInputPredicate(); // minusC
    this->addInputPredicate(); // plusR
    this->addInputPredicate(); // minusR
    this->addInputConstant();  // query
  }


  template <class GetCache>
  QueryBaseDirector::shared_pointer
  RacerRoleAtom<GetCache>::getDirectors(const dlvhex::dl::Query& query) const
  {
    const DLQuery::shared_pointer& dlq = query.getDLQuery();

    QueryCompositeDirector::shared_pointer comp(new QueryCompositeDirector(this->stream));
    
    this->setupRacer(comp);
    this->openOntology(query, comp);
    this->increaseABox(query, comp);
    
    if (dlq->isRetrieval()) // retrieval mode
      {
	comp->add(new RacerRoleQuery(this->stream));
      }
    else if (dlq->isBoolean()) // boolean query mode
      {
	comp->add(new RacerIsRoleQuery(this->stream));
      }
    else if (dlq->isMixed()) // pattern retrieval mode
      {
	comp->add(new RacerIndvFillersQuery(this->stream));
      }
    else
      {
	throw PluginError("DLQuery has wrong query type, expected retrieval, boolean or mixed query");
      }

    return this->cacheQuery(comp);
  }


  template <class GetCache>
  RacerDatatypeRoleAtom<GetCache>::RacerDatatypeRoleAtom(std::iostream& s, RacerKBManager& k)
    : RacerCachingAtom<GetCache>(s, k)
  {
    //
    // &dlDR[kb,plusC,minusC,plusR,minusR,query](X,Y)
    //
    
    this->setOutputArity(2);
    
    this->addInputConstant();  // kb URI
    this->addInputPredicate(); // plusC
    this->addInputPredicate(); // minusC
    this->addInputPredicate(); // plusR
    this->addInputPredicate(); // minusR
    this->addInputConstant();  // query
  }


  template <class GetCache>
  void
  RacerDatatypeRoleAtom<GetCache>::setupRacer(QueryCompositeDirector::shared_pointer& comp) const
  {
    RacerExtAtom::setupRacer(comp);
    
    if (!Registry::getDataSubstrateMirroring())
      {
	// enable data substrate mirroring
	comp->add(new QueryDirector<RacerFunAdapterBuilder<RacerDataSubstrateMirroringCmd>,
		  RacerIgnoreAnswer>(this->stream)
	  );
	
	Registry::setDataSubstrateMirroring(true);
      }
  }


  template <class GetCache>
  QueryBaseDirector::shared_pointer
  RacerDatatypeRoleAtom<GetCache>::getDirectors(const dlvhex::dl::Query& query) const
  {
    const DLQuery::shared_pointer& dlq = query.getDLQuery();
    
    QueryCompositeDirector::shared_pointer comp(new QueryCompositeDirector(this->stream));
    
    this->setupRacer(comp);
    this->openOntology(query, comp);
    
    // we don't have to increase the ABox here, we use retrieve-under-premise
    
    if (dlq->isRetrieval() || dlq->isMixed())
      {
	// pose datatype role query
	comp->add
	  (new QueryDirector<RacerAdapterBuilder<NRQLRetrieveUnderPremise<NRQLDatatypeBuilder> >,
	   RacerAnswerDriver>(this->stream)
	  );
      }
    else
      {
	throw PluginError("DLQuery has wrong query type, expected retrieval or mixed query");
      }
    
    return this->cacheQuery(comp);
  }


  template <class GetCache>
  RacerCQAtom<GetCache>::RacerCQAtom(std::iostream& s, RacerKBManager& k, unsigned n)
    : RacerCachingAtom<GetCache>(s,k)
  {
    //
    // &dlCQn[kb,plusC,minusC,plusR,minusR,query](X_1,...,X_n)
    //
    
    this->setOutputArity(n);
    
    this->addInputConstant();  // kb URI
    this->addInputPredicate(); // plusC
    this->addInputPredicate(); // minusC
    this->addInputPredicate(); // plusR
    this->addInputPredicate(); // minusR
    this->addInputConstant();  // query
  }
  

  template <class GetCache>
  QueryBaseDirector::shared_pointer
  RacerCQAtom<GetCache>::getDirectors(const dlvhex::dl::Query& query) const
  {
    QueryCompositeDirector::shared_pointer comp(new QueryCompositeDirector(this->stream));
    
    this->setupRacer(comp);
    this->openOntology(query, comp);
    
    // we don't have to increase the ABox here, we use retrieve-under-premise
    
    // pose a conjunctive query
    comp->add
      (new QueryDirector<RacerAdapterBuilder<NRQLRetrieveUnderPremise<NRQLConjunctionBuilder> >,
       RacerAnswerDriver>(this->stream)
      );
  
    ///@todo right now we don't cache conjunctive queries
    return comp;
  }


  template <class GetCache>
  RacerUCQAtom<GetCache>::RacerUCQAtom(std::iostream& s, RacerKBManager& k, unsigned n)
    : RacerCachingAtom<GetCache>(s,k)
  {
    //
    // &dlUCQn[kb,plusC,minusC,plusR,minusR,query](X_1,...,X_n)
    //
    
    this->setOutputArity(n);
    
    this->addInputConstant();  // kb URI
    this->addInputPredicate(); // plusC
    this->addInputPredicate(); // minusC
    this->addInputPredicate(); // plusR
    this->addInputPredicate(); // minusR
    this->addInputConstant();  // query
  }
  

  template <class GetCache>
  QueryBaseDirector::shared_pointer
  RacerUCQAtom<GetCache>::getDirectors(const dlvhex::dl::Query& query) const
  {
    QueryCompositeDirector::shared_pointer comp(new QueryCompositeDirector(this->stream));
    
    this->setupRacer(comp);
    this->openOntology(query, comp);
    
    // we don't have to increase the ABox here, we use retrieve-under-premise
    
    // pose a union of conjunctive queries
    comp->add
      (new QueryDirector<RacerAdapterBuilder<NRQLRetrieveUnderPremise<NRQLDisjunctionBuilder> >,
       RacerAnswerDriver>(this->stream)
      );
  
    ///@todo right now we don't cache conjunctive queries
    return comp;
  }


} // namespace racer
} // namespace dl
} // namespace dlvhex

#endif /* _RACEREXTATOM_TCC */
