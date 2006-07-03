/* -*- C++ -*- */

/**
 * @file   Cache.h
 * @author Thomas Krennwallner
 * @date   Thu Jun  1 11:03:43 2006
 * 
 * @brief  
 * 
 * 
 */


#ifndef _CACHE_H
#define _CACHE_H

#include "RacerQuery.h"

#include <set>

namespace dlvhex {
namespace racer {

  /**
   * @brief Base class for caching classes.
   */
  class BaseCache
  {
  public:
    virtual
    ~BaseCache();

    /**
     * checks if there is a cached entry for query.
     *
     * @param query
     *
     * @return the cached QueryCtx, an empty QueryCtx otherwise.
     */
    virtual QueryCtx::shared_pointer
    cacheHit(const QueryCtx::shared_pointer& query) = 0;

    /** 
     * insert query into the cache.
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
    struct QueryCtxCompare
      : public std::binary_function<const QueryCtx::shared_pointer,
				    const QueryCtx::shared_pointer,
				    bool>
    {
      bool
      operator() (const QueryCtx::shared_pointer& a,
		  const QueryCtx::shared_pointer& b) const
      {
	return a->getQuery() < b->getQuery();
      }
    };

    /// caches QueryCtx::shared_pointer with help of a std::set
    typedef std::set<QueryCtx::shared_pointer, QueryCtxCompare> CacheSet;

    CacheSet cache;

  public:
    virtual
    ~Cache();

    virtual QueryCtx::shared_pointer
    cacheHit(const QueryCtx::shared_pointer&);

    virtual void
    insert(const QueryCtx::shared_pointer&);
  };


  /** 
   * For debugging purposes. 
   */
  class DebugCache : public Cache
  {
  public:
    virtual
    ~DebugCache();

    virtual QueryCtx::shared_pointer
    cacheHit(const QueryCtx::shared_pointer&);

    virtual void
    insert(const QueryCtx::shared_pointer&);
  };

} // namespace racer
} // namespace dlvhex

#endif /* _RACERDIRECTOR_H */
