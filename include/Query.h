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
     * @param q 
     * @param p 
     */
    DLQuery(const Term& q, const Tuple& p);

    /** 
     * Ctor for a conjunctive dl-query.
     * 
     * @param cq 
     * @param p 
     */
    DLQuery(const AtomSet& cq, const Tuple& p);

    virtual
    ~DLQuery();

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
	os << '{' << q.getPatternTuple() << " | ";
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
	return os << q.getQuery() << '(' << q.getPatternTuple() << ')';
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

    if (q1.isConjQuery() == q2.isConjQuery())
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
   * @brief A Query holds informations about an Ontology, the assigned
   * interpretation and a DLQuery.
   *
   * Holds both semantic and syntactic information of a dl-query.
   */
  class Query
  {
  private:
    /// ontology uri + namespace
    Ontology::shared_pointer ontology;

    KBManager& kbManager;

    /// the whole projected interpretation
    AtomSet proj;

    /// projected interpretation for plus concepts
    AtomSet plusC;
    /// projected interpretation for minus concepts
    AtomSet minusC;
    /// projected interpretation for plus roles
    AtomSet plusR;
    /// projected interpretation for minus roles
    AtomSet minusR;

    /// the dl-query
    DLQuery query;

    /// setup projected interpretations #proj, #plusC, #minusC,
    /// #plusR, #minusR
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
     * @param onto ontology
     * @param kb kb manager
     * @param pc plus concept
     * @param mc minus concept
     * @param pr plus role
     * @param mr minus role
     * @param q dl-query
     * @param i interpretation
     */
    Query(const Ontology::shared_pointer onto,
	  KBManager& kb,
	  const Term& pc,
	  const Term& mc,
	  const Term& pr,
	  const Term& mr,
	  const DLQuery& q,
	  const AtomSet& i);

    virtual
    ~Query();

    virtual const Ontology::shared_pointer&
    getOntology() const;

    virtual KBManager&
    getKBManager() const;

    virtual const DLQuery&
    getDLQuery() const;

    virtual const AtomSet&
    getProjectedInterpretation() const;

    virtual const AtomSet&
    getPlusC() const;

    virtual const AtomSet&
    getMinusC() const;

    virtual const AtomSet&
    getPlusR() const;

    virtual const AtomSet&
    getMinusR() const;

    ///@return true if interpretation #proj is a subset of @a q2
    ///interpretation, false otherwise.
    virtual bool
    isSubseteq(const Query& q2) const;

    ///@return true if interpretation #proj is a superset of @a q2
    ///interpretation, false otherwise.
    virtual bool
    isSuperseteq(const Query& q2) const;


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
    if (q.getDLQuery().isConjQuery())
      {
	os << "&dlCQ: ";
      }
    else if (q.getDLQuery().getPatternTuple().size() == 1)
      {
	os << "&dlC: ";
      }
    else
      {
	os << "&dlR: ";
      }

    os << *q.getOntology()
       << ','
       << q.getDLQuery()
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
    return *q1.getOntology() < *q2.getOntology() ? true :
      q1.getDLQuery() < q2.getDLQuery() ? true : false;
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
    return *q1.getOntology() == *q2.getOntology()
      && q1.getDLQuery() == q2.getDLQuery();
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
