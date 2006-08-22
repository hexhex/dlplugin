/* -*- C++ -*- */

/**
 * @file   QueryDirector.cpp
 * @author Thomas Krennwallner
 * @date   Wed Aug 24 09:45:18 2005
 * 
 * @brief  Composite and Caching Director classes for the communication with a dl engine.
 * 
 * 
 */

#include "QueryDirector.h"

#include "DLError.h"
#include "QueryCtx.h"
#include "Query.h"
#include "Answer.h"
#include "Cache.h"

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <iterator>

using namespace dlvhex::dl;


QueryCompositeDirector::QueryCompositeDirector(std::iostream& s)
  : QueryBaseDirector(),
    stream(s)
{ }


void
QueryCompositeDirector::add(QueryBaseDirector* d)
{
  if (d)
    {
      this->dirs.push_back(d);
    }
}


QueryCtx::shared_pointer
QueryCompositeDirector::query(QueryCtx::shared_pointer qctx) throw(DLError)
{
  for (boost::ptr_vector<QueryBaseDirector>::iterator it = dirs.begin();
       it != dirs.end(); ++it)
    {
      qctx = it->query(qctx);

      ///@todo decouple inconsistency handling from QueryCompositeDirector

      if (qctx->getAnswer().getIncoherent())
	{
	  return handleInconsistency(qctx);
	}
    }

  return qctx;
}


namespace dlvhex {
  namespace dl {

    /**
     * @brief Grounds the range [#pbeg, #pend) with [#ubeg, #uend).
     */
    class Grounder
    {
    private:
      std::vector<Tuple>& tuples;
      Tuple tuple;
      Ontology::Objects::const_iterator ubeg;
      Ontology::Objects::const_iterator uend;
      Tuple::const_iterator pbeg;
      Tuple::const_iterator pend;
      

      /// recursively grounding
      void
      ground(Tuple::const_iterator p, Tuple::iterator ins)
      {
	if (p == pend)
	  { 
	    tuples.push_back(tuple);
	  }
	else
	  {
	    if (!p->isVariable() && !p->isAnon())
	      {
		*ins = *p;
		ground(++p, ++ins);
	      }
	    else
	      {
		for (Ontology::Objects::const_iterator it = ubeg; it != uend; ++it)
		  {
		    *ins = *it;
		    ground(++p, ++ins);
		    --p; --ins;
		  }
	      }
	  }
      }
      
    public:
      
      /** 
       * Ctor.
       * 
       * @param t  append grounding to this tuple vector
       * @param ub begin of the universe
       * @param ue end of the universe
       * @param pb begin of the output list
       * @param pe end of the output list
       */
      Grounder(std::vector<Tuple>& t,
	       Ontology::Objects::const_iterator ub,
	       Ontology::Objects::const_iterator ue,
	       Tuple::const_iterator pb,
	       Tuple::const_iterator pe)
	: tuples(t),
	  tuple(pe - pb), // we reserve exactly the length of the output list
	  ubeg(ub),
	  uend(ue),
	  pbeg(pb),
	  pend(pe)
      { }

      void
      ground()
      {
	ground(pbeg, tuple.begin());
      }

    };


  } // namespace dl
} // namespace dlvhex



QueryCtx::shared_pointer
QueryCompositeDirector::handleInconsistency(QueryCtx::shared_pointer qctx)
{
  const DLQuery::shared_pointer& dlq = qctx->getQuery().getDLQuery();

  if (dlq->isBoolean())
    {
      // querying is trivial now -> true
      qctx->getAnswer().setAnswer(true);
      qctx->getAnswer().setTuples(std::vector<Tuple>());
    }
  else // retrieval modes
    {
      // just get all individuals
      
      Ontology::ObjectsPtr universe = dlq->getOntology()->getIndividuals();

      //
      // add individuals from the interpretation of qctx to universe
      //

      std::insert_iterator<Ontology::Objects> ii = std::inserter(*universe, universe->begin());

      for (AtomSet::const_iterator it1 = qctx->getQuery().getProjectedInterpretation().begin();
	   it1 != qctx->getQuery().getProjectedInterpretation().end();
	   ++it1)
	{
	  // get the arguments of each atom in the interpretation and
	  // insert it into the universe
	  const Tuple& args = it1->getArguments();
	  std::copy(args.begin(), args.end(), ii);
	}

      std::vector<Tuple> tuples;
      const Tuple& pat = dlq->getPatternTuple();

      Grounder g(tuples,
		 universe->begin(),
		 universe->end(),
		 pat.begin(),
		 pat.end());
      g.ground();

      qctx->getAnswer().setTuples(tuples);
    }

  return qctx;
}




QueryCachingDirector::QueryCachingDirector(BaseCache& c,
					   QueryBaseDirector::shared_pointer d)
  : QueryBaseDirector(),
    director(d),
    cache(c)
{ }


QueryCtx::shared_pointer
QueryCachingDirector::query(QueryCtx::shared_pointer qctx) throw(DLError)
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
