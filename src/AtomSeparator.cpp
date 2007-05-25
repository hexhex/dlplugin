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
 * @file   AtomSeparator.cpp
 * @author Thomas Krennwallner
 * @date   Wed Oct 11 13:24:22 2006
 * 
 * @brief  Separates atoms in a string.
 * 
 * 
 */


#include "AtomSeparator.h"

#include <dlvhex/AtomSet.h>
#include <dlvhex/Atom.h>
#include <dlvhex/Term.h>

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/replace.hpp>


using namespace dlvhex::dl;


namespace dlvhex {
namespace dl {

  /**
   * @brief A TokenizerFunc for boost::tokenizer<>, model of
   * TokenizerFunction, tokenizes comma-separated atoms in a string.
   *
   * @see boost::tokenizer
   * http://www.boost.org/libs/tokenizer/index.html
   */
  class AtomSeparatorFunc
  {
  private:
    enum ScannerState
      {
	INITIAL = 0,
	PARENOPEN = 1,
	PARENCLOSE = 2
      };
    
    /// internal state keeps track of opened/closed parentheses
    ScannerState state;
    
  public:
    AtomSeparatorFunc() : state(INITIAL) { }
    
    /** 
     * Tokenizes list of atoms found in next and appends each atom
     * to tok.
     *
     * @param next
     * @param end
     * @param tok
     * 
     * @return true if a full atom could be parsed, false otherwise.
     */
    template <typename InputIterator, typename Token>
    bool
    operator() (InputIterator& next, InputIterator end, Token& tok)
      throw (DLParsingError)
    {
      tok = Token();
      
      if (next == end)
	{
	  if (state == PARENCLOSE) return false;
	  else throw DLParsingError("No closing parenthesis found.");
	}
      
      bool unexpected = false;
      
      for (; next != end; ++next)
	{
	  switch (*next)
	    {
	    case '(':
	      if (state == INITIAL) { state = PARENOPEN; tok += *next; }
	      else unexpected = true;
	      break;
	      
	    case ')':
	      if (state == PARENOPEN) { state = PARENCLOSE; tok += *next; }
	      else unexpected = true;
	      break;
	      
	    case ',':
	      if (state == PARENOPEN) { tok += *next; } // ignore
	      else if (state == PARENCLOSE) { reset(); ++next; return true; } // done
	      else unexpected = true;
	      break;
	      
	    case ' ':
	    case '\t':
	      tok += *next; // always ignore whitespaces
	      break;
	      
	    default:
	      if (state == INITIAL || state == PARENOPEN) { tok += *next; }
	      else unexpected = true;
	      break;
	    }
	  
	  if (unexpected)
	    throw DLParsingError("unexpected " + *next);
	}
      
      if (state == PARENCLOSE)
	{
	  return true;
	}
      
      return false;
    }
    
    void
    reset()
    {
      state = INITIAL;
    }
    
  };
  
} // namespace dl
} // namespace dlvhex



AtomSeparator::AtomSeparator(const std::string& al, AtomSet& as)
  : atomlist(al), atoms(as)
{ }


void
AtomSeparator::parse() throw (DLParsingError)
{
  std::string predicate;
  Tuple tup;

  // tokenize the atoms of the query string
  boost::tokenizer<AtomSeparatorFunc> tok(atomlist);

  for (boost::tokenizer<AtomSeparatorFunc>::const_iterator it = tok.begin();
       it != tok.end(); ++it)
    {
      predicate.clear();
      tup.clear();

      std::string atom = *it;

      //
      // determine the namespace prefix and replace every
      // abbr. name by the corresponding fully-fledged
      // namespace which can be found in Term::namespaces
      //
      for (std::vector<std::pair<std::string,std::string> >::iterator ns = Term::namespaces.begin();
	   ns != Term::namespaces.end();
	   ++ns)
	{
	  boost::replace_all(atom, ns->second + ":", ns->first);
	}

      std::string::size_type pred = atom.find('(');
      std::string::size_type t1 = atom.find(',');
      std::string::size_type t2 = atom.find(')');

      predicate = atom.substr(0, pred);
      boost::trim(predicate);

      bool isNegated = (predicate[0] == '-');
      
      if (isNegated) // remove '-' from predicate name
	{
	  predicate.erase(0, 1);
	}

      if (t1 != std::string::npos)
	{
	  std::string a1 = atom.substr(pred + 1, t1 - pred - 1);
	  std::string a2 = atom.substr(t1 + 1, t2 - t1 - 1);
	  
	  boost::trim(a1);
	  boost::trim(a2);
	  
	  tup.push_back(Term(a1));
	  tup.push_back(Term(a2));
	}
      else
	{
	  std::string a1 = atom.substr(pred + 1, t2 - pred - 1);
	  boost::trim(a1);
	  tup.push_back(Term(a1));
	}

      // ap is always first-order, otherwise we would end up in a
      // higher-order atom if concept or role name is uppercase
      AtomPtr ap(new Atom(predicate, tup, isNegated));
      ap->setAlwaysFO();
      atoms.insert(ap);
    }
}


UnionAtomSeparator::UnionAtomSeparator(const std::string& al, std::vector<AtomSet>& as)
  : unionatomlist(al), atoms(as)
{ }


void
UnionAtomSeparator::parse() throw (DLParsingError)
{
  const char* vel = " v ";
  std::string::size_type i = 0;
  std::string::size_type off = 0;

  while ((off = unionatomlist.find(vel, i)) != std::string::npos)
    {
      const std::string& q = unionatomlist.substr(i, off - i);

      // parse cq
      AtomSet as;
      AtomSeparator asep(q, as);
      asep.parse();
      atoms.push_back(as);

      i = off + 3; // skip " v " in the next round
    }

  // last " v " passed, get rest of string

  const std::string& q = unionatomlist.substr(i);

  AtomSet as;
  AtomSeparator asep(q, as);
  asep.parse();
  atoms.push_back(as);
}


// Local Variables:
// mode: C++
// End:
