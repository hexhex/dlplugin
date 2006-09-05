/* -*- C++ -*- */


/**
 * @file   DLQuery.cpp
 * @author Thomas Krennwallner
 * @date   Mon Sep  4 10:41:07 2006
 * 
 * @brief  DL-Query class.
 * 
 * 
 */

#include "DLQuery.h"

#include <dlvhex/Atom.h>
#include <dlvhex/AtomSet.h>
#include <dlvhex/Term.h>

using namespace dlvhex::dl;


namespace dlvhex {
namespace dl {

  bool
  operator< (const DLQuery& q1, const DLQuery& q2)
  {
    ///@todo right now, we only support plain queries
    assert(!q1.isConjQuery());

    // first check if ontology of q1 is less or greater than the ontology of q2

    if (*q1.getOntology() < *q2.getOntology())
      {
	return true;
      }
    else if (*q1.getOntology() > *q2.getOntology())
      {
	return false;
      }

    // otw. the ontologies are equal and we have to consider the
    // actual queries

    bool lessthan = q1.getQuery().getString() < q2.getQuery().getString();

    // if q1 >= q2 we have to look at the query types in order to
    // compute the < relation on them
    if (!lessthan)
      {
	// check equality on q1 and q2
	bool eq = q1 == q2;

	const Tuple& p1 = q1.getPatternTuple();
	const Tuple& p2 = q2.getPatternTuple();
	
	// if query types are equal we have to distinguish between the
	// actual pattern tuples in a component-wise fashion
	if (eq &&
	    q1.getTypeFlags() == q2.getTypeFlags() &&
	    p1.size() == p2.size())
	  {
	    unsigned long type = q1.getTypeFlags();
	    
	    Tuple::const_iterator it1 = p1.begin();
	    Tuple::const_iterator it2 = p2.begin();
	    unsigned long mask = 0x1;

	    for (;
		 it1 != p1.end() && it2 != p2.end();
		 ++it1, ++it2, mask <<= 1
		 )
	      {
		if ((type & mask) == mask)
		  {
		    if (*it1 < *it2)
		      {
			lessthan = true;
			break;
		      }
		  }
	      }
	  }
	else if (eq && p1.size() < p2.size())
	  {
	    // equal but arity differs
	    lessthan = true;
	  }
	else if (eq && p1.size() == p2.size() &&
		 q1.getTypeFlags() < q2.getTypeFlags())
	  {
	    // equal but types differ
	    lessthan = true;
	  }
	else
	  {
	    // nothing to do, either q1 > q2, or q1 == q1 and
	    // typeFlags or sizes are >=
	  }
      }
    
    return lessthan;
  }

} // namespace dl
} // namespace dlvhex



DLQuery::DLQuery(Ontology::shared_pointer o, const Term& q, const Tuple& p)
  : ontology(o),
    query(q)
{
  setPatternTuple(p);
}


DLQuery::DLQuery(Ontology::shared_pointer o, const AtomSet& cq, const Tuple& p)
  : ontology(o),
    conj(cq)
{
  setPatternTuple(p);
}


const Ontology::shared_pointer&
DLQuery::getOntology() const
{
  return this->ontology;
}

unsigned long
DLQuery::getTypeFlags() const
{
  return typeFlags;
}


bool
DLQuery::isBoolean() const
{
  // shift a 1 size times to the left - 1 => bitmask selects the valid
  // bits
  unsigned long mask = (1 << pattern.size()) - 1;

  // if any flag in the negation of flags is true, i.e. we have a
  // variable term in pattern, we don't have a purely boolean query
  return (~typeFlags & mask) == 0;
}


bool
DLQuery::isRetrieval() const
{
  // if any flag is true, i.e. we have a ground term in pattern, we
  // don't have a purely retrieval query
  return pattern.empty() ? false : typeFlags == 0;
}


bool
DLQuery::isMixed() const
{
  // if both flags and the negated flags are positive, we have a mixed
  // query
  return pattern.empty() ? false : typeFlags > 0 && ~typeFlags > 0;
}


bool
DLQuery::isConjQuery() const
{
  return !conj.empty();
}


const Term&
DLQuery::getQuery() const
{
  return this->query;
}


const AtomSet&
DLQuery::getConjQuery() const
{
  return this->conj;
}



void
DLQuery::setPatternTuple(const Tuple& pattern)
{
  assert(pattern.size() < 8 * sizeof(typeFlags));

  this->pattern = pattern;
  this->typeFlags = 0;

  unsigned long mask = 0x1;
  for (Tuple::const_iterator it = pattern.begin();
       it != pattern.end();
       ++it, mask <<= 1)
    {
      // for every ground term we set a flag in typeFlags
      if (!it->isVariable() && !it->isAnon())
	{
	  typeFlags |= mask;
	}
    }
}


const Tuple&
DLQuery::getPatternTuple() const
{
  return this->pattern;
}
