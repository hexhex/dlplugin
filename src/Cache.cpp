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
 * @file   Cache.cpp
 * @author Thomas Krennwallner
 * @date   Mon Jul  3 11:10:38 2006
 * 
 * @brief  Various DL caches.
 * 
 */


#include "Cache.h"
#include "QueryCtx.h"
#include "Query.h"
#include "Answer.h"

#include <iterator>
#include <functional>
#include <iostream>

#include <boost/iterator/indirect_iterator.hpp>
#include <boost/shared_ptr.hpp>

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
  QueryCtx::shared_pointer p;

  if (found)
    {
      p = isValid(query, *found);
      if (p) stats->hits(1);
      else   stats->miss(1);
    }
  else // nothing found
    {
      stats->miss(1);
    }

  return p;
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

      stats->qctxno(-remove.size());

      //
      // after that, we can insert the new QueryCtx
      //

      std::pair<CacheSet::iterator, bool> p = found->insert(query);

      // only update the statistics if we inserted a fresh QueryCtx
      if (p.second)
	{
	  stats->qctxno(1);
	}
    }
  else // dl-query not found, insert a new entry in the map
    {
      CacheSet cs;
      cs.insert(query);
      std::pair<QueryAnswerMap::iterator, bool> p = cacheMap.insert
	(std::make_pair(query->getQuery().getDLQuery(), cs));

      // only update the statistics if we inserted a fresh QueryCtx
      if (p.second)
	{
	  stats->dlqno(1);
	  stats->qctxno(1);
	}
    }
}


QueryCtx::shared_pointer
DebugCache::cacheHit(const QueryCtx::shared_pointer& query) const
{
  std::cerr << "===== now looking for dl-query a = " << query->getQuery() << std::endl;

  std::cerr << "----- cache content:" << std::endl;

  for (QueryAnswerMap::const_iterator it = cacheMap.begin();
       it != cacheMap.end(); ++it)
    {
      std::cerr << *(it->first) << std::endl;
    }

  const CacheSet* found = Cache::find(query);

  if (found)
    {
      std::cerr << "----- found cache(a): " << std::endl;

      for (CacheSet::const_iterator it = found->begin();
	   it != found->end(); ++it)
	{
	  std::cerr << *(*it) << " = " << (*it)->getAnswer() << std::endl;
	}

      QueryCtx::shared_pointer p = Cache::isValid(query, *found);

      if (p)
	{
	  std::cerr << "===== cache-hit for a is " << *p << std::endl;
	  stats->hits(1);
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

  stats->miss(1);

  return QueryCtx::shared_pointer();
}


// Local Variables:
// mode: C++
// End:
