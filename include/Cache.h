/* -*- C++ -*- */

/**
 * @file   Cache.h
 * @author Thomas Krennwallner
 * @date   Thu Jun  1 11:03:43 2006
 * 
 * @brief  Various DL caches.
 * 
 * 
 */


#ifndef _CACHE_H
#define _CACHE_H

#include "QueryCtx.h"

#include <dlvhex/AtomSet.h>

#include <set>

#include <boost/ptr_container/indirect_fun.hpp>

namespace dlvhex {
namespace dl {

  /**
   * @brief Base class for caching classes.
   */
  class BaseCache
  {
  public:
    /// Dtor.
    virtual
    ~BaseCache()
    { }

    /**
     * checks if there is a cached entry for @a query.
     *
     * @param query
     *
     * @return the cached QueryCtx, an empty QueryCtx::shared_pointer otherwise.
     */
    virtual QueryCtx::shared_pointer
    cacheHit(const QueryCtx::shared_pointer& query) const = 0;

    /** 
     * insert @a query into the cache.
     * 
     * @param query 
     */
    virtual void
    insert(const QueryCtx::shared_pointer& query) = 0;
  };


  /** 
   * @brief The standard DL cache.
   *
   * @see Thomas Eiter, Giovambattista Ianni, Roman Schindlauer, and
   * Hans Tompits. Nonmonotonic Description Logic Programs:
   * Implementation and Experiments. In F. Baader and A. Voronkov,
   * editors, Proceedings 12th International Conference on Logic for
   * Programming Artificial Intelligence and Reasoning (LPAR 2004),
   * 3452 in LNCS, pages 511-517. Springer, 2005.
   */
  class Cache : public BaseCache
  {
  protected:
    struct IntCmp
    {
      bool
      operator() (const QueryCtx::shared_pointer& q1,
		  const QueryCtx::shared_pointer& q2) const
      {
	const AtomSet& a1 = q1->getQuery().getProjectedInterpretation();
	const AtomSet& a2 = q2->getQuery().getProjectedInterpretation();

	// we don't have to consider the Query component, since all
	// members in a CacheSet have the same DLQuery
	return a1 < a2;
      }
    };

    /// caches QueryCtx::shared_pointer with help of a std::set
    typedef std::set<QueryCtx::shared_pointer,IntCmp> CacheSet;

    /// maps dl-queries to sets of QueryCtxen
    typedef std::map<DLQuery::shared_pointer, CacheSet,
		     boost::indirect_fun<std::less<DLQuery> > > QueryAnswerMap;

    /// the cache
    QueryAnswerMap cacheMap;

    virtual const CacheSet*
    find(const QueryCtx::shared_pointer& q) const;

    virtual QueryCtx::shared_pointer
    isValid(const QueryCtx::shared_pointer& q, const CacheSet& f) const;

  public:
    virtual QueryCtx::shared_pointer
    cacheHit(const QueryCtx::shared_pointer& query) const;

    virtual void
    insert(const QueryCtx::shared_pointer& query);
  };


  /** 
   * For debugging purposes. 
   */
  class DebugCache : public Cache
  {
  public:
    virtual QueryCtx::shared_pointer
    cacheHit(const QueryCtx::shared_pointer& query) const;
  };


  class NullCache : public BaseCache
  {
  public:
    /**
     * @return an empty QueryCtx::shared_pointer.
     */
    QueryCtx::shared_pointer
    cacheHit(const QueryCtx::shared_pointer& /* query */) const
    {
      return QueryCtx::shared_pointer();
    }

    /** 
     * noop.
     */
    void
    insert(const QueryCtx::shared_pointer& /* query */)
    { }
  };

} // namespace dl
} // namespace dlvhex

#endif /* _RACERDIRECTOR_H */
