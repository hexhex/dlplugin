/* -*- C++ -*- */

/**
 * @file   RacerDirector.cpp
 * @author Thomas Krennwallner
 * @date   Wed Aug 24 09:45:18 2005
 * 
 * @brief  Composite and Caching Director classes for the communication with RACER.
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
RacerCompositeDirector::handleInconsistency(QueryCtxPtr qctx)
{
  Query::QueryType t = qctx->getQuery().getType();

  if (t == Query::Nullary || t == Query::Boolean || t == Query::RelatedBoolean)
    {
      // querying is trivial now -> true
      qctx->getAnswer().setAnswer(true);
      qctx->getAnswer().setTuples(std::vector<Tuple>());
    }
  else // retrieval modes
    {
      // just get all individuals
      RacerAllIndQuery all(stream);
      qctx = all.query(qctx);

      // check if we need to generate all possible pairs
      if (t == Query::RelatedRetrieval)
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
      // is query cached?
      RacerCache::iterator it = cache.find(qctx.get());

      if (it != cache.end())
	{
	  QueryCtx* found = *it;
	  
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
      cache.insert(qctx.get());
    }

  return qctx;
}



bool
RacerCachingDirector::cacheHit(const QueryCtx& query, const QueryCtx& found) const
{
  Query::QueryType t = query.getQuery().getType();
  Query& q1 = query.getQuery();
  Query& q2 = found.getQuery();

  if (t == Query::Boolean || t == Query::RelatedBoolean)
    {
      bool isPositive = found.getAnswer().getAnswer();

      if (isPositive)
	{
	  return q2.isSubseteq(q1);
	}
      else // check if found is a superset of query
	{
	  return q2.isSuperseteq(q1);
	}
    }
  else // retrieval modes
    {
      // is the set of ints in query equal to the set of ints in found?
      return
	q1.getInterpretation() == q2.getInterpretation();
    }
}
