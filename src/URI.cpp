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
