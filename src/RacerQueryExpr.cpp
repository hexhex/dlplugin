/* -*- C++ -*- */

/**
 * @file   RacerQueryExpr.cpp
 * @author Thomas Krennwallner
 * @date   Tue Apr 18 16:41:34 2006
 * 
 * @brief  Class hierarchy for basic query expressions.
 * 
 */

#include "RacerQueryExpr.h"

#include <iosfwd>
#include <string>

using namespace dlvhex::dl::racer;


namespace dlvhex {
namespace dl {
namespace racer {

  std::ostream&
  operator<< (std::ostream& s, const QueryExpr& e)
  {
    return e.output(s);
  }

}
}
}


std::ostream&
ABoxQueryExpr::output(std::ostream& s) const
{
  ///@todo we cannot assume that # is in every URI individual

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


std::ostream&
ABoxQueryVariable::output(std::ostream& s) const
{
  return s << (typeFlags & VariableType::noninjective ? "$" : "")
	   << '?'
	   << (typeFlags & VariableType::substrate ? "*" : "")
	   << symbol;
}


std::ostream&
ABoxNegatedConcept::output(std::ostream& s) const
{
  return s << "(not " << *cExpr << ')';
}


std::ostream&
ABoxSomeConcept::output(std::ostream& s) const
{
  return s << "(some " << *rExpr << ' ' << *cExpr << ')';
}


std::ostream&
ABoxOneOfConcept::output(std::ostream& s) const
{
  return s << "(one-of " << list << ')';
}


std::ostream&
ABoxQueryRole::output(std::ostream& s) const
{
  if (isDatatype)
    {
      s << "(:owl-datatype-role ";
      ABoxQueryExpr::output(s);
      s << ')';
    }
  else
    {
      ABoxQueryExpr::output(s);
    }

  return s;
}


std::ostream&
ABoxInvertedRole::output(std::ostream& s) const
{
  return s << "(inv " << *rExpr << ')';
}


std::ostream&
ABoxInstanceAssertion::output(std::ostream& s) const
{
  return s << "(instance "
	   << *iExpr
	   << ' '
	   << *cExpr
	   << ')';
}


std::ostream&
ABoxRelatedAssertion::output(std::ostream& s) const
{
  return s << "(related "
	   << *i1Expr
	   << ' '
	   << *i2Expr
	   << ' '
	   << *rExpr
	   << ')';
}


std::ostream&
ABoxAddConceptAssertion::output(std::ostream& s) const
{
  return s << "(add-concept-assertion "
	   << abox
	   << ' '
	   << *iExpr
	   << ' '
	   << *cExpr
	   << ')';
}


std::ostream&
ABoxAddRoleAssertion::output(std::ostream& s) const
{
  return s << "(add-role-assertion "
	   << abox
	   << ' '
	   << *i1Expr
	   << ' '
	   << *i2Expr
	   << ' '
	   << *rExpr
	   << ')';
}


std::ostream&
NegationQuery::output(std::ostream& s) const
{
  return s << "(not " << *atom << ')';
}


std::ostream&
InvertedQuery::output(std::ostream& s) const
{
  return s << "(inv " << *atom << ')';
}


std::ostream&
ConceptQuery::output(std::ostream& s) const
{
  return s << '(' << *oExpr << ' ' << *cExpr << ')';
}


std::ostream&
RoleQuery::output(std::ostream& s) const
{
  return s << '(' << *o1Expr << ' ' << *o2Expr << ' '  << *rExpr << ')';
}


std::ostream&
SameAsQuery::output(std::ostream& s) const
{
  return s << "(same-as " << *o1Expr << ' ' << *o2Expr << ')';
}
