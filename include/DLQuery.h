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
 * @file   DLQuery.h
 * @author Thomas Krennwallner
 * @date   Mon Sep  4 10:36:46 2006
 * 
 * @brief  DL-Query class.
 * 
 * 
 */

#ifndef _DLQUERY_H
#define _DLQUERY_H

#include "Ontology.h"

#include <dlvhex2/ComfortPluginInterface.h>
#include <dlvhex2/Registry.h>

#include <iosfwd>
#include <iterator>

#include <boost/shared_ptr.hpp>

namespace dlvhex {
namespace dl {


  /**
   * @brief A DLQuery provides syntactic information for querying the
   * DL KB.
   */
  class DLQuery
  {
  private:
    /// Registry
    RegistryPtr reg;

    /// ontology uri + namespace
    Ontology::shared_pointer ontology;

    /// the query term
    ComfortTerm query;

    /// conjunctive query
    ComfortInterpretation cq;

    /// union of conjunctive query
    std::vector<ComfortInterpretation> ucq;

    /// tuple pattern
    ComfortTuple pattern;

    /// bitvector for quickly comparing the pattern tuple
    unsigned long typeFlags;

    /// setup #typeFlags and #pattern
    void
    setPatternTuple(const ComfortTuple& pattern);

  public:
    /** 
     * Ctor for a plain dl-query.
     * 
     * @param o ontology
     * @param q plain query
     * @param p output 
     */
    DLQuery(Ontology::shared_pointer o, const ComfortTerm& q, const ComfortTuple& p);

    /** 
     * Ctor for a cq-query.
     * 
     * @param o ontology
     * @param cq conjunctive query
     * @param p output
     */
    DLQuery(Ontology::shared_pointer o, const ComfortInterpretation& cq, const ComfortTuple& p);

    /** 
     * Ctor for a ucq-query.
     * 
     * @param o ontology
     * @param ucq union of conjunctive queries
     * @param p output
     */
    DLQuery(Ontology::shared_pointer o, const std::vector<ComfortInterpretation>& ucq, const ComfortTuple& p);

    /// dtor.
    virtual
    ~DLQuery()
    { }

    virtual const Ontology::shared_pointer&
    getOntology() const;

    virtual unsigned long
    getTypeFlags() const;

    virtual const ComfortTuple&
    getPatternTuple() const;

    virtual const ComfortTerm&
    getQuery() const;

    virtual const ComfortInterpretation&
    getConjQuery() const;

    virtual const std::vector<ComfortInterpretation>&
    getUnionConjQuery() const;

    virtual bool
    isBoolean() const;

    virtual bool
    isRetrieval() const;

    virtual bool
    isMixed() const;

    virtual bool
    isConjQuery() const;

    virtual bool
    isUnionConjQuery() const;

    friend std::ostream&
    operator<< (std::ostream& os, const DLQuery& q);

    friend bool
    operator< (const DLQuery& q1, const DLQuery& q2);

    friend bool
    operator== (const DLQuery& q1, const DLQuery& q2);

    friend bool
    operator!= (const DLQuery& q1, const DLQuery& q2);

    typedef boost::shared_ptr<DLQuery> shared_pointer;
  };



  /**
   * @brief put the string representation of @a q into @a os.
   *
   * @param os
   * @param q
   * @return @a os
   */
  inline std::ostream&
  operator<< (std::ostream& os, const DLQuery& q)
  {
    if (q.isConjQuery()) // cq-query
      {
	os << *q.getOntology()
	   << " {"
// @TODO: fix this
//	   << q.getPatternTuple()
	   << " | ";

	const ComfortInterpretation& cq = q.getConjQuery();
	if (!cq.empty())
	  {
	    std::copy(cq.begin(), --cq.end(), std::ostream_iterator<ComfortAtom>(os, ", "));
	    os << *(--cq.end());
	  }

	return os.put('}');
      }
    else if (q.isUnionConjQuery()) // ucq-query
      {
	os << *q.getOntology()
	   << " {"
// @TODO: fix this
//	   << q.getPatternTuple()
	   << " | ";

	const std::vector<ComfortInterpretation>& ucq = q.getUnionConjQuery();

	for (std::vector<ComfortInterpretation>::const_iterator it = ucq.begin();
	     it != --ucq.end(); ++it)
	  {
	    if (!it->empty())
	      {
		os << '(';
		std::copy(it->begin(), --it->end(), std::ostream_iterator<ComfortAtom>(os, ", "));
		os << *(--it->end()) << ") v ";
	      }
	  }

	const ComfortInterpretation& last = ucq.back();

	if (!last.empty())
	  {
	    os << '(';
	    std::copy(last.begin(), --last.end(), std::ostream_iterator<ComfortAtom>(os, ", "));
	    os << *(--last.end()) << ')';
	  }

	return os.put('}');
      }
    else // plain dl-query
      {
	return os << *q.getOntology()
		  << ' '
		  << q.getQuery()
		  << '('
// @TODO: fix this
//		  << q.getPatternTuple()
		  << ')';
      }
  }

  /**
   * @brief lexicographically compare @a q1 to @a q2 and check if @a
   * q1 is less than @a q2.
   *
   * @param q1
   * @param q2
   * @return true if @a q1 < @a q2, false otherwise.
   */
  bool
  operator< (const DLQuery& q1, const DLQuery& q2);

  /**
   * @brief lexicographically compare @a q1 to @a q2 and check if both
   * dl-queries are equal.
   *
   * @param q1
   * @param q2
   * @return true if @a q1 equals @a q2, false otherwise.
   */
  inline bool
  operator== (const DLQuery& q1, const DLQuery& q2)
  {
    ///@todo right now, we only support plain queries
    assert(!q1.isConjQuery() && !q1.isUnionConjQuery());

    if (q1.isConjQuery() == q2.isConjQuery() &&
	q1.isUnionConjQuery() == q2.isUnionConjQuery() &&
	*q1.getOntology() == *q2.getOntology()
	)
      {
	return q1.isConjQuery() ?
	  q1.getConjQuery() == q2.getConjQuery() :
	  (q2.isUnionConjQuery() ? q1.getUnionConjQuery() == q2.getUnionConjQuery() :
	   q1.getQuery() == q2.getQuery());
      }

    return false;
  }

  /**
   * @brief converse of operator==.
   * @param q1
   * @param q2
   * @return true if !(@a q1 == @a q2), false otherwise.
   */
  inline bool
  operator!= (const DLQuery& q1, const DLQuery& q2)
  {
    return !(q1 == q2);
  }

} // namespace dl
} // namespace dlvhex

#endif /* _DLQUERY_H */


// Local Variables:
// mode: C++
// End:
