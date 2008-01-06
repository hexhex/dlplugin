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

#include <cctype>   // toupper()
#include <cerrno>   // perror
#include <climits>  // PATH_MAX
#include <unistd.h> // getcwd()


namespace dlvhex {
  namespace dl {


URI::URI(const std::string& u, bool absolute)
  : uri()
{
  setupURI(u, absolute);
}


URI::URI(const Term& t, bool absolute)
  : uri()
{
  std::string s = t.getUnquotedString();

  if (s[0] == '-')
    {
      s.erase(0);
      setupURI(s, absolute);
      uri.insert(0, 1, '-');
    }
  else
    {
      setupURI(s, absolute);
    }
}


std::string
URI::getAbsolutePath(const std::string& s)
{
  if (s[0] != '/') // relative path
    {
      char buf[PATH_MAX];

      if (::getcwd(buf, PATH_MAX) == 0)
	{
	  std::perror("getcwd");
	  std::exit(1);
	}

      return buf + std::string("/") + s;
    }
  else // absolute path
    {
      return s;
    }
}


void
URI::setupURI(const std::string& u, bool absolute)
{
  if (u.find("file://") != 0 && u.find("file:") != 0 && u.find("http://") != 0)
    {
      // must be a plain pathname
      uri = "file:" + (absolute ? getAbsolutePath(u) : u);
    }
  else if (u.find("file://") == 0) // this needs a special massage
    {
      std::string tmp = u.substr(7);
      uri = "file:" + (absolute ? getAbsolutePath(tmp) : tmp);
    }
  else if (u.find("file:") == 0)
    {
      if (absolute)
	{
	  uri = "file:" + getAbsolutePath(u.substr(5));
	}
      else // use uri as is
	{
	  uri = u;
	}
    }
  else // use non-local URI as is
    {
      uri = u;
    }
}


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


bool
URI::isValid(const std::string& s)
{
  if (s.length() < 2)
    {
      return false;
    }

  int start = 0;
  int end = s.length() - 1;

  // first, we check if we have a negated uri

  if (s[start] == '-')
    {
      start++;
    }

  // then, check if s is in turtle syntax

  char b = s[start];
  char e = s[end];

  if (b == '<' && e == '>') // is in turtle syntax, hence substract the enclosing characters
    {
      start++;
      end--;
    }

  // and now check if we have a valid URI

  // URI scheme part: [A-Za-z][A-Za-z0-9+-.]*

  std::string::const_iterator it;

  // find first non-alphanumeric occurence in s, i.e. skip the URI
  // scheme part which consists only of alphanumeric and +/-/. chars

  char c0 = std::toupper(s[start]);

  if (c0 >= 'A' && c0 <= 'Z') // first char in range ['A','Z']
    {
      it = std::find_if(s.begin() + start + 1, s.begin() + end + 1, IsNotAlNumPMD());
    }
  else
    {
      return false;
    }

  // URI is valid if found non-alphanumeric occurence is a ':' character
  return it != (s.begin() + end + 1) && *it == ':';
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


std::string
URI::getPlainURI(const std::string& s)
{
  if (s.length() < 2)
    {
      return s;
    }

  int start = 0;
  int end = s.length() - 1;

  // first, we check if we have a negated uri

  if (s[start] == '-')
    {
      start++;
    }

  // then, check if s is in turtle syntax

  char b = s[start];
  char e = s[end];

  if (b == '<' && e == '>') // is in turtle syntax, hence substract the enclosing characters
    {
      start++;
      end--;
    }

  return s.substr(start, end - start + 1);
}



  } // namespace dl
} // namespace dlvhex


// Local Variables:
// mode: C++
// End:
