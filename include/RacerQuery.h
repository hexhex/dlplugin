/* -*- C++ -*- */

/**
 * @file   RacerQuery.h
 * @author Thomas Krennwallner
 * @date   Sat Aug 20 17:12:00 2005
 * 
 * @brief  Encapsulates a query and the corresponding answer.
 * 
 * 
 */

#ifndef _RACERQUERY_H
#define _RACERQUERY_H

#include "RacerNRQL.h"
#include "RacerQueryExpr.h"

#include <dlvhex/PluginInterface.h>
#include <dlvhex/Atom.h>
#include <dlvhex/Term.h>

#include <string>
#include <functional>
#include <vector>
#include <sstream>

namespace dlvhex {
namespace racer {

  /**
   * @brief A RACER Query.
   */
  class Query
  {
  private:
    /// use this string to prefix the individuals of the interpretation
    std::string nspace;
    /// ontology uri
    std::string ontology;

    /// the query term
    Term query;
    /// tuple pattern
    Tuple pattern;
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

    /// bitvector for quickly comparing the pattern tuple
    unsigned long typeFlags;

  public:
    Query();

    virtual
    ~Query();

    virtual unsigned long
    getTypeFlags() const;

    virtual bool
    isBoolean() const;

    virtual bool
    isRetrieval() const;

    virtual bool
    isMixed() const;

    virtual void
    setNamespace(const std::string& nspace);

    virtual const std::string&
    getNamespace() const;

    virtual void
    setOntology(const std::string& ontology);

    virtual const std::string&
    getOntology() const;

    virtual void
    setQuery(const Term& query);

    virtual const Term&
    getQuery() const;

    virtual void
    setPatternTuple(const Tuple& pattern);

    virtual const Tuple&
    getPatternTuple() const;

    virtual void
    setInterpretation(const Interpretation& ints);

    virtual const Interpretation&
    getInterpretation() const;

    virtual void
    setPlusC(const Term& plusC);

    virtual const Term&
    getPlusC() const;

    virtual void
    setMinusC(const Term& minusC);

    virtual const Term&
    getMinusC() const;

    virtual void
    setPlusR(const Term& plusR);

    virtual const Term&
    getPlusR() const;

    virtual void
    setMinusR(const Term& minusR);

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


    virtual const std::vector<NRQLBody::shared_pointer>*
    createBody() const;

    virtual const std::vector<ABoxQueryObject::shared_pointer>*
    createHead() const;

    virtual const std::vector<ABoxAssertion::shared_pointer>*
    createPremise() const;


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


  /**
   * @brief The answer to a query.
   */
  class Answer : public PluginAtom::Answer
  {
  private:
    /// a RACER error message 
    std::string errorMsg;
    /// ABox is inconsistent
    bool isIncoherent;
    /// a boolean answer
    bool answer;
    /// keep a reference to our query, so we can determine if we need
    /// to fill tuples in some query flavours
    const Query* query;

  public:
    Answer();

    Answer(const Query* q);

    virtual
    ~Answer();

    virtual void
    setQuery(const Query* q);

    virtual void
    setIncoherent(bool isIncoherent);

    virtual bool
    getIncoherent() const;

    virtual void
    setErrorMessage(const std::string& errorMsg);

    virtual const std::string&
    getErrorMessage() const;

    virtual void
    setAnswer(bool answer);

    virtual bool
    getAnswer() const;

    virtual void
    addTuple(const Tuple& out);

  };


  /**
   * @brief Encapsulates a Query and the corresponding Answer.
   */
  class QueryCtx
  {
  private:
    Query* q;
    Answer* a;

  public:
    QueryCtx();

    explicit
    QueryCtx(const PluginAtom::Query& query);

    explicit
    QueryCtx(const QueryCtx& qctx);

    virtual
    ~QueryCtx();

    virtual void
    setQuery(Query* q);

    virtual Query&
    getQuery() const;

    virtual void
    setAnswer(Answer* a);

    virtual Answer&
    getAnswer() const;

    typedef QueryCtx value_type;
    typedef boost::shared_ptr<value_type> shared_pointer; /// managed QueryCtx
  };

} // namespace racer
} // namespace dlvhex

#endif /* _RACERQUERY_H */
