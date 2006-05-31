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

using namespace dlvhex::racer;


namespace dlvhex {
namespace racer {

  std::ostream&
  operator<< (std::ostream& s, const QueryExpr& e)
  {
    return e.output(s);
  }

}
}


std::ostream&
ABoxQueryExpr::output(std::ostream& s) const
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


std::ostream&
ABoxQueryVariable::output(std::ostream& s) const
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


std::ostream&
ABoxNegatedConcept::output(std::ostream& s) const
{
  return s << "(not " << *cExpr << ')';
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
ABoxInstance::output(std::ostream& s) const
{
  return s << "(instance "
	   << *iExpr
	   << " "
	   << *cExpr
	   << ")";
}


std::ostream&
ABoxRelated::output(std::ostream& s) const
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
