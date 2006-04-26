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

// #include <boost/regex.hpp>

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
  
}

void
DLRewriter::rewriteRole(const std::string& role)
{


}


void
DLRewriter::rewriteLine(const std::string& line)
{
  std::string::size_type i = 0;

  while (i < line.length())
    {
      std::string::size_type begin = line.find("DL[", i);
      std::string::size_type end = line.find(")", begin);

      if (begin != std::string::npos &&
	  end != std::string::npos)
	{
	  *out << line.substr(i, begin);

	  std::string dlAtom = line.substr(begin,end);
	  
	  if (dlAtom.find(',', dlAtom.find('(')) == std::string::npos)
	    {
	      rewriteConcept(dlAtom);
	    }
	  else
	    {
	      rewriteRole(dlAtom);
	    }

	  i = end;
	}
      else
	{
	  *out << line.substr(i);

	  i = line.length();
	}
    }
}


// bool
// regexCb(const boost::match_results<std::string::const_iterator>& what)
// {
//   int i = 0;

//   for (boost::match_results<std::string::const_iterator>::const_iterator it = what.begin();
//        it != what.end();
//        it++, i++)
//     {
//       if (it->matched)
// 	{
// 	  switch(i)
// 	    {
// 	    case 1: // comment line
// 	    case 4: // no dl-atom line
// 	      *out << *it;
// 	      return true;

// 	    case 2: // DL-atom line
// 	      rewriteAtoms(*it);
// 	      return true;

// 	    default: // skip what[0] and friends
// 	      break;
// 	    }
// 	}
//     }

//    return true;
// }

const char* DLRewriter::re =
// #1: comment lines
  "(^[[:space:]]*%.*$)|"
// #2+#3: lines with at least one DL-atom
  "(.*DL\\[[\\w;_+-= \t]+\\]\\(\"?[\\w_]+\"?(,\"?[\\w_]+\"?)?\\).*)|"
// #4: lines without an DL-atom
  "(.*[^D][^L][^\\[].*)";

void
DLRewriter::rewrite()
{
  std::set<Term> concepts;
  std::set<Term> roles;

  OWLParser p(uri);

  p.parseNames(concepts, roles);
  
//   boost::regex expression(DLRewriter::re);
  std::string line;
  std::stringbuf sb;

  while (in->good())
    {
      // ignore newlines, they screw with in stream
      while (in->peek() == '\n')
	{
	  *out << std::endl;
	  in->ignore();
	}

      sb.str("");
      in->get(sb, '\n');
      line = sb.str();

//       boost::sregex_iterator m1(line.begin(), line.end(), expression);
//       boost::sregex_iterator m2;

//       std::for_each(m1, m2, std::mem_fun(&DLRewriter::regexCb));
    }

  // output rules
}
