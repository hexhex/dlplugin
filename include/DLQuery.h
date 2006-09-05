/* -*- C++ -*- */

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

#include <dlvhex/AtomSet.h>
#include <dlvhex/Term.h>

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
    /// ontology uri + namespace
    Ontology::shared_pointer ontology;

    /// the query term
    Term query;

    /// conjunctive query
    AtomSet conj;

    /// tuple pattern
    Tuple pattern;

    /// bitvector for quickly comparing the pattern tuple
    unsigned long typeFlags;

    /// setup #typeFlags and #pattern
    void
    setPatternTuple(const Tuple& pattern);

  public:
    /** 
     * Ctor for a plain dl-query.
     * 
     * @param o ontology
     * @param q plain query
     * @param p 
     */
    DLQuery(Ontology::shared_pointer o, const Term& q, const Tuple& p);

    /** 
     * Ctor for a conjunctive dl-query.
     * 
     * @param o ontology
     * @param cq conjunctive query
     * @param p 
     */
    DLQuery(Ontology::shared_pointer o, const AtomSet& cq, const Tuple& p);

    /// dtor.
    virtual
    ~DLQuery()
    { }

    virtual const Ontology::shared_pointer&
    getOntology() const;

    virtual unsigned long
    getTypeFlags() const;

    virtual const Tuple&
    getPatternTuple() const;

    virtual const Term&
    getQuery() const;

    virtual const AtomSet&
    getConjQuery() const;

    virtual bool
    isBoolean() const;

    virtual bool
    isRetrieval() const;

    virtual bool
    isMixed() const;

    virtual bool
    isConjQuery() const;

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
    if (q.isConjQuery())
      {
	os << *q.getOntology()
	   << " {"
	   << q.getPatternTuple()
	   << " | ";

	const AtomSet& cq = q.getConjQuery();
	if (!cq.empty())
	  {
	    std::copy(cq.begin(), --cq.end(), std::ostream_iterator<Atom>(os, ", "));
	    os << *(--cq.end());
	  }

	return os.put('}');
      }
    else
      {
	return os << *q.getOntology()
		  << ' '
		  << q.getQuery()
		  << '('
		  << q.getPatternTuple()
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
    assert(!q1.isConjQuery());

    if (q1.isConjQuery() == q2.isConjQuery() &&
	*q1.getOntology() == *q2.getOntology()
	)
      {
	return q1.isConjQuery() ?
	  q1.getConjQuery() == q2.getConjQuery() :
	  q1.getQuery() == q2.getQuery();
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
