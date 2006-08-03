/* -*- C++ -*- */

/**
 * @file   Query.h
 * @author Thomas Krennwallner
 * @date   Thu Aug  3 10:24:52 2006
 * 
 * @brief  
 * 
 * 
 */


#ifndef _QUERY_H
#define _QUERY_H

#include <dlvhex/Atom.h>
#include <dlvhex/AtomSet.h>
#include <dlvhex/Term.h>

#include <string>
#include <iosfwd>

namespace dlvhex {
namespace racer {


  /**
   * @brief A DL Query, i.e. syntactic information for querying the DL
   * KB.
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

    /// setup typeFlags and pattern
    void
    setPatternTuple(const Tuple& pattern);

  public:
    DLQuery(const Term& q, const Tuple& p);

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
   * @brief put the string representation of q into os
   * @param os
   * @param q
   * @return os
   */
  std::ostream&
  operator<< (std::ostream& os, const DLQuery& q);

  /**
   * @brief lexicographically compare q1 to q2 and check if q1 is less than q2
   * @param q1
   * @param q2
   * @return true if q1 < q2, false otherwise.
   */
  bool
  operator< (const DLQuery& q1, const DLQuery& q2);

  /**
   * @brief lexicographically compare q1 to q2 and check if both dl-queries are equal
   * @param q1
   * @param q2
   * @return true if q1 equals q2, false otherwise.
   */
  bool
  operator== (const DLQuery& q1, const DLQuery& q2);

  /**
   * @brief converse of operator==
   * @param q1
   * @param q2
   * @return true if !(q1 == q2), false otherwise.
   */
  bool
  operator!= (const DLQuery& q1, const DLQuery& q2);



  /**
   * @brief A Query with encapsulated DLQuery, holds both semantic and
   * syntactic information (DLQuery).
   */
  class Query
  {
  private:
    /// ontology uri
    std::string ontology;
    /// use this string to prefix the individuals of the interpretation
    std::string nspace;

    /// term used to identify which individuals are used to extend
    /// positive concepts
    Term plusC;
    /// term used to identify which individuals are used to extend
    /// negative concepts
    Term minusC;
    /// term used to identify which individuals are used to extend
    /// positive roles
    Term plusR;
    /// term used to identify which individuals are used to extend
    /// negative roles
    Term minusR;
    /// set of ground atoms
    Interpretation interpretation;

    /// the dl-query
    DLQuery query;

    /// private default ctor
    Query();

    /// setup interpretation
    void
    setInterpretation(const Interpretation& ints);


  public:
    Query(const std::string& uri,
	  const std::string& nspace,
	  const Term& pc,
	  const Term& mc,
	  const Term& pr,
	  const Term& mr,
	  const DLQuery& q,
	  const Interpretation& i);

    virtual
    ~Query();

    virtual const std::string&
    getNamespace() const;

    virtual const std::string&
    getOntology() const;

    virtual const DLQuery&
    getDLQuery() const;

    virtual const Interpretation&
    getInterpretation() const;

    virtual const Term&
    getPlusC() const;

    virtual const Term&
    getMinusC() const;

    virtual const Term&
    getPlusR() const;

    virtual const Term&
    getMinusR() const;

    ///@return true if interpretation is a subset of q2's
    ///interpretation, false otherwise
    virtual bool
    isSubseteq(const Query& q2) const;

    ///@return true if interpretation is a superset of q2's
    ///interpretation, false otherwise
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
   * @brief put the string representation of q into os
   * @param os
   * @param q
   * @return os
   */
  std::ostream&
  operator<< (std::ostream& os, const Query& q);

  /**
   * @brief lexicographically compare q1 to q2 and check if q1 is less than q2
   * @param q1
   * @param q2
   * @return true if q1 < q2, false otherwise.
   */
  bool
  operator< (const Query& q1, const Query& q2);

  /**
   * @brief lexicographically compare q1 to q2 and check if both queries are equal
   * @param q1
   * @param q2
   * @return true if q1 equals q2, false otherwise.
   */
  bool
  operator== (const Query& q1, const Query& q2);

  /**
   * @brief converse of operator==
   * @param q1
   * @param q2
   * @return true if !(q1 == q2), false otherwise.
   */
  bool
  operator!= (const Query& q1, const Query& q2);

} // namespace racer
} // namespace dlvhex

#endif /* _QUERY_H */
