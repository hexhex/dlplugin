/* dlvhex-dlplugin -- Integration of Answer-Set Programming and Description Logics.
 *
 * Copyright (C) 2005, 2006, 2007  Thomas Krennwallner
 * 
 * This file is part of dlvhex-dlplugin.
 *
 * dlvhex-dlplugin is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * dlvhex-dlplugin is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dlvhex-dlplugin; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


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

#include <dlvhex/Term.h>

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <iosfwd>
#include <string>

namespace dlvhex {
namespace dl {
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
    virtual
    ~QueryExpr()
    { }

    friend std::ostream&
    operator<< (std::ostream& s, const QueryExpr& e);
  };


  /** 
   * Calls e.output().
   * 
   * @param s 
   * @param e
   * 
   * @return the stream result of e.output().
   */
  inline std::ostream&
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
    /// symbol term
    const Term symbol;
    /// namespace identifier
    const std::string nsid;

    /**
     * output the content of #symbol in proper Racer syntax.
     *
     * @param s stream
     *
     * @return @a s
     */
    virtual std::ostream&
    output(std::ostream& s) const;

    /// protected default ctor
    ABoxQueryExpr()
      : symbol(), nsid()
    { }
    
    /** 
     * protected ctor.
     * 
     * @param s symbol term
     * @param n namespace identifier
     */
    ABoxQueryExpr(const Term& s, const std::string& n)
      : symbol(s), nsid(n)
    { }

  public:
    typedef ABoxQueryExpr value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };


  /**
   * Base class for ABox Query objects, that is individuals and
   * variables.
   */
  class ABoxQueryObject : public ABoxQueryExpr
  {
  protected:
    /// protected ctor
    ABoxQueryObject(const Term& s, const std::string& n)
      : ABoxQueryExpr(s, n)
    { }

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
    /// the constants used for describing the nRQL variable type
    struct VariableType
      {
	static const unsigned noninjective = 0x1;
	static const unsigned substrate = 0x2;
      };

  protected:
    /// the type of this nRQL query variable
    unsigned typeFlags;

    std::ostream&
    output(std::ostream& s) const;

   public:
    explicit
    ABoxQueryVariable(const Term& name, unsigned tf = 0)
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
    ABoxQueryIndividual(const Term& name,
			const std::string& nsid = std::string())
      : ABoxQueryObject(name, nsid)
    { }

    typedef ABoxQueryIndividual value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };



  /**
   * Base class for atomic and complex concepts.
   */
  class ABoxConceptDescrExpr : public ABoxQueryExpr
  {
  protected:
    /// protected default ctor
    ABoxConceptDescrExpr()
    { }

    /// protected ctor
    ABoxConceptDescrExpr(const Term& s, const std::string& n)
      : ABoxQueryExpr(s, n)
    { }

  public:
    typedef ABoxConceptDescrExpr value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };


  /**
   * Base class for atomic and complex roles.
   */
  class ABoxRoleDescrExpr : public ABoxQueryExpr
  {
  protected:
    /// protected default ctor
    ABoxRoleDescrExpr()
    { }

    /// protected ctor
    ABoxRoleDescrExpr(const Term& s, const std::string& n)
      : ABoxQueryExpr(s, n)
    { }

  public:
    typedef ABoxRoleDescrExpr value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };


  /**
   * An atomic Concept expression.
   */
  class ABoxQueryConcept : public ABoxConceptDescrExpr
  {
  public:
    explicit
    ABoxQueryConcept(const Term& name,
		     const std::string& nsid = std::string())
      : ABoxConceptDescrExpr(name, nsid)
    { }
  };


  /**
   * A negated concept expression.
   */
  class ABoxNegatedConcept : public ABoxConceptDescrExpr
  {
  private:
    ABoxConceptDescrExpr::shared_pointer cExpr;

    std::ostream&
    output(std::ostream& s) const;

  public:
    explicit
    ABoxNegatedConcept(ABoxConceptDescrExpr::const_pointer c)
      : cExpr(c)
    { }
  };


  /**
   * An existential quantified concept expression.
   */
  class ABoxSomeConcept : public ABoxConceptDescrExpr
  {
  private:
    ABoxRoleDescrExpr::shared_pointer rExpr;
    ABoxConceptDescrExpr::shared_pointer cExpr;

    std::ostream&
    output(std::ostream& s) const;

  public:
    ABoxSomeConcept(ABoxRoleDescrExpr::const_pointer r,
		    ABoxConceptDescrExpr::const_pointer c)
      : rExpr(r), cExpr(c)
    { }
  };


  /**
   * An one-of concept expression.
   */
  class ABoxOneOfConcept : public ABoxConceptDescrExpr
  {
  public:
    typedef boost::ptr_vector<ABoxQueryIndividual> IndividualVector;

  private:
    IndividualVector list;

    std::ostream&
    output(std::ostream& s) const;

  public:
    ABoxOneOfConcept(IndividualVector& l)
      : list(l.release()) // release the managed content of l and move it to list
    { }
  };


  /**
   * An atomic Role expression.
   */
  class ABoxQueryRole : public ABoxRoleDescrExpr
  {
  private:
    /// flag takes care of datatype role querying
    bool isDatatype;

    std::ostream&
    output(std::ostream& s) const;

  public:
    explicit
    ABoxQueryRole(const Term& name,
		  const std::string& nsid = std::string(),
		  bool d = false)
      : ABoxRoleDescrExpr(name, nsid),
	isDatatype(d)
    { }
  };


  /**
   * An inverted role expression.
   */
  class ABoxInvertedRole : public ABoxRoleDescrExpr
  {
  private:
    ABoxQueryRole::shared_pointer rExpr;

    std::ostream&
    output(std::ostream& s) const;

  public:
    explicit
    ABoxInvertedRole(ABoxQueryRole::const_pointer r)
      : rExpr(r)
    { }
  };


  /**
   * Base class for ABox assertions.
   */
  class ABoxAssertion : public QueryExpr
  {
  protected:
    /// protected default ctor
    ABoxAssertion()
    { }

  public:
    typedef ABoxAssertion value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };


  /**
   * instance assertion.
   */
  class ABoxInstanceAssertion : public ABoxAssertion
  {
  private:
    const ABoxConceptDescrExpr::shared_pointer cExpr;
    const ABoxQueryIndividual::shared_pointer iExpr;

  protected:
    std::ostream&
    output(std::ostream& s) const;

  public:
    ABoxInstanceAssertion(ABoxConceptDescrExpr::const_pointer c,
			  ABoxQueryIndividual::const_pointer i)
      : cExpr(c), iExpr(i)
    { }
  };


  /**
   * related assertion.
   */
  class ABoxRelatedAssertion : public ABoxAssertion
  {
  private:
    const ABoxRoleDescrExpr::shared_pointer rExpr;
    const ABoxQueryIndividual::shared_pointer i1Expr;
    const ABoxQueryIndividual::shared_pointer i2Expr;

  protected:
    std::ostream&
    output(std::ostream& s) const;

  public:
    ABoxRelatedAssertion(ABoxRoleDescrExpr::const_pointer r,
			 ABoxQueryIndividual::const_pointer i1,
			 ABoxQueryIndividual::const_pointer i2)
      : rExpr(r), i1Expr(i1), i2Expr(i2)
    { }
  };


  /**
   * add concept assertion.
   */
  class ABoxAddConceptAssertion : public ABoxAssertion
  {
  private:
    const ABoxConceptDescrExpr::shared_pointer cExpr;
    const ABoxQueryIndividual::shared_pointer iExpr;
    const std::string& abox;

  protected:
    std::ostream&
    output(std::ostream& s) const;

  public:
    ABoxAddConceptAssertion(ABoxConceptDescrExpr::const_pointer c,
			    ABoxQueryIndividual::const_pointer i,
			    const std::string& a)
      : cExpr(c), iExpr(i), abox(a)
    { }
  };


  /**
   * add role assertion.
   */
  class ABoxAddRoleAssertion : public ABoxAssertion
  {
  private:
    const ABoxRoleDescrExpr::shared_pointer rExpr;
    const ABoxQueryIndividual::shared_pointer i1Expr;
    const ABoxQueryIndividual::shared_pointer i2Expr;
    const std::string& abox;

  protected:
    std::ostream&
    output(std::ostream& s) const;

  public:
    ABoxAddRoleAssertion(ABoxRoleDescrExpr::const_pointer r,
			 ABoxQueryIndividual::const_pointer i1,
			 ABoxQueryIndividual::const_pointer i2,
			 const std::string& a)
      : rExpr(r), i1Expr(i1), i2Expr(i2), abox(a)
    { }
  };


  /**
   * Base class for simple and complex query expressions.
   */
  class ABoxQueryAtom : public QueryExpr
  {
  protected:
    /// protected default ctor
    ABoxQueryAtom()
    { }

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
    output(std::ostream& s) const;

  public:
    explicit
    NegationQuery(ABoxQueryAtom::const_pointer a)
      : atom(a)
    { }
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
    output(std::ostream& s) const;

  public:
    explicit
    InvertedQuery(ABoxQueryAtom::const_pointer a)
      : atom(a)
    { }
  };


  /**
   * A default negated query expression.
   */
  class NAFQuery : public ABoxQueryAtom
  {
  private:
    const ABoxQueryAtom::shared_pointer atom;

  protected:
    std::ostream&
    output(std::ostream& s) const;

  public:
    explicit
    NAFQuery(ABoxQueryAtom::const_pointer a)
      : atom(a)
    { }
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
    output(std::ostream& s) const;

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
    output(std::ostream& s) const;

  public:
    RoleQuery(ABoxQueryRole::const_pointer r,
	      ABoxQueryObject::const_pointer o1,
	      ABoxQueryObject::const_pointer o2)
      : rExpr(r), o1Expr(o1), o2Expr(o2)
    { }
  };


  /**
   * the same-as query atom.
   */
  class SameAsQuery : public ABoxQueryAtom
  {
  private:
    const ABoxQueryObject::shared_pointer o1Expr;
    const ABoxQueryObject::shared_pointer o2Expr;

  protected:
    std::ostream&
    output(std::ostream& s) const;

  public:
    SameAsQuery(ABoxQueryObject::const_pointer o1,
		ABoxQueryObject::const_pointer o2)
      : o1Expr(o1), o2Expr(o2)
    { }
  };



} // namespace racer
} // namespace dl
} // namespace dlvhex


#include "RacerQueryExpr.tcc"


#endif /* _RACERQUERYEXPR_H */


// Local Variables:
// mode: C++
// End:
