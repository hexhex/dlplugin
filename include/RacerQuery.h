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
    std::string nspace;
    std::string ontology;

    GAtom query;
    GAtomSet plusConcept;
    GAtomSet minusConcept;
    GAtomSet plusRole;

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
    setQuery(const GAtom& query);

    virtual const GAtom&
    getQuery() const;

    virtual void
    setPlusConcept(const GAtomSet& plusConcept);

    virtual const GAtomSet&
    getPlusConcept() const;

    virtual void
    setMinusConcept(const GAtomSet& minusConcept);

    virtual const GAtomSet&
    getMinusConcept() const;

    virtual void
    setPlusRole(const GAtomSet& plusRole);

    virtual const GAtomSet&
    getPlusRole() const;

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
	q1.getNamespace() == q2.getNamespace()&&
	q1.getOntology() == q2.getOntology() &&
	q1.getPlusConcept() == q2.getPlusConcept() &&
	q1.getMinusConcept() == q2.getMinusConcept() &&
	q1.getPlusRole() == q2.getPlusRole()
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
  class Answer
  {
  private:
    std::string errorMsg;
    bool isIncoherent;

    bool answer;
    PluginAtom::TUPLEVECTOR tuples;

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
    setTuples(const PluginAtom::TUPLEVECTOR& tuples);

    virtual void
    addTuple(const Tuple& tuple);

    virtual const PluginAtom::TUPLEVECTOR&
    getTuples() const;

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
