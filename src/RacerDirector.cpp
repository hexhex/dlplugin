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
#include "Cache.h"

#include <boost/shared_ptr.hpp>

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


QueryCtx::shared_pointer
RacerCompositeDirector::query(QueryCtx::shared_pointer qctx) throw(RacerError)
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


QueryCtx::shared_pointer
RacerCompositeDirector::handleInconsistency(QueryCtx::shared_pointer qctx)
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




RacerCachingDirector::RacerCachingDirector(BaseCache& c,
					   RacerBaseDirector::shared_pointer d)
  : RacerBaseDirector(),
    director(d),
    cache(c)
{ }

RacerCachingDirector::~RacerCachingDirector()
{ }

QueryCtx::shared_pointer
RacerCachingDirector::query(QueryCtx::shared_pointer qctx) throw(RacerError)
{
  if (director.get() != 0)
    {
      QueryCtx::shared_pointer found = cache.cacheHit(qctx);

      if (found)
	{
	  // delete qctx pointer and overwrite with found pointer
	  qctx = found;
	}
      else
	{
	  // ask the director and add qctx pointer to the cache
	  qctx = director->query(qctx);
	  cache.insert(qctx);
	}
    }

  return qctx;
}
