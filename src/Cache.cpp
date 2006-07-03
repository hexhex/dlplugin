/* -*- C++ -*- */

/**
 * @file   Cache.cpp
 * @author Thomas Krennwallner
 * @date   Mon Jul  3 11:10:38 2006
 * 
 * @brief  
 * 
 * 
 */


#include "Cache.h"


using namespace dlvhex::racer;


BaseCache::~BaseCache()
{
}


Cache::~Cache()
{
}

QueryCtx::shared_pointer
Cache::cacheHit(const QueryCtx::shared_pointer& query)
{
  CacheSet::const_iterator found = cache.find(query);

  if (found != cache.end())
    {
      Query::QueryType t = query->getQuery().getType();
      const Query& q1 = query->getQuery();
      const Query& q2 = (*found)->getQuery();

      bool isCacheHit = false;

      if (t == Query::Boolean || t == Query::RelatedBoolean)
	{
	  bool isPositive = (*found)->getAnswer().getAnswer();
	  
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
	  isCacheHit = q1.getInterpretation() == q2.getInterpretation();
	}

      // only return found if we've got a cache hit
      if (isCacheHit)
	{
	  return *found;
	}
      else
	{
	  // there is no cache hit
	  // -> invalidate cache entry and found QueryCtx::shared_pointer
	  cache.erase(found);
	}
    }

  return QueryCtx::shared_pointer(); // return empty shared_ptr
}

void
Cache::insert(const QueryCtx::shared_pointer& qctx)
{
  cache.insert(qctx);
}



DebugCache::~DebugCache()
{
}

QueryCtx::shared_pointer
DebugCache::cacheHit(const QueryCtx::shared_pointer& query)
{
  std::cerr << "-----" << std::endl;

  for (CacheSet::const_iterator it = cache.begin();
       it != cache.end(); it++)
    {
      std::cerr << "   " << (*it)->getQuery() << std::endl;
    }

  std::cerr << "q: " << query->getQuery();

  QueryCtx::shared_pointer found = DebugCache::cacheHit(query);

  if (found)
    {
      std::cerr << " found in cache and is a cache-hit";
    }
  else
    {
      std::cerr << std::endl;
    }

  return found;
}

void
DebugCache::insert(const QueryCtx::shared_pointer& qctx)
{
  Cache::insert(qctx);
}
