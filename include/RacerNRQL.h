/* -*- C++ -*- */

/**
 * @file   RacerNRQL.h
 * @author Thomas Krennwallner
 * @date   Thu Apr 13 11:24:32 2006
 * 
 * @brief  Class hierarchy for nRQL query expressions.
 * 
 */

#ifndef _RACERNRQL_H
#define _RACERNRQL_H

#include "RacerQueryExpr.h"

#include <boost/shared_ptr.hpp>

#include <iostream>
#include <vector>

namespace dlvhex {
namespace racer {



  /**
   * Base class for nRQL query expressions. Resembles interpreter
   * pattern.
   */
  class NRQLBase
  {
  private:
    virtual std::ostream&
    output(std::ostream&) const = 0;

  public:
    virtual
    ~NRQLBase() {}

    friend std::ostream&
    operator<< (std::ostream&, const NRQLBase&);
  };


  /** 
   * Calls b.output().
   * 
   * @param s 
   * @param b 
   * 
   * @return the stream result of b.output().
   */
  std::ostream&
  operator<< (std::ostream& s, const NRQLBase& b);



  /**
   * Base class for nRQL body expressions.
   */
  class NRQLBody : public NRQLBase
  {
  protected:
    NRQLBody() {}

  public:
    typedef NRQLBody value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };


  /**
   * A nRQL query atom.
   */
  class NRQLQueryAtom : public NRQLBody
  {
  private:
    const ABoxQueryAtom::shared_pointer atom;

    std::ostream&
    output(std::ostream& s) const;

  public:
    explicit
    NRQLQueryAtom(ABoxQueryAtom::const_pointer a);
  };



  /**
   * A nRQL conjunctive expression.
   */
  class NRQLConjunction : public NRQLBody
  {
  private:
    std::vector<NRQLBody::shared_pointer> list;

    std::ostream&
    output(std::ostream& s) const;

  public:
    void
    addAtom(NRQLBody::const_pointer e);
  };



  /**
   * A nRQL union expression.
   */
  class NRQLUnion : public NRQLBody
  {
  private:
    std::vector<NRQLBody::shared_pointer> list;

    std::ostream&
    output(std::ostream& s) const;

  public:
    void
    addAtom(NRQLBody::const_pointer e);
  };


  /**
   * Base class for the nRQL queries.
   */
  class NRQLQuery : public NRQLBase
  {
  protected:
    std::vector<ABoxQueryObject::shared_pointer> head; /**< query head */
    std::vector<NRQLBody::shared_pointer> body;	/**< query body */

    NRQLQuery() {}

  public:
    void
    addHead(ABoxQueryObject::const_pointer e);

    void
    addBody(NRQLBody::const_pointer e);

    typedef NRQLQuery value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };


  /**
   * A retrieve query.
   */
  class NRQLRetrieve : public NRQLQuery
  {
  private:
    std::ostream&
    output(std::ostream& s) const;
  };


  /**
   * A tbox-retrieve query.
   */
  class NRQLTBoxRetrieve : public NRQLQuery
  {
  private:
    std::ostream&
    output(std::ostream& s) const;
  };


  /**
   * A retrieve-under-premise query.
   */
  class NRQLRetrieveUnderPremise : public NRQLQuery
  {
  private:
    std::vector<ABoxAssertion::shared_pointer> premise;	/**< query premise */

    std::ostream&
    output(std::ostream& s) const;

  public:
    void
    addPremise(ABoxAssertion::const_pointer e);
  };



} // namespace racer
} // namespace dlvhex

#endif /* _RACERNRQL_H */
