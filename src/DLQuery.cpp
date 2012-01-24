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
 * @file   DLQuery.cpp
 * @author Thomas Krennwallner
 * @date   Mon Sep  4 10:41:07 2006
 * 
 * @brief  DL-Query class.
 * 
 * 
 */

#include "DLQuery.h"

#include <dlvhex/ComfortPluginInterface.hpp>


namespace dlvhex {
namespace dl {

  bool
  operator< (const DLQuery& q1, const DLQuery& q2)
  {
    ///@todo right now, we only support plain queries
    assert(!q1.isConjQuery() && !q1.isUnionConjQuery());
    assert(!q2.isConjQuery() && !q2.isUnionConjQuery());

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

    bool lessthan = q1.getQuery().strval < q2.getQuery().strval;

    // if q1 >= q2 we have to look at the query types in order to
    // compute the < relation on them
    if (!lessthan)
      {
	// check equality on q1 and q2
	bool eq = q1 == q2;

	const ComfortTuple& p1 = q1.getPatternTuple();
	const ComfortTuple& p2 = q2.getPatternTuple();
	
	// if query types are equal we have to distinguish between the
	// actual pattern tuples in a component-wise fashion
	if (eq &&
	    q1.getTypeFlags() == q2.getTypeFlags() &&
	    p1.size() == p2.size())
	  {
	    unsigned long type = q1.getTypeFlags();
	    
	    ComfortTuple::const_iterator it1 = p1.begin();
	    ComfortTuple::const_iterator it2 = p2.begin();
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


DLQuery::DLQuery(Ontology::shared_pointer o, const ComfortTerm& q, const ComfortTuple& p)
  : ontology(o),
    query(q),
    cq(),
    ucq(),
    pattern(),
    typeFlags(0)
{
  setPatternTuple(p);
}


DLQuery::DLQuery(Ontology::shared_pointer o, const std::vector<ComfortTerm>& c, const ComfortTuple& p)
  : ontology(o),
    query(ComfortTerm::createConstant("")),
    cq(c),
    ucq(),
    pattern(),
    typeFlags(0)
{
  setPatternTuple(p);
}


DLQuery::DLQuery(Ontology::shared_pointer o, const std::vector<std::vector<ComfortTerm> >& u, const ComfortTuple& p)
  : ontology(o),
    query(ComfortTerm::createConstant("")),
    cq(),
    ucq(u.begin(), u.end()),
    pattern(),
    typeFlags(0)
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

  // if any flag in the negation of the typeFlags is true, i.e. we
  // have a variable term in the output, we don't have a purely
  // boolean query
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
  return !cq.empty();
}


bool
DLQuery::isUnionConjQuery() const
{
  return !ucq.empty();
}


const ComfortTerm&
DLQuery::getQuery() const
{
  return this->query;
}


const std::vector<ComfortTerm>&
DLQuery::getConjQuery() const
{
  return this->cq;
}


const std::vector<std::vector<ComfortTerm> >&
DLQuery::getUnionConjQuery() const
{
  return this->ucq;
}


void
DLQuery::setPatternTuple(const ComfortTuple& pattern)
{
  assert(pattern.size() < 8 * sizeof(typeFlags));

  this->pattern = pattern;
  this->typeFlags = 0;

  unsigned long mask = 0x1;
  for (ComfortTuple::const_iterator it = pattern.begin();
       it != pattern.end();
       ++it, mask <<= 1)
    {
      // for every ground term we set a flag in typeFlags
      if (!it->isVariable() && !(getQuery().strval == "_"))
	{
	  typeFlags |= mask;
	}
    }
}


const ComfortTuple&
DLQuery::getPatternTuple() const
{
  return this->pattern;
}

} // namespace dl
} // namespace dlvhex


// Local Variables:
// mode: C++
// End:
