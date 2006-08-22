/* -*- C++ -*- */

/**
 * @file   Cache.cpp
 * @author Thomas Krennwallner
 * @date   Mon Jul  3 11:10:38 2006
 * 
 * @brief  Various DL caches.
 * 
 * @todo   add cache statistics
 * 
 */


#include "Cache.h"
#include "QueryCtx.h"
#include "Query.h"
#include "Answer.h"

#include <iterator>
#include <functional>
#include <boost/iterator/indirect_iterator.hpp>

using namespace dlvhex::dl;


const Cache::CacheSet*
Cache::find(const QueryCtx::shared_pointer& q) const
{
  QueryAnswerMap::const_iterator foundit = cacheMap.find(q->getQuery().getDLQuery());

  return foundit == cacheMap.end() ? 0 : &foundit->second;
}


QueryCtx::shared_pointer
Cache::isValid(const QueryCtx::shared_pointer& query, const Cache::CacheSet& found) const
{
  for (CacheSet::const_iterator it = found.begin(); it != found.end(); ++it)
    {
      const Query& q1 = query->getQuery();
      const Query& q2 = (*it)->getQuery();

      const AtomSet& i = q1.getProjectedInterpretation();
      const AtomSet& j = q2.getProjectedInterpretation();

      ///@todo support for cq missing

      if (q1.getDLQuery()->isBoolean())
	{
	  bool isPositive = (*it)->getAnswer().getAnswer();
	  
	  if (isPositive)
	    {
	      // does i include j, i.e. is j \subseteq i?
	      if (std::includes(i.begin(), i.end(), j.begin(), j.end()))
		{
		  return *it;
		}
	    }
	  else
	    {
	      // does j include i, i.e. is j \supseteq i?
	      if (std::includes(j.begin(), j.end(), i.begin(), i.end()))
		{
		  return *it;
		}
	    }
	}
      else // retrieval modes
	{
	  // is the set of ints in query equal to the set of ints in found?
	  if (i == j)
	    {
	      return *it;
	    }
	}
    }

  return QueryCtx::shared_pointer(); // nothing found
}


QueryCtx::shared_pointer
Cache::cacheHit(const QueryCtx::shared_pointer& query) const
{
  const CacheSet* found = find(query);

  if (found)
    {
      return isValid(query, *found);
    }

  return QueryCtx::shared_pointer(); // nothing found
}


namespace dlvhex {
  namespace dl {

    struct InterSubset : public std::unary_function<const QueryCtx::shared_pointer&, bool>
    {
      const QueryCtx::shared_pointer& q1;

      InterSubset(const QueryCtx::shared_pointer& q)
	: q1(q)
      { }

      bool
      operator() (const QueryCtx::shared_pointer& q2) const
      {
	const AtomSet& i = q1->getQuery().getProjectedInterpretation();
	const AtomSet& j = q2->getQuery().getProjectedInterpretation();
	// does i include j and i != j, i.e. is j \subset i?
	return i != j && std::includes(i.begin(), i.end(), j.begin(), j.end());
      }
    };

    struct InterSuperset : public std::unary_function<const QueryCtx::shared_pointer&, bool>
    {
      const QueryCtx::shared_pointer& q1;

      InterSuperset(const QueryCtx::shared_pointer& q)
	: q1(q)
      { }

      bool
      operator() (const QueryCtx::shared_pointer& q2) const
      {
	const AtomSet& i = q1->getQuery().getProjectedInterpretation();
	const AtomSet& j = q2->getQuery().getProjectedInterpretation();
	// does j include i and i != j, i.e. is j \supset i?
	return i != j && std::includes(j.begin(), j.end(), i.begin(), i.end());
      }
    };

    struct InterEqual : public std::unary_function<const QueryCtx::shared_pointer&, bool>
    {
      const QueryCtx::shared_pointer& q1;

      InterEqual(const QueryCtx::shared_pointer& q)
	: q1(q)
      { }

      bool
      operator() (const QueryCtx::shared_pointer& q2) const
      {
	const AtomSet& i = q1->getQuery().getProjectedInterpretation();
	const AtomSet& j = q2->getQuery().getProjectedInterpretation();
	return i == j;
      }
    };

  } // namespace dl
} // namespace dlvhex


void
Cache::insert(const QueryCtx::shared_pointer& query)
{
  QueryAnswerMap::iterator foundit = cacheMap.find(query->getQuery().getDLQuery());
  CacheSet* found = foundit == cacheMap.end() ? 0 : &foundit->second;

  if (found) // found a dl-query in the map
    {
      //
      // first, we have to perform the cache maintainance algorithm
      //

      typedef std::vector<CacheSet::iterator> RemoveVector;
      RemoveVector remove;
      std::insert_iterator<RemoveVector> rins = std::inserter(remove, remove.begin());

      if (query->getQuery().getDLQuery()->isBoolean()) // boolean query
	{
	  if (query->getAnswer().getAnswer()) // positive answer
	    {
	      // we only want to cache minimal interpretations
	      InterSuperset iss(query);

	      ///@todo maybe we can just insert it and remove all
	      ///previous (iteratorwise) boolean QueryCtxen with
	      ///positive answer?
	      for (CacheSet::iterator it = found->begin(); it != found->end(); ++it)
		{
		  if ((*it)->getAnswer().getAnswer() && iss(*it))
		    {
		      *rins = it;
		      ++rins;
		    }
		}
	    }
	  else // negative answer
	    {
	      // we only want to cache maximal interpretations
	      InterSubset iss(query);

	      ///@todo maybe we can just insert it and remove all
	      ///coming (iteratorwise) boolean QueryCtxen with
	      ///negative answer?
	      for (CacheSet::iterator it = found->begin(); it != found->end(); ++it)
		{
		  if (!(*it)->getAnswer().getAnswer() && iss(*it))
		    {
		      *rins = it;
		      ++rins;
		    }
		}
	    }
	}
      else // retrieval query
	{
	  // just insert it, nothing to remove. It should not occur
	  // that we have a clash when inserting two QueryCtx'en with
	  // the same interpretation since in that case we would have
	  // a cache-hit.
	}


      //
      // that is, we have to remove superfluous QueryCtx objects
      //

      for (RemoveVector::const_iterator it = remove.begin(); it != remove.end(); ++it)
	{
	  found->erase(*it);
	}

      //
      // after that, we can insert the new QueryCtx
      //

      std::pair<CacheSet::iterator, bool> p = found->insert(query);
    }
  else // dl-query not found, insert a new entry in the map
    {
      CacheSet cs;
      cs.insert(query);
      cacheMap.insert(std::make_pair(query->getQuery().getDLQuery(), cs));
    }
}


QueryCtx::shared_pointer
DebugCache::cacheHit(const QueryCtx::shared_pointer& query) const
{
  std::cerr << "===== cache content:" << std::endl;

  for (QueryAnswerMap::const_iterator it = cacheMap.begin();
       it != cacheMap.end(); ++it)
    {
      std::cerr << *(it->first) << std::endl;
    }

  std::cerr << "----- now looking for dl-query a = " << query->getQuery() << std::endl;

  const CacheSet* found = Cache::find(query);

  if (found)
    {
      std::cerr << "----- found cache(a): " << std::endl;

      std::copy(boost::make_indirect_iterator(found->begin()),
		boost::make_indirect_iterator(found->end()),
		std::ostream_iterator<QueryCtx>(std::cerr, "\n")
		);

      QueryCtx::shared_pointer p = Cache::isValid(query, *found);

      if (p)
	{
	  std::cerr << "===== cache-hit for a is " << *p << std::endl;
	  return p;
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

