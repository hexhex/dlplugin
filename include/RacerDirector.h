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

    /**
     * query method implemented in the children of RacerBaseDirector.
     *
     * @param qctx
     */
    virtual QueryCtxPtr
    query(QueryCtxPtr qctx) throw(RacerError) = 0;
  };


  /**
   * @brief Template Director for building RACER commands and parsing
   * their result.
   */
  template <class Builder, class Parser>
  class RacerDirector : public RacerBaseDirector
  {
  protected:
    /// Builder object
    Builder builder;
    /// Parser object
    Parser parser;

  public:
    /**
     * Ctor.
     *
     * @param s use s to instantiate Builder and Parser
     */
    explicit
    RacerDirector(std::iostream& s);

    /**
     * @brief builds/parses RACER commands/answers using the Builder
     * and Parser members.
     *
     * @param qctx pass the appropriate QueryCtxPtr member to the
     * Builder/Parser.
     *
     * @return the QueryCtxPtr with the corresponding Answer to the
     * Query
     */
    virtual QueryCtxPtr
    query(QueryCtxPtr qctx) throw(RacerError);
  };



  /**
   * @brief A base composite director for querying lists of RACER commands.
   *
   * Provides a list of RacerBaseDirector and takes care of
   * inconsistency during execution of the RACER commands.
   */
  class RacerCompositeDirector : public RacerBaseDirector
  {
  protected:
    /// managed RacerBaseDirector pointers
    typedef std::vector<RacerBaseDirector*> DirectorList;

    /// holds a list of RacerBaseDirector objects
    DirectorList dirs;

    /// keep a reference to the iostream just in case we need to
    /// create a dedicated director
    std::iostream& stream;

    /**
     * handleInconsistency() implemented in the children of
     * RacerCompositeDirector.
     *
     * @param qctx
     *
     * @return the QueryCtxPtr with the corresponding Answer to the
     * Query
     */
    virtual QueryCtxPtr
    handleInconsistency(QueryCtxPtr qctx);

  public:
    /// Ctor
    explicit
    RacerCompositeDirector(std::iostream&);

    /**
     * Dtor deletes managed RacerBaseDirector objects.
     */
    virtual
    ~RacerCompositeDirector();

    /**
     * adds a new RacerBaseDirector to dirs.
     *
     * @param d add it to the managed list of RacerBaseDirector objects
     */
    virtual void
    add(RacerBaseDirector* d);

    /**
     * iterates through the list of RacerBaseDirectors and calls their
     * query() method. If RACERs ABox gets inconsistent during the
     * iteration query() calls handleInconsitency() in order to
     * generate the appropriate answer.
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
   * @brief Provides caching support for RACER Queries.
   *
   * @see Thomas Eiter, Giovambattista Ianni, Roman Schindlauer, and
   * Hans Tompits. Nonmonotonic Description Logic Programs:
   * Implementation and Experiments. In F. Baader and A. Voronkov,
   * editors, Proceedings 12th International Conference on Logic for
   * Programming Artificial Intelligence and Reasoning (LPAR 2004),
   * 3452 in LNCS, pages 511-517. Springer, 2005.
   */
  class RacerCachingDirector : public RacerBaseDirector
  {
  public:
    struct QueryCtxCompare : public std::binary_function<const QueryCtx*, const QueryCtx*, bool>
    {
      bool
      operator() (const QueryCtx* a, const QueryCtx* b) const
      {
	return a->getQuery() < b->getQuery();
      }
    };

    /// caches QueryCtx* with help of a std::set
    typedef std::set<QueryCtx*, QueryCtxCompare> RacerCache;

  protected:
    /// the underlying director, usually a RacerCompositeDirector
    DirectorPtr director;

    /// reference to the cache of QueryCtx objects
    RacerCache& cache;

    /**
     * checks if found is really a cache hit
     *
     * @param query
     * @param found
     *
     * @return true if found contains a reasonable answer for query,
     * false otherwise.
     */
    virtual bool
    cacheHit(const QueryCtx& query, const QueryCtx& found) const;

    /**
     * tries to find query in the cache.
     *
     * @param query
     *
     * @return the position of the cached QueryCtx*.
     */
    virtual RacerCache::iterator
    find(QueryCtx* query) const;

  public:
    explicit
    RacerCachingDirector(RacerCache&, DirectorPtr d);

    virtual
    ~RacerCachingDirector();

    /**
     * Tries to lookup the query in its cache and determines with help
     * of cacheHit() if its really a cache-hit. Otherwise it delegates
     * the querying to its underlying director and caches its QueryCtx
     * object.
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
   * @brief For debugging reasons.
   */
  class RacerDebugCachingDirector : public RacerCachingDirector
  {
  protected:
    virtual bool
    cacheHit(const QueryCtx& query, const QueryCtx& found) const;
 
    virtual RacerCache::iterator
    find(QueryCtx* query) const;

  public:
    explicit
    RacerDebugCachingDirector(RacerCache&, DirectorPtr d);
  };


  // following typedefs are here for easy of use

  /// builds nothing and parses nothing. Mainly for testing purposes.
  typedef RacerDirector<RacerNullBuilder, RacerNullParser> RacerNullDirector;
  /// requests all individuals and parses the corresponding answer
  typedef RacerDirector<RacerAllIndividualsBuilder, RacerParse> RacerAllIndQuery;
  /// turn on unique name assumption
  typedef RacerDirector<RacerUNABuilder, RacerIgnoreAnswer> RacerUNA;
  /// create a temporary ABox called DEFAULT
  typedef RacerDirector<RacerTempABoxBuilder, RacerParse> RacerTempABox;
  /// request to open an OWL document
  typedef RacerDirector<RacerOpenOWLBuilder, RacerParse> RacerOpenOWL;
  /// ask whether ABox is consistent
  typedef RacerDirector<RacerABoxConsistentBuilder, RacerParse> RacerABoxConsistent;
  ///
  typedef RacerDirector<RacerCheckABoxConsistencyOffBuilder, RacerIgnoreAnswer> RacerABoxConsistencyOff;

  /// extend ABox by a given set of individuals/pairs
  typedef RacerDirector<RacerStateBuilder, RacerParse> RacerConceptRolePM;
  /// request a list of individuals from a specified concept
  typedef RacerDirector<RacerConceptInstancesBuilder, RacerParse> RacerConceptQuery;
  /// request a list of pairs from a specified role
  typedef RacerDirector<RacerRoleIndividualsBuilder, RacerParse> RacerRoleQuery;
  /// ask whether a given individual is member of a specified concept
  typedef RacerDirector<RacerIsConceptMemberBuilder, RacerParse> RacerIsConceptQuery;
  /// ask whether a given pair is member of a specified role
  typedef RacerDirector<RacerIsRoleMemberBuilder, RacerParse> RacerIsRoleQuery;
  /// request a list of individuals which are fillers of a role for a
  /// specified individual
  typedef RacerDirector<RacerIndividualFillersBuilder, RacerParse> RacerIndvFillersQuery;
  /// request a list of datatype individuals which are fillers of a
  /// role for a specified individual
  typedef RacerDirector<RacerIndividualDatatypeFillersBuilder,RacerParse> RacerIndvDataFillersQuery;

} // namespace racer
} // namespace dlvhex


// include the implemantation of the templates
#include "RacerDirector.tcc"


#endif /* _RACERDIRECTOR_H */
