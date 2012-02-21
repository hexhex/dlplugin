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

#include <dlvhex2/ComfortPluginInterface.h>

#include <set>
#include <iosfwd>

#include <boost/ptr_container/indirect_fun.hpp>

namespace dlvhex {
namespace dl {


  ///@brief cache statistics
  class CacheStats
  {
  protected:
    unsigned m_hits;
    unsigned m_miss;
    unsigned m_dlqno;
    unsigned m_qctxno;

  public:
    /// default ctor
    CacheStats()
      : m_hits(0), m_miss(0), m_dlqno(0), m_qctxno(0)
    { }

    unsigned
    hits() const
    { return this->m_hits; }

    void
    hits(int n)
    { this->m_hits += n; }

    unsigned
    miss() const
    { return this->m_miss; }

    void
    miss(int n)
    { this->m_miss += n; }

    unsigned
    dlqno() const
    { return this->m_dlqno; }

    void
    dlqno(int n)
    { this->m_dlqno += n; }

    unsigned
    qctxno() const
    { return this->m_qctxno; }

    void
    qctxno(int n)
    { this->m_qctxno += n; }

  };

  /** 
   * Output the content of a CacheStats object @a cs.
   * 
   * @param os 
   * @param cs 
   * 
   * @return @a os
   */
  inline std::ostream&
  operator<< (std::ostream& os, const CacheStats& cs)
  {
    return os << "Cache hits: " << cs.hits() << std::endl
	      << "Cache miss: " << cs.miss() << std::endl
	      << "Number of cached dl-queries: " << cs.dlqno() << std::endl
	      << "Total number of cache queries: " << cs.qctxno() << std::endl;
  }


  /**
   * @brief Base class for caching classes.
   */
  class BaseCache
  {
  protected:
    /// cache statistics
    mutable CacheStats* stats;

  public:
    /// Ctor
    explicit
    BaseCache(CacheStats& s)
      : stats(&s)
    { }

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
    /// @brief we order the QueryCtx'en in the CacheSet accourding to
    /// their interpretations
    struct IntCmp
    {
      bool
      operator() (const QueryCtx::shared_pointer& q1,
		  const QueryCtx::shared_pointer& q2) const
      {
	const ComfortInterpretation& a1 = q1->getQuery().getProjectedInterpretation();
	const ComfortInterpretation& a2 = q2->getQuery().getProjectedInterpretation();

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
    /// ctor
    explicit
    Cache(CacheStats& s)
      : BaseCache(s),
	cacheMap()
    { }

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
    /// ctor
    explicit
    DebugCache(CacheStats& s)
      : Cache(s)
    { }

    virtual QueryCtx::shared_pointer
    cacheHit(const QueryCtx::shared_pointer& query) const;
  };


  ///@brief does not cache anything at all.
  class NullCache : public BaseCache
  {
  public:
    /// ctor
    explicit
    NullCache(CacheStats& s)
      : BaseCache(s)
    { }

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


// Local Variables:
// mode: C++
// End:
