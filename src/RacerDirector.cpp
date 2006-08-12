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
#include "QueryCtx.h"
#include "Query.h"
#include "Answer.h"
#include "Cache.h"

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

using namespace dlvhex::racer;


RacerCompositeDirector::RacerCompositeDirector(std::iostream& s)
  : RacerBaseDirector(),
    stream(s)
{ }


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
  for (boost::ptr_vector<RacerBaseDirector>::iterator it = dirs.begin();
       it != dirs.end(); ++it)
    {
      qctx = it->query(qctx);

      if (qctx->getAnswer().getIncoherent())
	{
	  return handleInconsistency(qctx);
	}
    }

  return qctx;
}


namespace dlvhex {
  namespace racer {

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


  } // namespace racer
} // namespace dlvhex



QueryCtx::shared_pointer
RacerCompositeDirector::handleInconsistency(QueryCtx::shared_pointer qctx)
{
  const DLQuery& dlq = qctx->getQuery().getDLQuery();

  if (dlq.isBoolean())
    {
      // querying is trivial now -> true
      qctx->getAnswer().setAnswer(true);
      qctx->getAnswer().setTuples(std::vector<Tuple>());
    }
  else // retrieval modes
    {
      // just get all individuals

      ///@todo add interpretation from qctx
      Ontology::ObjectsPtr universe = qctx->getQuery().getOntology()->getIndividuals();

      std::vector<Tuple> tuples;
      const Tuple& pat = dlq.getPatternTuple();

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




RacerCachingDirector::RacerCachingDirector(BaseCache& c,
					   RacerBaseDirector::shared_pointer d)
  : RacerBaseDirector(),
    director(d),
    cache(c)
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
