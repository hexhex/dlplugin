/* dlvhex-dlplugin -- Integration of Answer-Set Programming and Description Logics.
 *
 * Copyright (C) 2005, 2006, 2007  Thomas Krennwallner
 * 
 * This file is part of dlvhex-dlplugin.
 *
 * dlvhex-dlplugin is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * dlvhex-dlplugin is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dlvhex-dlplugin; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


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

#include <dlvhex/Atom.h>
#include <dlvhex/Term.h>
#include <dlvhex/AtomSet.h>
#include <dlvhex/Error.h>
#include <dlvhex/PluginInterface.h>

#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

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


  /// request to open an OWL document
  typedef QueryDirector<RacerOpenOWLBuilder, RacerAnswerDriver> RacerOpenOWL;

  /// extend ABox by a given set of individuals/pairs
  typedef QueryDirector<RacerStateBuilder, RacerAnswerDriver> RacerConceptRolePM;

  
  template<class GetKBManager>
  RacerExtAtom<GetKBManager>::RacerExtAtom(std::iostream& s)
    : stream(s), getKBManager()
  { }


  template<class GetKBManager>
  void
  RacerExtAtom<GetKBManager>::retrieve(const PluginAtom::Query& query,
				       PluginAtom::Answer& answer) throw(PluginError)
  {
    try
      {
	QueryCtx::shared_pointer qctx(new QueryCtx(query, getKBManager()));

	QueryBaseDirector::shared_pointer dirs = getDirectors(qctx->getQuery());

	boost::posix_time::ptime start = boost::posix_time::microsec_clock::local_time();

	qctx = dirs->query(qctx);

	boost::posix_time::ptime end = boost::posix_time::microsec_clock::local_time();
      
	if (Registry::getVerbose() > 1)
	  {
	    boost::posix_time::time_duration diff = end - start;
	    std::cerr << "Runtime: " << diff << ' ' << qctx->getQuery() << std::endl;
	  }

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
  


  template<class GetKBManager>
  void
  RacerExtAtom<GetKBManager>::setupRacer(QueryCompositeDirector::shared_pointer& comp) const
  {
    if (!Registry::getUNA()) // only set UNA once
      {
	// turn on unique name assumption
	comp->add(new QueryDirector<RacerFunAdapterBuilder<RacerUNACmd>,
		  RacerIgnoreAnswer>(this->stream)
		  );
	
	Registry::setUNA(true);
      }
  }
  

  template<class GetKBManager>
  void
  RacerExtAtom<GetKBManager>::openOntology(const dlvhex::dl::Query& query,
					   QueryCompositeDirector::shared_pointer& comp) const
  {
    // check if Racer has an open KB with the name of the real URI of
    // the query's ontology, we can reuse it
    
    const std::string& kbname = query.getDLQuery()->getOntology()->getRealURI().getString();

    if (!getKBManager().isOpenKB(kbname))
      {
	// update opened KBs
	getKBManager().updateOpenKB();
  
	if (!getKBManager().isOpenKB(kbname)) // only open OWL after we updated the open KBs
	  {
	    comp->add(new RacerOpenOWL(this->stream));
	    
	    // import all referenced ontologies
	    comp->add(new QueryDirector<RacerFunAdapterBuilder<RacerImportOntologiesCmd>,
		      RacerIgnoreAnswer>(this->stream)
		      );
	  }
      }
  }


  template<class GetKBManager>
  void
  RacerExtAtom<GetKBManager>::increaseABox(const dlvhex::dl::Query& /* query */,
					   QueryCompositeDirector::shared_pointer& comp) const
  {
    // create a temporary ABox for the (state) command
    comp->add(new QueryDirector<RacerFunAdapterBuilder<RacerCloneABoxCmd>,
	      RacerIgnoreAnswer>(this->stream)
	      );
    
    // add concept and role assertions via (state) command
    comp->add(new RacerConceptRolePM(this->stream));
  }


  template <class GetKBManager, class GetCache>
  RacerCachingAtom<GetKBManager,GetCache>::RacerCachingAtom(std::iostream& s)
    : RacerExtAtom<GetKBManager>(s), getCache()
  { }


  template <class GetKBManager, class GetCache>
  QueryBaseDirector::shared_pointer
  RacerCachingAtom<GetKBManager,GetCache>::cacheQuery(QueryCompositeDirector::shared_pointer comp) const
  {
    // use the QueryCachingDirector as proxy for the QueryCompositeDirector
    return QueryBaseDirector::shared_pointer(new QueryCachingDirector(getCache(), comp));
  }




  template <class GetKBManager>
  RacerConsistentAtom<GetKBManager>::RacerConsistentAtom(std::iostream& s)
    : RacerExtAtom<GetKBManager>(s)
  {
    //
    // &dlConsistent[kb,plusC,minusC,plusR,minusR]()
    //
    
    this->setOutputArity(0);
    
    this->addInputConstant();  // kb URI
    this->addInputPredicate(); // plusC
    this->addInputPredicate(); // minusC
    this->addInputPredicate(); // plusR
    this->addInputPredicate(); // minusR
  }


  
  template <class GetKBManager>
  QueryBaseDirector::shared_pointer
  RacerConsistentAtom<GetKBManager>::getDirectors(const dlvhex::dl::Query& q) const
  {
    QueryCompositeDirector::shared_pointer comp(new QueryCompositeDirector(this->stream));
    
    this->setupRacer(comp);
    this->openOntology(q, comp);
    this->increaseABox(q, comp);

    // ask whether ABox is consistent
    comp->add
      (new QueryDirector<RacerFunAdapterBuilder<RacerABoxConsistentCmd>,RacerAnswerDriver>(this->stream)
       );

    return comp;
  }


  template <class GetKBManager, class GetCache>
  RacerConceptAtom<GetKBManager,GetCache>::RacerConceptAtom(std::iostream& s)
    : RacerCachingAtom<GetKBManager,GetCache>(s)
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
  

  template <class GetKBManager, class GetCache>
  QueryBaseDirector::shared_pointer
  RacerConceptAtom<GetKBManager,GetCache>::getDirectors(const dlvhex::dl::Query& query) const
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


  template <class GetKBManager, class GetCache>
  RacerRoleAtom<GetKBManager,GetCache>::RacerRoleAtom(std::iostream& s)
    : RacerCachingAtom<GetKBManager,GetCache>(s)
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


  template <class GetKBManager, class GetCache>
  QueryBaseDirector::shared_pointer
  RacerRoleAtom<GetKBManager,GetCache>::getDirectors(const dlvhex::dl::Query& query) const
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


  template <class GetKBManager, class GetCache>
  RacerDatatypeRoleAtom<GetKBManager,GetCache>::RacerDatatypeRoleAtom(std::iostream& s)
    : RacerCachingAtom<GetKBManager,GetCache>(s)
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


  template <class GetKBManager, class GetCache>
  void
  RacerDatatypeRoleAtom<GetKBManager,GetCache>::setupRacer(QueryCompositeDirector::shared_pointer& comp) const
  {
    RacerExtAtom<GetKBManager>::setupRacer(comp);
    
    if (!Registry::getDataSubstrateMirroring())
      {
	// enable data substrate mirroring
	comp->add(new QueryDirector<RacerFunAdapterBuilder<RacerDataSubstrateMirroringCmd>,
		  RacerIgnoreAnswer>(this->stream)
	  );
	
	Registry::setDataSubstrateMirroring(true);
      }
  }


  template <class GetKBManager, class GetCache>
  QueryBaseDirector::shared_pointer
  RacerDatatypeRoleAtom<GetKBManager,GetCache>::getDirectors(const dlvhex::dl::Query& query) const
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


  template <class GetKBManager, class GetCache>
  RacerCQAtom<GetKBManager,GetCache>::RacerCQAtom(std::iostream& s, unsigned n)
    : RacerCachingAtom<GetKBManager,GetCache>(s)
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
  

  template <class GetKBManager, class GetCache>
  QueryBaseDirector::shared_pointer
  RacerCQAtom<GetKBManager,GetCache>::getDirectors(const dlvhex::dl::Query& query) const
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


  template <class GetKBManager, class GetCache>
  RacerUCQAtom<GetKBManager,GetCache>::RacerUCQAtom(std::iostream& s, unsigned n)
    : RacerCachingAtom<GetKBManager,GetCache>(s)
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
  

  template <class GetKBManager, class GetCache>
  QueryBaseDirector::shared_pointer
  RacerUCQAtom<GetKBManager,GetCache>::getDirectors(const dlvhex::dl::Query& query) const
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


// Local Variables:
// mode: C++
// End:
