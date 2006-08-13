/* -*- C++ -*- */

/**
 * @file   QueryDirector.h
 * @author Thomas Krennwallner
 * @date   Wed Jul  6 09:49:23 2005
 * 
 * @brief  Director classes for the communication with a dl engine.
 * 
 * 
 */

#ifndef _QUERYDIRECTOR_H
#define _QUERYDIRECTOR_H

#include "QueryCtx.h"
#include "DLError.h"
#include "Cache.h"

#include <iosfwd>

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/shared_ptr.hpp>

namespace dlvhex {
namespace dl {

  /**
   * @brief Base class for the director classes.
   */
  class QueryBaseDirector
  {
  public:
    virtual
    ~QueryBaseDirector()
    { }

    /**
     * query method implemented in the children of QueryBaseDirector.
     *
     * @param qctx
     */
    virtual QueryCtx::shared_pointer
    query(QueryCtx::shared_pointer qctx) throw(DLError) = 0;

    typedef boost::shared_ptr<QueryBaseDirector> shared_pointer; 
  };


  /**
   * @brief Template Director for building RACER commands and parsing
   * their result.
   */
  template <class Builder, class Parser>
  class QueryDirector : public QueryBaseDirector
  {
  private:
    /// Builder object
    Builder builder;
    /// Parser object
    Parser parser;

  public:
    /**
     * Ctor.
     *
     * @param s use @a s to instantiate Builder and Parser
     */
    explicit
    QueryDirector(std::iostream& s);

    /**
     * @brief builds/parses RACER commands/answers using the Builder
     * and Parser members.
     *
     * @param qctx pass the appropriate QueryCtx::shared_pointer
     * member to the Builder/Parser.
     *
     * @return the QueryCtx::shared_pointer with the corresponding
     * Answer to the Query found in @a qctx
     */
    virtual QueryCtx::shared_pointer
    query(QueryCtx::shared_pointer qctx) throw(DLError);
  };



  /**
   * @brief A base composite director for querying lists of RACER
   * commands.
   *
   * Provides a list of QueryBaseDirector and takes care of
   * inconsistency during execution of the RACER commands.
   */
  class QueryCompositeDirector : public QueryBaseDirector
  {
  private:
    /// holds a list of QueryBaseDirector objects
    boost::ptr_vector<QueryBaseDirector> dirs;

    /// keep a reference to the iostream just in case we need to
    /// create a dedicated director
    std::iostream& stream;

    /**
     * If @a qctx leads to inconsistency, this method is called to
     * handle that case.
     *
     * @param qctx
     *
     * @return the QueryCtx::shared_pointer with the appropriate
     * Answer to the Query found in @a qctx.
     */
    virtual QueryCtx::shared_pointer
    handleInconsistency(QueryCtx::shared_pointer qctx);

  public:
    /// Ctor
    explicit
    QueryCompositeDirector(std::iostream&);

    /**
     * adds a new QueryBaseDirector to dirs.
     *
     * @param d add it to #dirs
     */
    virtual void
    add(QueryBaseDirector* d);

    /**
     * iterates through the list of QueryBaseDirectors and calls their
     * query() method. If RACERs ABox gets inconsistent during the
     * iteration query() calls handleInconsitency() in order to
     * generate the appropriate answer.
     *
     * @param qctx
     *
     * @return the QueryCtx::shared_pointer with the corresponding
     * Answer to the Query
     */
    virtual QueryCtx::shared_pointer
    query(QueryCtx::shared_pointer qctx) throw(DLError);

    typedef boost::shared_ptr<QueryCompositeDirector> shared_pointer;
  };


  /**
   * @brief Provides caching support for RACER Queries.
   */
  class QueryCachingDirector : public QueryBaseDirector
  {
  private:
    /// the underlying director, usually a QueryCompositeDirector
    QueryBaseDirector::shared_pointer director;

    /// reference to the cache of QueryCtx objects
    BaseCache& cache;

  public:
    /** 
     * Ctor.
     * 
     * @param c the cache
     * @param d delegation director
     */
    QueryCachingDirector(BaseCache& c, QueryBaseDirector::shared_pointer d);

    /**
     * Tries to lookup the query @a qctx in its cache and determines
     * BaseCache::cacheHit() if its really a cache-hit. Otherwise it
     * delegates the querying to its underlying @a director and caches
     * the new QueryCtx object.
     *
     * @param qctx
     *
     * @return the QueryCtx::shared_pointer with the corresponding
     * Answer to the Query
     */
    virtual QueryCtx::shared_pointer
    query(QueryCtx::shared_pointer qctx) throw(DLError);
  };


} // namespace dl
} // namespace dlvhex


// include the implemantation of the templates
#include "QueryDirector.tcc"


#endif /* _QUERYDIRECTOR_H */
