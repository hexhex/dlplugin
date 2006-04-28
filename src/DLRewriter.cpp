/* -*- C++ -*- */

/**
 * @file   DLRewriter.cpp
 * @author Thomas Krennwallner
 * @date   Tue Apr 25 13:48:38 2006
 * 
 * @brief  
 * 
 * 
 */

#include "DLRewriter.h"
#include "OWLParser.h"

#include <algorithm>
#include <iterator>

using namespace dlvhex::racer;

DLRewriter::DLRewriter(std::istream& i, std::ostream& o)
  : PluginRewriter(i,o),
    in(&i),
    out(&o),
    extAtomNo(0)
{ }

void
DLRewriter::setUri(const std::string& s)
{
  this->uri = s;
}

void
DLRewriter::setStreams(std::istream* i, std::ostream* o)
{
  in = i;
  out = o;
}


std::istream&
DLRewriter::getInput() const
{
  return *in;
}

std::ostream&
DLRewriter::getOutput() const
{
  return *out;
}



struct RewriteRule
{
  unsigned extAtomNo;
  char opChar;
  std::string lhs;
  std::string rhs;
};




void
DLRewriter::rewriteConcept(const std::string& concept)
{
  //  getOutput() << "&dlC[\"" << uri << "\",";
  getOutput() << concept;

  unsigned i = 3; // skip DL[

  while (i < concept.length())
    {
      std::string::size_type pm = concept.find_first_of("=", i);

      if (pm != std::string::npos) // we have += or -= cmd
	{
	  char c = concept[pm - 1];

	  std::string lhs;
	  std::string rhs;
	  
	  lhs = concept.substr(i, pm - i - 1); // skip '-' resp '+'
	  std::string::size_type sc = concept.find(';');
	  
	  if (sc != std::string::npos)
	    {
	      rhs = concept.substr(pm + 1, sc - pm - 1); // skip '='
	      i = sc + 1;
	    }
	  else // is this possible?
	    {
	      std::string::size_type b = concept.find(']');
	      rhs = concept.substr(pm + 1, b - pm - 1);
	      i = b + 1;
	    }

	  // strip whitespace

	  std::string::size_type skip = lhs.find_first_not_of(' ');
	  lhs = lhs.substr(skip, lhs.find_first_of(' ', skip) - skip);

	  skip = rhs.find_first_not_of(' ');
	  rhs = rhs.substr(skip, lhs.find_first_of(' ', skip) - skip);

	  std::cout << c << ": " << lhs << " " << rhs << std::endl;
	}
      else
	{
	  std::string::size_type b = concept.find(']');
	  std::string query = concept.substr(i, b - i);

	  // strip whitespace

	  std::string::size_type skip = query.find_first_not_of(' ');
	  query = query.substr(skip, query.find_first_of(' ', skip) - skip);

	  std::cout << query << std::endl;

	  i = concept.length();
	}
    }
}


void
DLRewriter::rewriteRole(const std::string& role)
{
  getOutput() << role;
}


void
DLRewriter::rewriteLine(const std::string& line)
{
  std::string::size_type i = 0;

  while (i < line.length())
    {
      std::string::size_type begin = line.find("DL[", i);
      std::string::size_type end = line.find(")", begin);

      if (begin != std::string::npos && end != std::string::npos)
	{
	  getOutput() << line.substr(i, begin - i); // output preceeding string

	  std::string dlAtom = line.substr(begin, end - begin + 1);
	  
	  if (dlAtom.find(',', dlAtom.find('(')) == std::string::npos)
	    {
	      rewriteConcept(dlAtom);
	    }
	  else
	    {
	      rewriteRole(dlAtom);
	    }

	  i = end + 1;
	}
      else
	{
	  getOutput() << line.substr(i);

	  i = line.length();
	}
    }
}



void
DLRewriter::rewrite()
{
  std::set<Term> concepts;
  std::set<Term> roles;

  OWLParser p(uri);

  p.parseNames(concepts, roles);

  for (std::set<Term>::const_iterator it = concepts.begin();
       it != concepts.end(); it++)
    {
      std::cout << it->getUnquotedString() << std::endl;
    }

  std::string line;
  std::stringbuf sb;

  while (in->good())
    {
      // ignore newlines, they screw with in stream
      while (in->peek() == '\n')
	{
	  getOutput() << std::endl;
	  in->ignore();
	}

      sb.str("");
      in->get(sb, '\n');
      line = sb.str();

      std::string::size_type c = line.find('%');

      if (c != std::string::npos)
	{
	  if (line.substr(0, c - 1).find_first_not_of(" \t") != std::string::npos)
	    {
	      // comment line found
	      getOutput() << line;
	      continue;
	    }
	}

      std::string::size_type d = line.find("DL[");

      if (d != std::string::npos && c == std::string::npos)
	{
	  // there is a dl-atom in the line
	  rewriteLine(line);
	}
      else if (d != std::string::npos && c != std::string::npos)
	{
	  // dl-atom found with comment at the end
	  rewriteLine(line.substr(0, c));
	  getOutput() << line.substr(c);
	}
      else
	{
	  // no dl-atom found
	  getOutput() << line;
	}
    }

  // output rules
}
