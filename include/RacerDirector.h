/* -*- C++ -*- */

/**
 * @file   RacerDirector.h
 * @author Thomas Krennwallner
 * @date   Wed Jul  6 09:49:23 2005
 * 
 * @brief  Director classes for the communication with RACER.
 * 
 * 
 */

#ifndef _RACERDIRECTOR_H
#define _RACERDIRECTOR_H

#include <iostream>
#include <map>
#include <memory>

#include <dlvhex/Atom.h>
#include <dlvhex/Term.h>

#include "RacerError.h"
#include "RacerQuery.h"
#include "RacerParse.h"
#include "RacerBuilder.h"

namespace dlvhex {
namespace racer {

  /**
   * @brief Base class for the director classes.
   */
  class RacerBaseDirector
  {
  public:
    /// managed RacerBaseDirector
    typedef std::auto_ptr<RacerBaseDirector> DirectorPtr;
    /// managed QueryCtx
    typedef std::auto_ptr<QueryCtx> QueryCtxPtr;

  protected:
    RacerBaseDirector()
    { }

  public:
    virtual
    ~RacerBaseDirector()
    { }

    virtual QueryCtxPtr
    query(QueryCtxPtr) throw(RacerError) = 0;
  };


  /**
   * @brief Template Director for building RACER commands and parsing
   * their result.
   */
  template <class Builder, class Parser>
  class RacerDirector : public RacerBaseDirector
  {
  protected:
    Builder builder;
    Parser parser;

  public:
    explicit
    RacerDirector(std::iostream&);

    /**
     * @brief builds/parses RACER commands/answers using the Builder
     * and Parser members.
     *
     * @param qctx
     *
     * @return the QueryCtxPtr with the corresponding Answer to the
     * Query
     */
    virtual QueryCtxPtr
    query(QueryCtxPtr qctx) throw(RacerError);
  };



  /**
   * @brief A Composite Director for querying lists of RACER commands.
   */
  class RacerCompositeDirector : public RacerBaseDirector
  {
  protected:
    typedef std::vector<RacerBaseDirector*> DirectorList;

    DirectorList dirs;
    std::iostream& stream;

    virtual QueryCtxPtr
    handleInconsistency(QueryCtxPtr) = 0;

  public:
    RacerCompositeDirector(std::iostream&);

    virtual
    ~RacerCompositeDirector();

    virtual void
    add(RacerBaseDirector*);

    virtual QueryCtxPtr
    query(QueryCtxPtr qctx) throw(RacerError);
  };


  /**
   * @brief A Composite Director for RACER query commands.
   */
  class RacerQueryComposite : public RacerCompositeDirector
  {
  protected:
    virtual QueryCtxPtr
    handleInconsistency(QueryCtxPtr qctx);

  public:
    explicit
    RacerQueryComposite(std::iostream& s) : RacerCompositeDirector(s)
    { }
  };


  /**
   * @brief A Composite Director for RACER retrieval commands.
   */
  class RacerRetrieveComposite : public RacerCompositeDirector
  {
  public:
    typedef enum { INDIVIDUALS, RELATED } RetrievalType;

  protected:
    RetrievalType type;

    virtual QueryCtxPtr
    handleInconsistency(QueryCtxPtr qctx);

  public:
    explicit
    RacerRetrieveComposite(std::iostream& s, RetrievalType t = INDIVIDUALS)
      : RacerCompositeDirector(s),
	type(t)
    { }
  };


  /**
   * @brief Provides caching support for RACER Queries.
   */
  class RacerCachingDirector : public RacerBaseDirector
  {
  public:
    /// maps Term -> QueryCtx*
    typedef std::map<Term, QueryCtx*> RacerCache;

  protected:
    DirectorPtr director;
    RacerCache& cache;

    /// template method checks if found is really a cache hit
    virtual bool
    cacheHit(const QueryCtx& query, const QueryCtx& found) const = 0;

  public:
    explicit
    RacerCachingDirector(RacerCache&, DirectorPtr d = DirectorPtr());

    virtual
    ~RacerCachingDirector();

    /// performs query caching
    virtual QueryCtxPtr
    query(QueryCtxPtr) throw(RacerError);
  };


  /**
   * @brief Provides a CacheHit Algorithm for Boolean Queries.
   */
  class RacerBooleanCache : public RacerCachingDirector
  {
  protected:
    virtual bool
    cacheHit(const QueryCtx& query, const QueryCtx& found) const;

  public:
    explicit
    RacerBooleanCache(RacerCache& c, DirectorPtr d = DirectorPtr())
      : RacerCachingDirector(c, d)
    { }
  };


  /**
   * @brief Provides a CacheHit Algorithm for Retrieval Queries.
   */
  class RacerTermCache : public RacerCachingDirector
  {
  protected:
    virtual bool
    cacheHit(const QueryCtx& query, const QueryCtx& found) const;

  public:
    explicit
    RacerTermCache(RacerCache& c, DirectorPtr d = DirectorPtr())
      : RacerCachingDirector(c, d)
    { }
  };


  typedef RacerDirector<RacerNullBuilder, RacerNullParser> RacerNullDirector;
  typedef RacerDirector<RacerAllIndividualsBuilder, RacerAnswerList> RacerAllIndQuery;
  typedef RacerDirector<RacerUNABuilder, RacerIgnoreAnswer> RacerUNA;
  typedef RacerDirector<RacerTempABoxBuilder, RacerSimpleAnswer> RacerTempABox;
  typedef RacerDirector<RacerOpenOWLBuilder, RacerSimpleAnswer> RacerOpenOWL;
  typedef RacerDirector<RacerABoxConsistentBuilder, RacerBooleanAnswer> RacerABoxConsistent;
  typedef RacerDirector<RacerCheckABoxConsistencyOffBuilder, RacerIgnoreAnswer> RacerABoxConsistencyOff;

  typedef RacerDirector<RacerStateBuilder, RacerSimpleAnswer> RacerConceptRolePM;
  typedef RacerDirector<RacerConceptInstancesBuilder, RacerAnswerList> RacerConceptQuery;
  typedef RacerDirector<RacerRoleIndividualsBuilder, RacerAnswerPairList> RacerRoleQuery;
  typedef RacerDirector<RacerIsConceptMemberBuilder, RacerBooleanAnswer> RacerIsConceptQuery;
  typedef RacerDirector<RacerIsRoleMemberBuilder, RacerBooleanAnswer> RacerIsRoleQuery;
  typedef RacerDirector<RacerIndividualFillersBuilder, RacerAnswerList> RacerIndvFillersQuery;

} // namespace racer
} // namespace dlvhex


// include the implemantation of the templates
#include "RacerDirector.tcc"


#endif /* _RACERDIRECTOR_H */
