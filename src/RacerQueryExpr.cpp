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
 * @file   RacerQueryExpr.cpp
 * @author Thomas Krennwallner
 * @date   Tue Apr 18 16:41:34 2006
 * 
 * @brief  Class hierarchy for basic query expressions.
 * 
 */

#include "RacerQueryExpr.h"
#include "URI.h"

#include <dlvhex/Term.hpp>
#include <dlvhex/ComfortPluginInterface.hpp>

#include <iosfwd>
#include <string>
#include <sstream>

using namespace dlvhex::dl::racer;


std::ostream&
ABoxQueryExpr::output(std::ostream& s) const
{
  std::string sym;

  if (symbol.isConstant())
    {
      sym = symbol.getUnquotedString();

      if (sym[0] == '-')
	{
	  ///@todo we have to check if the remainder is enclosed in
	  ///quotes... very ugly

	  std::string::size_type last = sym.size() - 1;

	  // remove unwanted quotes
	  if (sym[1] == '"' && sym[last] == '"')
	    {
	      sym = "-" + sym.substr(2, last - 2);
	    }
	}
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

  bool isuri = URI::isValid(sym);

  if (isuri) // symbol is a URI, no need to add namespace
    {
      return s << '|' << URI::getPlainURI(sym) << '|';
    }
  else // symbol is not a URI, check if we need to add the namespace
    {
      if (nsid.empty()) // no uri + no namespace = plain symbol
	{
	  return s << sym;
	}
      else // symbol + namespace = URI
	{
	  return s << '|' << nsid << (sym[0] == '-' ? sym.substr(1) : sym) << '|';
	}
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


// Local Variables:
// mode: C++
// End:
