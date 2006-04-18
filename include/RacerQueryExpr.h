/* -*- C++ -*- */

/**
 * @file   RacerQueryExpr.h
 * @author Thomas Krennwallner
 * @date   Sat Apr 15 16:02:08 2006
 * 
 * @brief  Class hierarchy for basic query expressions.
 * 
 */


#ifndef _RACERQUERYEXPR_H
#define _RACERQUERYEXPR_H

#include <boost/shared_ptr.hpp>

#include <iostream>
#include <iterator>


namespace dlvhex {
namespace racer {


  /**
   * Base class for all the basic query expressions. Resembles the
   * interpreter pattern.
   */
  class QueryExpr
  {
  protected:
    virtual std::ostream&
    output(std::ostream&) const = 0;

  public:
    friend std::ostream&
    operator<< (std::ostream&, const QueryExpr&);
  };


  /** 
   * Calls e.output().
   * 
   * @param s 
   * @param e
   * 
   * @return the stream result of e.output().
   */
  std::ostream&
  operator<< (std::ostream& s, const QueryExpr& e)
  {
    return e.output(s);
  }


  /**
   * Base class for simple expressions used in ABox Queries.
   */
  class ABoxQueryExpr : public QueryExpr
  {
  protected:
    std::string symbol;		/**< symbol string */
    std::string nsid;		/**< namespace identifier */


    
    virtual std::ostream&
    output(std::ostream& s) const
    {
      if (symbol.find('#') == std::string::npos) // symbol doesn't contain '#'
	{
	  if (nsid.find('#') != std::string::npos) // nsid contains '#'
	    {
	      s << '|' << nsid << symbol << '|';
	    }
	  else			// nsid doesn't contain '#'
	    {
	      s << symbol;
	    }
	}
      else // symbol contains '#'
	{
	  s << '|' << symbol << '|';
	}

      return s;
    }


    /** 
     * Ctor.
     * 
     * @param s symbol string
     * @param n namespace identifier
     */
    ABoxQueryExpr(const std::string& s, const std::string& n)
      : symbol(s), nsid(n)
    { }

    ABoxQueryExpr() { }

  public:
    typedef ABoxQueryExpr value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };


  /**
   * Base class for ABox Query objects, i.e. individuals and
   * variables.
   */
  class ABoxQueryObject : public ABoxQueryExpr
  {
  protected:
    ABoxQueryObject(const std::string& s, const std::string& n)
      : ABoxQueryExpr(s, n)
    { }

    ABoxQueryObject() {}

  public:
    typedef ABoxQueryObject value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };


  /**
   * An ABox query variable with 4 different kinds of types: injective
   * (?x), non-injective ($?x), substrate (?*x) and
   * non-injective/substrate ($?*x)
   */
  class ABoxQueryVariable : public ABoxQueryObject
  {
   public:
    struct VariableType
      {
	static const unsigned noninjective = 0x1;
	static const unsigned substrate = 0x2;
      };

  protected:
    unsigned typeFlags;		/**< the type of this Query
				   Variable */

    std::ostream&
    output(std::ostream& s) const
    {
      if (typeFlags & VariableType::noninjective)
	{
	  s << '$';
	}

      s << '?';

      if (typeFlags & VariableType::substrate)
	{
	  s << '*';
	}

      return s << symbol;
    }

   public:
    explicit
    ABoxQueryVariable(const std::string& name, unsigned tf = 0)
      : ABoxQueryObject(name, std::string()), typeFlags(tf)
    { }
  };





  /**
   * A simple ABox individual expression.
   */
  class ABoxQueryIndividual : public ABoxQueryObject
  {
  public:
    explicit
    ABoxQueryIndividual(const std::string& name,
			const std::string& nsid = std::string())
      : ABoxQueryObject(name, nsid)
    { }

    typedef ABoxQueryIndividual value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };



  /**
   * Base class for atomic and complex concepts and roles.
   */
  class ABoxDescrExpr : public ABoxQueryExpr
  {
  protected:
    ABoxDescrExpr(const std::string& s, const std::string& n)
      : ABoxQueryExpr(s, n)
    { }

    ABoxDescrExpr() {}

