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


Query::Query(KBManager& kb,
	     const DLQuery::shared_pointer& q,
	     const Term& pc,
	     const Term& mc,
	     const Term& pr,
	     const Term& mr,
	     const AtomSet& i)
  : kbManager(kb),
    proj(),
    query(q)
{
  setInterpretation(i, pc, mc, pr, mr);
}


KBManager&
Query::getKBManager() const
{
  return this->kbManager;
}

const DLQuery::shared_pointer&
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

      // negate minusC and minusR atoms
      AtomPtr ap(isPC || isMC || isPR || isMR ?
		 new Atom(it->getArguments(), isMC || isMR) :
		 0);

      if (ap) proj.insert(ap);
    }
}
