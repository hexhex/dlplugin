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
#include <boost/ptr_container/ptr_vector.hpp>

#include <iosfwd>

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
    operator<< (std::ostream& s, const NRQLBase& b);
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
    /// a managed ABoxQueryAtom 
    const ABoxQueryAtom::shared_pointer atom;

    std::ostream&
    output(std::ostream& s) const;

  public:
    explicit
    NRQLQueryAtom(ABoxQueryAtom::const_pointer a);

    typedef NRQLQueryAtom value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };



  /**
   * A nRQL conjunctive expression.
   */
  class NRQLConjunction : public NRQLBody
  {
  private:
    /// conjunction of NRQLBody expressions
    boost::ptr_vector<NRQLBody> list;

    std::ostream&
    output(std::ostream& s) const;

  public:
    void
    addAtom(NRQLBody::const_pointer e);

    typedef NRQLConjunction value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<value_type> shared_pointer;
  };



  /**
   * A nRQL union expression.
   */
  class NRQLUnion : public NRQLBody
  {
  private:
    /// union of NRQLBody expressions
    boost::ptr_vector<NRQLBody> list;

    std::ostream&
    output(std::ostream& s) const;

  public:
    void
    addAtom(NRQLBody::const_pointer e);

    typedef NRQLUnion value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<value_type> shared_pointer;
  };


  /**
   * Base class for the nRQL queries.
   */
  class NRQLQuery : public NRQLBase
  {
  protected:
    std::string abox; /// name of the ABox
    std::string tbox; /// name of the TBox

    boost::ptr_vector<ABoxQueryObject> head; /// query head
    NRQLBody::shared_pointer body;	     /// query body

    /** 
     * Protected Ctor.
     * 
     * @param a ABox name
     * @param t TBox name
     */
    NRQLQuery(const std::string& a, const std::string& t);

    virtual std::ostream&
    output(std::ostream& s) const;

  public:
    void
    addHead(ABoxQueryObject::const_pointer e);

    void
    setHead(boost::shared_ptr<boost::ptr_vector<ABoxQueryObject> > sp);

    void
    setBody(NRQLBody::shared_pointer sp);

    typedef NRQLQuery value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<value_type> shared_pointer;
  };


  /**
   * A retrieve query.
   */
  class NRQLRetrieve : public NRQLQuery
  {
  private:
    std::ostream&
    output(std::ostream& s) const;

  public:
    explicit
    NRQLRetrieve(const std::string& abox = "");

    typedef NRQLRetrieve value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<value_type> shared_pointer;
  };


  /**
   * A tbox-retrieve query.
   */
  class NRQLTBoxRetrieve : public NRQLQuery
  {
  private:
    std::ostream&
    output(std::ostream& s) const;

  public:
    explicit
    NRQLTBoxRetrieve(const std::string& tbox = "");

    typedef NRQLTBoxRetrieve value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<value_type> shared_pointer;
  };


  /**
   * A retrieve-under-premise query.
   */
  class NRQLRetrieveUnderPremise : public NRQLQuery
  {
  private:
    boost::ptr_vector<ABoxAssertion> premise;	/// query premise

    std::ostream&
    output(std::ostream& s) const;

  public:
    void
    addPremise(ABoxAssertion::const_pointer e);

    explicit
    NRQLRetrieveUnderPremise(const std::string& abox = "");

    typedef NRQLRetrieveUnderPremise value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<value_type> shared_pointer;
  };



} // namespace racer
} // namespace dlvhex

#endif /* _RACERNRQL_H */
