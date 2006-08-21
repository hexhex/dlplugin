/* -*- C++ -*- */

/**
 * @file   Query.cpp
 * @author Thomas Krennwallner
 * @date   Thu Aug  3 10:34:15 2006
 * 
 * @brief  Query classes.
 * 
 * 
 */


#include "Query.h"
#include "KBManager.h"

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



DLQuery::DLQuery(const Term& q, const Tuple& p)
  : query(q)
{
  setPatternTuple(p);
}


DLQuery::DLQuery(const AtomSet& cq, const Tuple& p)
  : conj(cq)
{
  setPatternTuple(p);
}

DLQuery::~DLQuery()
{ }


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



Query::Query(Ontology::shared_pointer o,
	     KBManager& kb,
	     const Term& pc,
	     const Term& mc,
	     const Term& pr,
	     const Term& mr,
	     const DLQuery& q,
	     const AtomSet& i)
  : ontology(o),
    kbManager(kb),
    query(q)
{
  setInterpretation(i, pc, mc, pr, mr);
}


Query::~Query()
{ }

const Ontology::shared_pointer&
Query::getOntology() const
{
  return this->ontology;
}

KBManager&
Query::getKBManager() const
{
  return this->kbManager;
}

const DLQuery&
Query::getDLQuery() const
{
  return this->query;
}

const AtomSet&
Query::getProjectedInterpretation() const
{
  return this->proj;
}

void
Query::setInterpretation(const AtomSet& ints,
			 const Term& pc, const Term& mc,
			 const Term& pr, const Term& mr
			 )
{
  // project out interpretation, i.e. compute I^\lambda
  for (AtomSet::const_iterator it = ints.begin();
       it != ints.end(); ++it)
    {
      const Term& p = it->getPredicate();
      unsigned arity = it->getArity() - 1;

      // we ignore atoms with wrong arity
      bool isPC = (p == pc) && (arity == 2);
      bool isMC = (p == mc) && (arity == 2);
      bool isPR = (p == pr) && (arity == 3);
      bool isMR = (p == mr) && (arity == 3);

      AtomPtr ap(isPC || isMC || isPR || isMR ?
		 new Atom(it->getArguments()) :
		 0);

      if      (isPC) plusC.insert(ap);
      else if (isMC) minusC.insert(ap);
      else if (isPR) plusR.insert(ap);
      else if (isMR) minusR.insert(ap);

      if (ap) proj.insert(ap);
    }
}

const AtomSet&
Query::getPlusC() const
{
  return this->plusC;
}

const AtomSet&
Query::getMinusC() const
{
  return this->minusC;
}

const AtomSet&
Query::getPlusR() const
{
  return this->plusR;
}

const AtomSet&
Query::getMinusR() const
{
  return this->minusR;
}


bool
Query::isSubseteq(const Query& q2) const
{
  //
  // isSubseteq() does not lexicographically compare q1 to q2 so this is
  // not an appropriate operator for std::less<Query>
  //
  const Query& q1 = *this;

  const AtomSet& i1 = q1.getProjectedInterpretation();
  const AtomSet& i2 = q2.getProjectedInterpretation();

  //
  // q1 is a subset of q2:
  //
  // interpretation of q1 is contained in the interpretation of q2.
  //
  return std::includes(i2.begin(), i2.end(), i1.begin(), i1.end());
}

bool
Query::isSuperseteq(const Query& q2) const
{
  //
  // q1 \supset q2 <=> q2 \subset q1
  //
  const Query& q1 = *this;
  return q2.isSubseteq(q1);
}