  public:
    typedef ABoxDescrExpr value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };




  /**
   * An atomic Concept expression.
   */
  class ABoxQueryConcept : public ABoxDescrExpr
  {
  public:
    explicit
    ABoxQueryConcept(const std::string& name,
		     const std::string& nsid = std::string())
      : ABoxDescrExpr(name, nsid)
    { }

    typedef ABoxQueryConcept value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };


  /**
   * An atomic Role expression.
   */
  class ABoxQueryRole : public ABoxDescrExpr
  {
  public:
    explicit
    ABoxQueryRole(const std::string& name,
		  const std::string& nsid = std::string())
      : ABoxDescrExpr(name, nsid)
    { }

    typedef ABoxQueryRole value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };




  /**
   * Base class for ABox assertions.
   */
  class ABoxAssertion : public QueryExpr
  {
  protected:
    ABoxAssertion() {}

  public:
    typedef ABoxAssertion value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };


  /**
   * instance assertion.
   */
  class ABoxInstance : public ABoxAssertion
  {
  private:
    const ABoxQueryConcept::shared_pointer cExpr;
    const ABoxQueryIndividual::shared_pointer iExpr;

  protected:
    std::ostream&
    output(std::ostream& s) const
    {
      return s << "(instance "
	       << *cExpr
	       << " "
	       << *iExpr
	       << ")";
    }

  public:
    ABoxInstance(ABoxQueryConcept::const_pointer c,
		 ABoxQueryIndividual::const_pointer i)
      : cExpr(c), iExpr(i)
    {}

    typedef ABoxInstance value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };


  /**
   * related assertion.
   */
  class ABoxRelated : public ABoxAssertion
  {
  private:
    const ABoxQueryRole::shared_pointer rExpr;
    const ABoxQueryIndividual::shared_pointer i1Expr;
    const ABoxQueryIndividual::shared_pointer i2Expr;

  protected:
    std::ostream&
    output(std::ostream& s) const
    {
      return s << "(related "
	       << *i1Expr
	       << " "
	       << *i2Expr
	       << " "
	       << *rExpr
	       << ")";
    }

  public:
    ABoxRelated(ABoxQueryRole::const_pointer r,
		ABoxQueryIndividual::const_pointer i1,
		ABoxQueryIndividual::const_pointer i2)
      : rExpr(r), i1Expr(i1), i2Expr(i2)
    {}
  };


  /**
   * Base class for simple and complex query expressions.
   */
  class ABoxQueryAtom : public QueryExpr
  {
  protected:
    ABoxQueryAtom() {}

  public:
    typedef ABoxQueryAtom value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };



  /**
   * A negated query expression.
   */
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


  /**
   * An inverted query expression.
   */
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


  /**
   * A concept query atom.
   */
  class ConceptQuery : public ABoxQueryAtom
  {
  private:
    const ABoxQueryConcept::shared_pointer cExpr;
    const ABoxQueryObject::shared_pointer oExpr;

  protected:
    std::ostream&
    output(std::ostream& s) const
    {
      return s << "(" << *oExpr << " " << *cExpr << ")";
    }

  public:
    ConceptQuery(ABoxQueryConcept::const_pointer c,
		 ABoxQueryObject::const_pointer o)
      : cExpr(c), oExpr(o)
    { }
  };


  /**
   * A role query atom.
   */
  class RoleQuery : public ABoxQueryAtom
  {
  private:
    const ABoxQueryRole::shared_pointer rExpr;
    const ABoxQueryObject::shared_pointer o1Expr;
    const ABoxQueryObject::shared_pointer o2Expr;

  protected:
    std::ostream&
    output(std::ostream& s) const
    {
      return s << "(" << *o1Expr << " " << *o2Expr << " "  << *rExpr << ")";
    }

  public:
    RoleQuery(ABoxQueryRole::const_pointer r,
	      ABoxQueryObject::const_pointer o1,
	      ABoxQueryObject::const_pointer o2)
      : rExpr(r), o1Expr(o1), o2Expr(o2)
    { }
  };


} // namespace racer
} // namespace dlvhex

#endif /* _RACERQUERYEXPR_H */
