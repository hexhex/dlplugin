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

namespace dlvhex {
  namespace dl {


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
      unsigned arity = it->getArity() - 1; // ignore the concept/role name parameter

      // we ignore atoms with wrong arity
      bool isPC = (p == pc) && (arity == 1);
      bool isMC = (p == mc) && (arity == 1);
      bool isPR = (p == pr) && (arity == 2);
      bool isMR = (p == mr) && (arity == 2);

      // negate minusC and minusR atoms
      AtomPtr ap(isPC || isMC || isPR || isMR ?
		 new Atom(it->getArguments(), isMC || isMR) :
		 0);

      if (ap) proj.insert(ap);
    }
}

} // namespace dl
} // namespace dlvhex


// Local Variables:
// mode: C++
// End:
