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

#include <boost/shared_ptr.hpp>

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


namespace dlvhex {
  namespace racer {

    struct DLRewriter::RewriteRule
    {
      unsigned extAtomNo;	/**< every external atom has its own number */
      char opChar;		/**< either 'p'(lus) or 'm'(inus) operator */
      std::string lhs;		/**< left term of operator */
      std::string rhs;		/**< right term of operator */
    };


    // strips leading and trailing space chars
    void
    strip(std::string& s)
    {
      std::string::size_type skip = s.find_first_not_of(' ');
      s = s.substr(skip, s.find_first_of(' ', skip) - skip);
    }

  }
}


std::string
DLRewriter::filterInput(const std::string& inputStr)
{
  unsigned i = 3; // skip "DL["

  // needed to distinguish ',' in input list vs. ',' in output list
  std::string::size_type brbegin = inputStr.rfind('(');

  while (i < inputStr.length())
    {
      std::string::size_type pm = inputStr.find('=', i);

      if (pm != std::string::npos) // we have a += or -= operator
	{
	  char c = inputStr[pm - 1] == '+' ? 'p' : 'm';

	  std::string lhs;
	  std::string rhs;
	  
	  lhs = inputStr.substr(i, pm - i - 1); // skip trailing '-' resp. '+'
	  std::string::size_type sc = inputStr.find(',', i);
	  
	  // is comma inside input list?
	  if (sc != std::string::npos && sc < brbegin)
	    {
	      rhs = inputStr.substr(pm + 1, sc - pm - 1); // skip leading '='
	      i = sc + 1;
	    }
	  else
	    {
	      // ',' outside input list -> find query delimiter ';'
	      std::string::size_type b = inputStr.find(';', i);
	      rhs = inputStr.substr(pm + 1, b - pm - 1); // skip leading '='
	      i = b + 1;
	    }

	  // strip whitespace
	  // @todo use boost string algorithms?
	  strip(lhs);
	  strip(rhs);

	  // append a new RewriteRule
	  boost::shared_ptr<RewriteRule> r(new RewriteRule);
	  r->opChar = c;
	  r->extAtomNo = this->extAtomNo;
	  r->lhs = lhs;
	  r->rhs = rhs;

	  rules.push_back(r);
	}
      else // no '=' found, we have a query now and are done
	{
	  std::string::size_type b = inputStr.find(']', i);
	  std::string query = inputStr.substr(i, b - i);

	  // strip whitespace
	  strip(query);

	  return "\"" + query + "\"";
	}
    }

  // we should not end up here
  return "";
}


void
DLRewriter::rewriteLine(const std::string& line)
{
  std::string::size_type i = 0;

  while (i < line.length())
    {
      // from position i, get the beginning and the end of a dl-atom
      std::string::size_type begin = line.find("DL[", i);
      std::string::size_type end = line.find(")", begin);

      if (begin != std::string::npos && end != std::string::npos) // found dl-Atom
	{
	  getOutput() << line.substr(i, begin - i); // output preceeding string

	  std::string dlAtom = line.substr(begin, end - begin + 1);

	  // get beginning of output list
	  std::string::size_type obeg = dlAtom.rfind('(');

	  // either a concept or a role dl-atom
	  if (dlAtom.find(',', obeg) == std::string::npos)
	    {
	      getOutput() << "&dlC";
	    }
	  else
	    {
	      getOutput() << "&dlR";
	    }

	  // collect rules + get query string
	  std::string query = filterInput(dlAtom);

	  // output input list
	  getOutput() << "[\"" 
		      << uri 
		      << "\",dl_pc_"
		      << extAtomNo
		      << ",dl_mc_"
		      << extAtomNo
		      << ",dl_pr_"
		      << extAtomNo
		      << ",dl_mr_"
		      << extAtomNo
		      << ","
		      << query
		      << "]";

	  // now append output list
	  std::string::size_type oend = dlAtom.find(')', obeg);

	  getOutput() << dlAtom.substr(obeg, oend - obeg + 1);

	  //
	  // now we are done, set new beginning position and increment
	  // external atom counter
	  //

	  i = end + 1;

	  this->extAtomNo++;
	}
      else
	{
	  // no dl-atom found from position i, output rest of line
	  getOutput() << line.substr(i);
	  // we're done, stop scanning line
	  i = line.length();
	}
    }
}



void
DLRewriter::rewrite()
{
  std::set<Term> concepts;
  std::set<Term> roles;
  Query q;

  OWLParser p(uri);
  p.parseNames(concepts, roles);
  p.parseNamespace(q);

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
	      // comment line found, hit and run
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

  // output found rules

  for (std::vector<boost::shared_ptr<RewriteRule> >::const_iterator it = rules.begin();
       it != rules.end(); it++)
    {
      std::string s = q.getNamespace() + (*it)->lhs;

      if (concepts.find(Term(s)) != concepts.end())
	{
	  getOutput() << "dl_" << (*it)->opChar
		      << "c_"
		      << (*it)->extAtomNo
		      << "(\""
		      << (*it)->lhs
		      << "\",X) :- "
		      << (*it)->rhs
		      << "(X)."
		      << std::endl;
	}
      else
	{
	  getOutput() << "dl_" << (*it)->opChar
		      << "r_"
		      << (*it)->extAtomNo
		      << "(\""
		      << (*it)->lhs
		      << "\",X,Y) :- "
		      << (*it)->rhs
		      << "(X,Y)."
		      << std::endl;
	}
    }

  extAtomNo = 0;
  rules.clear();
}
