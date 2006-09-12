/* -*- C++ -*- */

/**
 * @file   URI.cpp
 * @author Thomas Krennwallner
 * @date   Fri Sep  8 09:49:13 2006
 * 
 * @brief  URI convenience class.
 * 
 * 
 */


#include "URI.h"

#include <dlvhex/Term.h>
#include <string>
#include <algorithm>
#include <cctype>

using namespace dlvhex::dl;


URI::URI(const std::string& u)
{
  setupURI(u);
}


URI::URI(const Term& t)
{
  std::string s = t.getUnquotedString();

  if (s[0] == '-')
    {
      s.erase(0);
      setupURI(s);
      uri.insert(0, 1, '-');
    }
  else
    {
      setupURI(s);
    }
}


void
URI::setupURI(const std::string& u)
{
  if (u.find("file://") != 0 && u.find("file:") != 0 && u.find("http://") != 0)
    {
      // must be a plain pathname
      uri = "file:" + u;
    }
  else if (u.find("file://") == 0) // this needs a special massage
    {
      uri = "file:" + u.substr(7);
    }
  else if (u.find("file:") == 0) // use uri as is
    {
      uri = u;
    }
  else // a non-local URI
    {
      uri = u;
    }
}


namespace dlvhex {
  namespace dl {

    ///@brief predicate returns true iff argument is not an
    ///alpha-numeric and not a {+,-,.} character
    struct IsNotAlNumPMD : public std::unary_function<char, bool>
    {
      bool
      operator() (char c)
      {
	c = std::toupper(c);
	return
	  (c < 'A' || c > 'Z') &&
	  (c < '0' || c > '9') &&
	  c != '+' &&
	  c != '-' &&
	  c != '.';
      }
    };

  } // namespace dl
} // namespace dlvhex

bool
URI::isValid(const std::string& s)
{
  if (s.length() < 2)
    {
      return false;
    }

  // URI scheme part: [A-Za-z][A-Za-z0-9+-.]*

  std::string::const_iterator it;

  // find first non-alphanumeric occurence in s, i.e. skip the URI
  // scheme part which consists only of alphanumeric and +/-/. chars

  char c0 = std::toupper(s[0]);
  char c1 = std::toupper(s[1]);

  if (c0 == '-' && c1 >= 'A' && c1 <= 'Z') // maybe a negated URI?
    {
      it = std::find_if(s.begin() + 2, s.end(), IsNotAlNumPMD());
    }
  else if (c0 >= 'A' && c0 <= 'Z') // first char in range ['A','Z']
    {
      it = std::find_if(s.begin() + 1, s.end(), IsNotAlNumPMD());
    }
  else
    {
      return false;
    }

  // URI is valid if found non-alphanumeric occurence is a ':' character
  return it != s.end() && *it == ':';
}

bool
URI::isValid(const Term& t)
{
  if (t.isVariable() || t.isAnon() || t.isInt())
    {
      return false;
    }
  else // term is a symbol or string
    {
      return URI::isValid(t.getUnquotedString());
    }
}
