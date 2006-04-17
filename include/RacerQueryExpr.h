/* -*- C++ -*- */

/**
 * @file   RacerQueryExpr.h
 * @author Thomas Krennwallner
 * @date   Sat Apr 15 16:02:08 2006
 * 
 * @brief  
 * 
 * @todo this is so not documented
 * 
 */


#ifndef _RACERQUERYEXPR_H
#define _RACERQUERYEXPR_H

#include <boost/shared_ptr.hpp>

#include <iostream>
#include <iterator>


namespace dlvhex {
namespace racer {


  class QueryExpr
  {
  protected:
    virtual std::ostream&
    output(std::ostream&) const = 0;

  public:
    friend std::ostream&
    operator<< (std::ostream&, const QueryExpr&);
  };


  std::ostream&
  operator<< (std::ostream& s, const QueryExpr& a)
  {
    return a.output(s);
  }


  class ABoxQueryExpr : public QueryExpr
  {
  protected:
    std::string symbol;

    explicit
    ABoxQueryExpr(const std::string& s) : symbol(s) { }

    ABoxQueryExpr() { }

  public:
    typedef ABoxQueryExpr value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };



  class ABoxQueryObject : public ABoxQueryExpr
  {
  protected:
    explicit
    ABoxQueryObject(const std::string& s) : ABoxQueryExpr(s) { }

    ABoxQueryObject() {}

  public:
    typedef ABoxQueryObject value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };


  class ABoxQueryVariable : public ABoxQueryObject
  {
  protected:
    std::ostream&
    output(std::ostream& s) const
    {
      ///@todo checkout variable type
      return s << "$?" << symbol;
    }

  public:
    explicit
    ABoxQueryVariable(const std::string& name)
      : ABoxQueryObject(name)
    { }
  };



  class ABoxQueryIndividual : public ABoxQueryObject
  {
  protected:
    std::ostream&
    output(std::ostream& s) const
    {
      ///@todo nsid
      return s << symbol;
    }

  public:
    explicit
    ABoxQueryIndividual(const std::string& name)
      : ABoxQueryObject(name)
    { }

    typedef ABoxQueryIndividual value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };




  class ABoxQueryConcept : public ABoxQueryExpr
  {
  protected:
    std::ostream&
    output(std::ostream& s) const
    {
      ///@todo nsid
      return s << symbol;
    }

  public:
    explicit
    ABoxQueryConcept(const std::string& name)
      : ABoxQueryExpr(name)
    { }

    typedef ABoxQueryConcept value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };


  class ABoxQueryRole : public ABoxQueryExpr
  {
  protected:
    std::ostream&
    output(std::ostream& s) const
    {
      ///@todo nsid
      return s << symbol;
    }

  public:
    explicit
    ABoxQueryRole(const std::string& name)
      : ABoxQueryExpr(name)
    { }

    typedef ABoxQueryRole value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };




  class ABoxAssertion : public QueryExpr
  {
  protected:
    ABoxAssertion() {}

  public:
    typedef ABoxAssertion value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };


  class ABoxInstance : public ABoxAssertion
  {
  private:
    const ABoxQueryConcept::shared_pointer cExpr;
    const ABoxQueryIndividual::shared_pointer aExpr;

  protected:
    std::ostream&
    output(std::ostream& s) const
    {
      return s << "(instance "
	       << *cExpr
	       << " "
	       << *aExpr
	       << ")";
    }

  public:
    ABoxInstance(ABoxQueryConcept::const_pointer c,
		 ABoxQueryIndividual::const_pointer a)
      : cExpr(c), aExpr(a)
    {}

    typedef ABoxInstance value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };





  class ABoxRelated : public ABoxAssertion
  {
  private:
    const ABoxQueryRole::shared_pointer rExpr;
    const ABoxQueryIndividual::shared_pointer a1Expr;
    const ABoxQueryIndividual::shared_pointer a2Expr;

  protected:
    std::ostream&
    output(std::ostream& s) const
    {
      return s << "(related "
	       << *a1Expr
	       << " "
	       << *a2Expr
	       << " "
	       << *rExpr
	       << ")";
    }

  public:
    ABoxRelated(ABoxQueryRole::const_pointer r,
		ABoxQueryIndividual::const_pointer a1,
		ABoxQueryIndividual::const_pointer a2)
      : rExpr(r), a1Expr(a1), a2Expr(a2)
    {}
  };




  class ABoxQueryAtom : public QueryExpr
  {
  protected:
    ABoxQueryAtom() {}

  public:
    typedef ABoxQueryAtom value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };



  class NegationQuery : public ABoxQueryAtom
  {
  private:
    const ABoxQueryAtom::shared_pointer atom;

  protected:
    std::ostream&
    output(std::ostream& s) const
    {
      return s << "(not " << *atom << ")";
    }

  public:
    explicit
    NegationQuery(ABoxQueryAtom::const_pointer a) : atom(a) { }
  };


  class InvertedQuery : public ABoxQueryAtom
  {
  private:
    const ABoxQueryAtom::shared_pointer atom;

  protected:
    std::ostream&
    output(std::ostream& s) const
    {
      return s << "(inv " << *atom << ")";
    }

  public:
    explicit
    InvertedQuery(ABoxQueryAtom::const_pointer a) : atom(a) { }
  };



  class ConceptQuery : public ABoxQueryAtom
  {
  private:
    const ABoxQueryConcept::shared_pointer cExpr;
    const ABoxQueryObject::shared_pointer aExpr;

  protected:
    std::ostream&
    output(std::ostream& s) const
    {
      return s << "(" << *aExpr << " " << *cExpr << ")";
    }

  public:
    ConceptQuery(ABoxQueryConcept::const_pointer c,
		 ABoxQueryObject::const_pointer a)
      : cExpr(c), aExpr(a)
    { }
  };



  class RoleQuery : public ABoxQueryAtom
  {
  private:
    const ABoxQueryRole::shared_pointer rExpr;
    const ABoxQueryObject::shared_pointer a1Expr;
    const ABoxQueryObject::shared_pointer a2Expr;

  protected:
    std::ostream&
    output(std::ostream& s) const
    {
      return s << "(" << *a1Expr << " " << *a2Expr << " "  << *rExpr << ")";
    }

  public:
    RoleQuery(ABoxQueryRole::const_pointer r,
	      ABoxQueryObject::const_pointer a1,
	      ABoxQueryObject::const_pointer a2)
      : rExpr(r), a1Expr(a1), a2Expr(a2)
    { }
  };


} // namespace racer
} // namespace dlvhex

#endif /* _RACERQUERYEXPR_H */
