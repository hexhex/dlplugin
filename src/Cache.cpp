/* -*- C++ -*- */

/**
 * @file   Cache.cpp
 * @author Thomas Krennwallner
 * @date   Mon Jul  3 11:10:38 2006
 * 
 * @brief  Various DL caches.
 * 
 * 
 */


#include "Cache.h"
#include "QueryCtx.h"
#include "Query.h"
#include "Answer.h"

#include <iterator>
#include <boost/iterator/indirect_iterator.hpp>

using namespace dlvhex::racer;


BaseCache::~BaseCache()
{ }


QueryCtx::shared_pointer
Cache::find(const QueryCtx::shared_pointer& q)
{
  CacheSet::const_iterator found = cache.find(q);

  if (found != cache.end())
    {
      return *found;
    }
  
  return QueryCtx::shared_pointer();
}


bool
Cache::isValid(const QueryCtx::shared_pointer& query, const QueryCtx::shared_pointer& found)
{
  const Query& q1 = query->getQuery();
  const Query& q2 = found->getQuery();

  bool isCacheHit = false;

  if (q1.getDLQuery().isBoolean())
    {
      bool isPositive = found->getAnswer().getAnswer();
	  
      if (isPositive)
	{
	  isCacheHit = q2.isSubseteq(q1);
	}
      else // check if found is a superset of query
	{
	  isCacheHit = q2.isSuperseteq(q1);
	}
    }
  else // retrieval modes
    {
      // is the set of ints in query equal to the set of ints in found?
      isCacheHit = q1.getProjectedInterpretation() == q2.getProjectedInterpretation();
    }

  if (!isCacheHit)
    {
      // there is no cache hit
      // -> invalidate cache entry and found QueryCtx::shared_pointer
      cache.erase(found);
    }

  return isCacheHit;
}


QueryCtx::shared_pointer
Cache::cacheHit(const QueryCtx::shared_pointer& query)
{
  QueryCtx::shared_pointer found = find(query);

  if (found && isValid(query, found))
    {
      return found;
    }

  return QueryCtx::shared_pointer(); // return empty shared_ptr
}


void
Cache::insert(const QueryCtx::shared_pointer& qctx)
{
  cache.insert(qctx);
}


QueryCtx::shared_pointer
DebugCache::cacheHit(const QueryCtx::shared_pointer& query)
{
  std::cerr << "===== cache content:" << std::endl;

  std::transform(boost::make_indirect_iterator(cache.begin()),
		 boost::make_indirect_iterator(cache.end()),
		 std::ostream_iterator<Query>(std::cerr, "\n"),
		 std::mem_fun_ref(&QueryCtx::getQuery)
		 );

  std::cerr << "----- now looking for: " << query->getQuery() << std::endl;

  QueryCtx::shared_pointer found = Cache::find(query);

  if (found)
    {
      std::cerr << "----- found in cache: " << found->getQuery() << std::endl;

      if (Cache::isValid(query, found))
	{
	  std::cerr << "===== cache-hit" << std::endl;
	  return found;
	}
      else
	{
	  std::cerr << "===== NOT a cache-hit" << std::endl;
	}
    }
  else
    {
      std::cerr << "===== NOT found in cache" << std::endl;
    }

  return QueryCtx::shared_pointer();
}
