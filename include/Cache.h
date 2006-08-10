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

#include <set>

#include <boost/ptr_container/indirect_fun.hpp>

namespace dlvhex {
namespace racer {

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
    cacheHit(const QueryCtx::shared_pointer& query) = 0;

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
    /// caches QueryCtx::shared_pointer with help of a std::set
    typedef std::set<QueryCtx::shared_pointer,
		     boost::indirect_fun<std::less<QueryCtx> > > CacheSet;

    /// the cache
    CacheSet cache;

    virtual QueryCtx::shared_pointer
    find(const QueryCtx::shared_pointer& q);

    virtual bool
    isValid(const QueryCtx::shared_pointer& q, const QueryCtx::shared_pointer& f);

  public:
    virtual QueryCtx::shared_pointer
    cacheHit(const QueryCtx::shared_pointer& query);

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
    cacheHit(const QueryCtx::shared_pointer& query);
  };

} // namespace racer
} // namespace dlvhex

#endif /* _RACERDIRECTOR_H */
