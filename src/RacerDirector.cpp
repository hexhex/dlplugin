/* -*- C++ -*- */

/**
 * @file   RacerDirector.cpp
 * @author Thomas Krennwallner
 * @date   Wed Aug 24 09:45:18 2005
 * 
 * @brief  Caching Director class for the communication with RACER.
 * 
 * 
 */

#include "RacerDirector.h"

#include "RacerError.h"
#include "RacerQuery.h"

#include <memory>

using namespace dlvex_racer;


RacerCachingDirector::RacerCachingDirector(RacerCache& c, DirectorPtr d)
  : RacerBaseDirector(d),
    cache(c)
{ }

RacerCachingDirector::~RacerCachingDirector()
{ }

RacerBaseDirector::QueryCtxPtr
RacerCachingDirector::query(QueryCtxPtr qctx) throw(RacerError)
{
  if (director.get() != 0)
    {
      Query& query = qctx->getQuery();
      const GAtom& q = query.getQuery();
      
      // is query cached?
      RacerCache::iterator it = cache.find(q);

      if (it != cache.end())
	{
	  QueryCtx* found = (*it).second;
	  
	  if (cacheHit(*qctx, *found))
	    {
	      // delete qctx pointer and overwrite with found pointer
	      qctx.reset(found);
	      return qctx;
	    }
	  
	  // there is no cache hit
	  // -> invalidate cache entry and delete found QueryCtx
	  cache.erase(it);
	  delete found;
	}
      
      // ask the director chain and add qctx pointer to the cache 
      qctx = director->query(qctx);
      cache[q] = qctx.get();
    }

  return qctx;
}


RacerTermCache::RacerTermCache(RacerCache& c, DirectorPtr d)
  : RacerCachingDirector(c, d)
{ }

RacerTermCache::~RacerTermCache()
{ }

bool
RacerTermCache::cacheHit(const QueryCtx& query, const QueryCtx& found) const
{
  // is query equal to the cached query, i.e., is the set of ints in
  // query equal to the set of ints in found?
  return query.getQuery() == found.getQuery();
}


RacerBooleanCache::RacerBooleanCache(RacerCache& c, DirectorPtr d)
  : RacerCachingDirector(c, d)
{ }

RacerBooleanCache::~RacerBooleanCache()
{ }

bool
RacerBooleanCache::cacheHit(const QueryCtx& query, const QueryCtx& found) const
{
  Query& q1 = query.getQuery();
  Query& q2 = found.getQuery();

  bool isPositive = found.getAnswer().getAnswer();

  // is query not equal to the cached query, i.e., is the set of ints
  // in query not equal to the set of ints in found?
  if (q1 != q2)
    {
      // if we've got a positive answer check if found is a proper
      // subset of query
      if (isPositive)
	{
	  return q2.isSubset(q1);
	}
      else // check if found is a proper superset of query
	{
	  return q2.isSuperset(q1);
	}
    }

  // query == found
  return true;
}
