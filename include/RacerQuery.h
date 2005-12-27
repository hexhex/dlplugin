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

#include <string>
#include <functional>
#include <vector>

#include <dlvhex/PluginInterface.h>
#include <dlvhex/Atom.h>
#include <dlvhex/Term.h>

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

  public:
    Query();

    virtual
    ~Query();

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

    ///@return true if interpretation is a proper subset of q2's
    ///interpretation, false otherwise
    virtual bool
    isSubset(const Query& q2) const;

    ///@return true if interpretation is a proper superset of q2's
    ///interpretation, false otherwise
    virtual bool
    isSuperset (const Query& q2) const;


    friend bool
    operator== (const Query&, const Query&);

    friend bool
    operator!= (const Query&, const Query&);
  };

  /**
   * @brief compare q1 to q2 and check if both queries are equal
   * @param q1
   * @param q2
   * @return true if q1 equals q2, false otherwise.
   */
  inline bool
  operator== (const Query& q1, const Query& q2)
  {
    if (q1.getQuery() == q2.getQuery() &&
	q1.getPatternTuple() == q2.getPatternTuple() &&
	q1.getNamespace() == q2.getNamespace()&&
	q1.getOntology() == q2.getOntology() &&
	q1.getInterpretation().getAtomSet()
	== q2.getInterpretation().getAtomSet() &&
	q1.getPlusC() == q2.getPlusC() &&
	q1.getMinusC() == q1.getMinusC() &&
	q1.getPlusR() == q1.getPlusR() &&
	q1.getMinusR() == q1.getMinusR()
	)
      {
	return true;
      }

    return false;
  }

  /**
   * @brief converse of operator==
   * @return true if !(q1 == q2), false otherwise.
   */
  inline bool
  operator!= (const Query& q1, const Query& q2)
  {
    return !(q1 == q2);
  }

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

  public:
    Answer();

    virtual
    ~Answer();

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

  };

} // namespace racer
} // namespace dlvhex

#endif /* _RACERQUERY_H */
