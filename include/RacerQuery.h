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
#include <sstream>

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
  public:
    /**
     * The type of a RACER query.
     *
     * The members of the enum have to be in this order for
     * operator<()!
     */
    enum QueryType
      {
	Nullary = 0,      // ()
	Retrieval,        // (X)
	Boolean,          // (a)
	RelatedRetrieval, // (X,Y)
	LeftRetrieval,    // (X,b)
	RightRetrieval,   // (a,Y)
	RelatedBoolean    // (a,b)
      };

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

    virtual QueryType
    getType() const;

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

    friend std::ostream&
    operator<< (std::ostream&, const Query&);

    friend bool
    operator< (const Query&, const Query&);

    friend bool
    operator== (const Query&, const Query&);

    friend bool
    operator!= (const Query&, const Query&);
  };


  /**
   * @brief put the string representation of q into os
   * @param os
   * @param q
   * @return os
   */
  inline std::ostream&
  operator<< (std::ostream& os, const Query& q)
  {
    os << q.getNamespace()
       << "DL["
       << q.getOntology()
       << ','
       << q.getPlusC()
       << ','
       << q.getMinusC()
       << ','
       << q.getPlusR()
       << ','
       << q.getMinusR()
       << ','
       << q.getQuery()
       << "]("
       << q.getPatternTuple()
       << ')';

    return os;
  }


  /**
   * @brief lexicographically compare q1 to q2 and check if q1 is less than q2
   * @param q1
   * @param q2
   * @return true if q1 < q2, false otherwise.
   */
  inline bool
  operator< (const Query& q1, const Query& q2)
  {
    std::ostringstream os1;
    std::ostringstream os2;

    // paste the string representation of the various Query members
    // (without the query types) into the ostringstream in order to
    // compare them

    os1 << q1.getNamespace() << q1.getOntology()
	<< q1.getPlusC() << q1.getMinusC() << q1.getPlusR() << q1.getMinusR()
	<< q1.getQuery();

    os2 << q2.getNamespace() << q2.getOntology()
	<< q2.getPlusC() << q2.getMinusC() << q2.getPlusR() << q2.getMinusR()
	<< q2.getQuery();

    // check if q1 < q2 without looking at the query types
    bool lessthan = os1.str() < os2.str();

    // if q1 >= q2 we have to look at the query types in order to
    // compute the < relation on them
    if (!lessthan)
      {
	// check equality on q1 and q2
	bool eq = q1 == q2;
    
	// if query types are equal we have to distinguish between the
	// actual pattern tuples in a component-wise fashion
	if (eq && (q1.getType() == q2.getType()))
	  {
	    Query::QueryType t = q1.getType();
	
	    const Tuple& p1 = q1.getPatternTuple();
	    const Tuple& p2 = q2.getPatternTuple();

	    if (t == Query::Boolean) // (a1) < (a2) ?
	      {
		lessthan = p1[0] < p2[0];
	      }
	    else if (t == Query::RelatedBoolean) // (a1,b1) < (a2,b2) ?
	      {
		lessthan = (p1[0] < p2[0]) || (p1[1] < p2[1]);
	      }
	    else if (t == Query::LeftRetrieval) // (X1,b1) < (X2,b2) ?
	      {
		lessthan = p1[1] < p2[1];
	      }
	    else if (t == Query::RightRetrieval) // (a1,Y1) < (a2,Y2) ?
	      {
		lessthan = p1[0] < p2[0];
	      }
	    else
	      {
		// both types are equal, i.e. we can't distinguish
		// between the variables of Query::Retrieval and
		// Query::RelatedRetrieval query types, or we have
		// nullary queries
		lessthan = false;
	      }
	  }
	else if (eq && (q1.getType() < q2.getType())) // equal but types differ
	  {
	    lessthan = true;
	  }
	else
	  {
	    // nothing to do, either q1 > q2, or q1 == q1 and
	    // q1.getType() > q2.geType().
	  }
      }

    return lessthan;
  }

  /**
   * @brief lexicographically compare q1 to q2 and check if both queries are equal
   * @param q1
   * @param q2
   * @return true if q1 equals q2, false otherwise.
   */
  inline bool
  operator== (const Query& q1, const Query& q2)
  {
    return
      q1.getNamespace() == q2.getNamespace()
      && q1.getOntology() == q2.getOntology()
      && q1.getPlusC() == q2.getPlusC()
      && q1.getMinusC() == q2.getMinusC()
      && q1.getPlusR() == q2.getPlusR()
      && q1.getMinusR() == q2.getMinusR()
      && q1.getQuery() == q2.getQuery();
  }

  /**
   * @brief converse of operator==
   * @param q1
   * @param q2
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
