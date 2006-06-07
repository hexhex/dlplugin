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
  typedef std::set<QueryCtx::shared_pointer, QueryCtxCompare> Cache;

} // namespace racer
} // namespace dlvhex

#endif /* _RACERDIRECTOR_H */
