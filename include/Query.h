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
 * @file   Query.h
 * @author Thomas Krennwallner
 * @date   Thu Aug  3 10:24:52 2006
 * 
 * @brief  Query class.
 * 
 * 
 */


#ifndef _QUERY_H
#define _QUERY_H

#include "DLQuery.h"

#include <dlvhex/Atom.h>
#include <dlvhex/AtomSet.h>
#include <dlvhex/Term.h>

#include <iosfwd>
#include <iterator>

namespace dlvhex {
namespace dl {

  //
  // forward declarations
  //
  class KBManager;


  /** 
   * @brief A Query holds informations about a knowledge base, the
   * assigned interpretation and a DLQuery.
   *
   * Holds both semantic and syntactic information of a dl-query.
   */
  class Query
  {
  private:
    /// kb-manager
    KBManager& kbManager;

    /// the projected interpretation
    AtomSet proj;

    /// the dl-query
    DLQuery::shared_pointer query;

    /// setup projected interpretations #proj
    void
    setInterpretation(const AtomSet& ints,
		      const Term& pc,
		      const Term& mc,
		      const Term& pr,
		      const Term& mr);

  public:
    /** 
     * Ctor.
     * 
     * @param kb kb manager
     * @param pc plus concept
     * @param mc minus concept
     * @param pr plus role
     * @param mr minus role
     * @param q dl-query
     * @param i interpretation
     */
    Query(KBManager& kb,
	  const DLQuery::shared_pointer& q,
	  const Term& pc,
	  const Term& mc,
	  const Term& pr,
	  const Term& mr,
	  const AtomSet& i);

    /// dtor.
    virtual
    ~Query()
    { }

    virtual KBManager&
    getKBManager() const;

    virtual const DLQuery::shared_pointer&
    getDLQuery() const;

    virtual const AtomSet&
    getProjectedInterpretation() const;

    friend std::ostream&
    operator<< (std::ostream& os, const Query& q);

    friend bool
    operator< (const Query& q1, const Query& q2);

    friend bool
    operator== (const Query& q1, const Query& q2);

    friend bool
    operator!= (const Query& q1, const Query& q2);
  };


  /**
   * @brief put the string representation of @a q into @a os.
   * @param os
   * @param q
   * @return @a os
   */
  inline std::ostream&
  operator<< (std::ostream& os, const Query& q)
  {
    if (q.getDLQuery()->isConjQuery())
      {
	os << "&dlCQ: ";
      }
    else if (q.getDLQuery()->getPatternTuple().size() == 1)
      {
	os << "&dlC: ";
      }
    else
      {
	os << "&dlR: ";
      }

    os << *q.getDLQuery()
       << " {";

    const AtomSet& pint = q.getProjectedInterpretation();

    if (!pint.empty())
      {
	std::copy(pint.begin(), --pint.end(), std::ostream_iterator<Atom>(os, ", "));
	os << *(--pint.end());
      }

    return os << '}';
  }

  /**
   * @brief lexicographically compare @a q1 to @a q2 and check if @a
   * q1 is less than @a q2.
   *
   * @param q1
   * @param q2
   * @return true if @a q1 < @a q2, false otherwise.
   */
  inline bool
  operator< (const Query& q1, const Query& q2)
  {
    // check if the components of query @a q1 are less than the
    // components of query @a q2
    return *q1.getDLQuery() < *q2.getDLQuery();
  }

  /**
   * @brief lexicographically compare @a q1 to @a q2 and check if both
   * queries are equal.
   *
   * @param q1
   * @param q2
   * @return true if @a q1 equals @a q2, false otherwise.
   */
  inline bool
  operator== (const Query& q1, const Query& q2)
  {
    return *q1.getDLQuery() == *q2.getDLQuery();
  }

  /**
   * @brief converse of operator==.
   * @param q1
   * @param q2
   * @return true if !(q1 == q2), false otherwise.
   */
  inline bool
  operator!= (const Query& q1, const Query& q2)
  {
    return !(q1 == q2);
  }

} // namespace dl
} // namespace dlvhex

#endif /* _QUERY_H */


// Local Variables:
// mode: C++
// End:
