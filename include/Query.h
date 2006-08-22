/* -*- C++ -*- */

/**
 * @file   Query.h
 * @author Thomas Krennwallner
 * @date   Thu Aug  3 10:24:52 2006
 * 
 * @brief  Query classes.
 * 
 * 
 */


#ifndef _QUERY_H
#define _QUERY_H

#include "Ontology.h"

#include <dlvhex/Atom.h>
#include <dlvhex/AtomSet.h>
#include <dlvhex/Term.h>

#include <iosfwd>
#include <iterator>

#include <boost/shared_ptr.hpp>

namespace dlvhex {
namespace dl {

  //
  // forward declarations
  //
  class KBManager;


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
