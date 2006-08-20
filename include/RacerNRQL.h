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
namespace dl {

  //
  // forward declarations
  //
  class Query;

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
    ~NRQLBase()
    { }

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
  inline std::ostream&
  operator<< (std::ostream& s, const NRQLBase& b)
  {
    return b.output(s);
  }


  /**
   * Base class for nRQL body expressions.
   */
  class NRQLBody : public NRQLBase
  {
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

    /** 
     * output #atom to @a s as nRQL body atom expression.
     * 
     * @param s 
     * 
     * @return @a s
     */
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

    /** 
     * output #list to @a s as nRQL (and) expression
     * 
     * @param s 
     * 
     * @return @a s
     */
    std::ostream&
    output(std::ostream& s) const;

  public:
    /** 
     * add @a e to #list.
     * 
     * @param e 
     */
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

    /** 
     * output #list to @a s as nRQL (union) expression.
     * 
     * @param s 
     * 
     * @return @a s
     */
    std::ostream&
    output(std::ostream& s) const;

  public:
    /** 
     * add @a e to #list.
     * 
     * @param e 
     */
    void
    addAtom(NRQLBody::const_pointer e);

    typedef NRQLUnion value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<value_type> shared_pointer;
  };


  /**
   * Base class for the nRQL queries.
   */
  template <class Builder>
  class NRQLQuery : public NRQLBase
  {
  protected:
    /// the builder object
    Builder builder;

    /// Query object used to create the nRQL query
    const Query& query;

  public:
    /** 
     * Ctor.
     * 
     * @param q
     */
    NRQLQuery(const Query& q);
  };


  /**
   * A nRQL retrieve query.
   */
  template <class Builder>
  class NRQLRetrieve : public NRQLQuery<Builder>
  {
  private:
    /** 
     * output #query with help of #builder to @a s as nRQL retrieve
     * expression.
     * 
     * @param s 
     * 
     * @return @a s
     */
    std::ostream&
    output(std::ostream& s) const;

  public:
    explicit
    NRQLRetrieve(const Query& q);
  };


  /**
   * A nRQL tbox-retrieve query.
   */
  template <class Builder>
  class NRQLTBoxRetrieve : public NRQLQuery<Builder>
  {
  private:
    /** 
     * output #query with help of #builder to @a s as nRQL
     * tbox-retrieve expression.
     * 
     * @param s 
     * 
     * @return @a s
     */
    std::ostream&
    output(std::ostream& s) const;

  public:
    explicit
    NRQLTBoxRetrieve(const Query& q);
  };


  /**
   * A nRQL retrieve-under-premise query.
   */
  template <class Builder>
  class NRQLRetrieveUnderPremise : public NRQLQuery<Builder>
  {
  private:
    /** 
     * output #query with help of #builder to @a s as nRQL
     * retrieve-under-premise expression.
     * 
     * @param s 
     * 
     * @return @a s
     */
    std::ostream&
    output(std::ostream& s) const;

  public:
    explicit
    NRQLRetrieveUnderPremise(const Query& q);
  };


} // namespace racer
} // namespace dl
} // namespace dlvhex


// include the template implementation
#include "RacerNRQL.tcc"


#endif /* _RACERNRQL_H */
