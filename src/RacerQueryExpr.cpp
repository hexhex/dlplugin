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
#include "URI.h"

#include <dlvhex/Term.h>

#include <iosfwd>
#include <string>
#include <sstream>

using namespace dlvhex::dl::racer;


std::ostream&
ABoxQueryExpr::output(std::ostream& s) const
{
  std::string sym;

  if (symbol.isString() || symbol.isSymbol())
    {
      sym = symbol.getUnquotedString();
    }
  else
    {
      std::ostringstream oss;
      oss << symbol;
      sym = oss.str();
    }

  //
  // Now check the possibilities for the symbol. If we have got an
  // URI, we have to embrace the symbol with '|' characters. If we
  // have got a '-' as first character, we ignore this since this is a
  // negated term.
  //

  if (!URI::isValid(symbol) && nsid.empty()) // no uri + no namespace = plain symbol
    {
      return s << sym;
    }
  else if (!URI::isValid(symbol) && !nsid.empty()) // symbol + namespace = URI
    {
      return s << '|' << nsid << (sym[0] == '-' ? sym.substr(1) : sym) << '|';
    }
  else // symbol is an URI, no need to add namespace
    {
      return s << '|' << (sym[0] == '-' ? sym.substr(1) : sym) << '|';
    }
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
NAFQuery::output(std::ostream& s) const
{
  return s << "(neg " << *atom << ')';
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
