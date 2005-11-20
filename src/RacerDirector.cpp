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

using namespace dlvhex::racer;


RacerCompositeDirector::RacerCompositeDirector(std::iostream& s)
  : RacerBaseDirector(),
    stream(s)
{ }

RacerCompositeDirector::~RacerCompositeDirector()
{
  for (DirectorList::iterator it = dirs.begin(); it != dirs.end(); it++)
    {
      delete (*it);
    }
}

void
RacerCompositeDirector::add(RacerBaseDirector* d)
{
  if (d)
    {
      this->dirs.push_back(d);
    }
}


RacerBaseDirector::QueryCtxPtr
RacerCompositeDirector::query(QueryCtxPtr qctx) throw(RacerError)
{
  for (DirectorList::iterator it = dirs.begin(); it != dirs.end(); it++)
    {
      qctx = (*it)->query(qctx);

      if (qctx->getAnswer().getIncoherent())
	{
	  return handleInconsistency(qctx);
	}
    }

  return qctx;
}



RacerBaseDirector::QueryCtxPtr
RacerQueryComposite::handleInconsistency(QueryCtxPtr qctx)
{
  // querying is trivial now -> true
  qctx->getAnswer().setAnswer(true);
  qctx->getAnswer().setTuples(std::vector<Tuple>());

  return qctx;
}



RacerBaseDirector::QueryCtxPtr
RacerRetrieveComposite::handleInconsistency(QueryCtxPtr qctx)
{
  // just get all individuals
  RacerAllIndQuery all(stream);
  qctx = all.query(qctx);

  // check if we need to generate all possible pairs
  if (type == RELATED)
    {
      const std::vector<Tuple>* tuples = qctx->getAnswer().getTuples();
      std::vector<Tuple> pairs;

      for (std::vector<Tuple>::const_iterator it1 = tuples->begin();
	   it1 != tuples->end(); it1++)
	{
	  for (std::vector<Tuple>::const_iterator it2 = tuples->begin();
	       it2 != tuples->end(); it2++)
	    {
	      Tuple t;
	      t.push_back((*it1)[0]);	 
	      t.push_back((*it2)[0]);
	      
	      pairs.push_back(t);
	    }
	}

      qctx->getAnswer().setTuples(pairs);
    }

  return qctx;
}




RacerCachingDirector::RacerCachingDirector(RacerCache& c, DirectorPtr d)
  : RacerBaseDirector(),
    director(d),
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
      const Term& q = query.getQuery();
      
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
      
      // ask the director and add qctx pointer to the cache 
      qctx = director->query(qctx);
      cache[q] = qctx.get();
    }

  return qctx;
}



bool
RacerTermCache::cacheHit(const QueryCtx& query, const QueryCtx& found) const
{
  // is query equal to the cached query, i.e., is the set of ints in
  // query equal to the set of ints in found?
  return query.getQuery() == found.getQuery();
}




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
