/* -*- C++ -*- */

/**
 * @file   HexDLRewriterDriver.cpp
 * @author Thomas Krennwallner
 * @date   Tue Jul 25 09:23:09 2006
 * 
 * @brief  
 * 
 * 
 */

#include "HexDLRewriterDriver.h"
#include "HexDLRewriterFlexLexer.h"
#include "OWLParser.h"

#include <iosfwd>

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>

using namespace dlvhex::racer;

HexDLRewriterDriver::HexDLRewriterDriver(std::istream& i, std::ostream& o)
  : PluginRewriter(i, o),
    lexer(new HexDLRewriterFlexLexer(this)),
    uri(""),
    extAtomNo(0)
{
  lexer->switch_streams(&i, &o);
}


HexDLRewriterDriver::~HexDLRewriterDriver()
{
  delete lexer;
}


HexDLRewriterFlexLexer*
HexDLRewriterDriver::getLexer() const
{
  return lexer;
}


std::ostream&
HexDLRewriterDriver::getOutput() const
{
  return *output;
}


void
HexDLRewriterDriver::setUri(const std::string& s)
{
  this->uri = s;
}


void
HexDLRewriterDriver::setStreams(std::istream* i, std::ostream* o)
{
  input = i;
  output = o;
  getLexer()->switch_streams(i, o);
}


void
HexDLRewriterDriver::rewrite()
{
  if (uri.empty())
    {
      // nothing to do, just swap rdbufs of input and output stream

      std::streambuf* tmp = input->rdbuf();
      input->rdbuf(output->rdbuf());
      output->rdbuf(tmp);

      return;
    }

  std::set<Term> concepts;
  std::set<Term> roles;
  std::string defaultNS;

  try
    {
      OWLParser p(uri);
      p.parseNames(concepts, roles);
      p.parseNamespace(defaultNS);
    }
  catch (RacerParsingError&)
    {
      // as for now, just ignore it
    }


  // and now parse that thing

  while (getLexer()->yylex() != 0) { /* no-op */ }


  // output rewritten dl-atoms

  for (boost::ptr_vector<RewriteDLAtom>::const_iterator it = rewrittenDLAtoms.begin();
       it != rewrittenDLAtoms.end(); it++)
    {
      std::ostringstream aux;

      aux << "dl_" << it->opChar;

      std::string s = defaultNS + it->lhs;
      Term t(s);

      if (concepts.find(t) != concepts.end())
	{
	  aux << "c_" << it->extAtomNo;

	  *output << aux.str()
		  << "(\""
		  << it->lhs
		  << "\",X) :- "
		  << it->rhs
		  << "(X)."
		  << std::endl;
	}
      else if (roles.find(t) != roles.end())
	{
	  aux << "r_" << it->extAtomNo;

	  *output << aux.str()
		  << "(\""
		  << it->lhs
		  << "\",X,Y) :- "
		  << it->rhs
		  << "(X,Y)."
		  << std::endl;
	}
      else
	{
	  std::ostringstream err;
	  err << "Couldn't rewrite DL atom "
	      << dlAtoms[it->extAtomNo]
	      << ", "
	      << s
	      << " is not a concept and not a role.";
	  throw PluginError(err.str());
	}

      // we don't want dl_XY_N to show up in the answer set
      Term::registerAuxiliaryName(aux.str());
    }

  // reset counter and rewritten dl-atoms / input dl-atoms
  extAtomNo = 0;
  rewrittenDLAtoms.clear();
  dlAtoms.clear();
}


std::string
HexDLRewriterDriver::rewriteDLAtom(const std::string& dlAtom)
{
  // we found a new DL Atom
  dlAtoms[this->extAtomNo] = dlAtom;

  // needed to distinguish ',' in input list vs. ',' in output list
  std::string::size_type brbegin = dlAtom.rfind(']');

  // std::cerr << "l: " << dlAtom << ' ' << dlAtom.substr(3, brbegin - 3) << std::endl;

  // tokenize the input list of the dl-atom string, i.e. start from
  // position 3 (skip "DL[") up to position brbegin
  boost::tokenizer<boost::char_separator<char> > tok
    (dlAtom.substr(3, brbegin - 3),
     boost::char_separator<char>(",;") // input list separator
     );

  // the query string
  std::string query;

  // get each item from the input list
  for (boost::tokenizer<boost::char_separator<char> >::const_iterator it = tok.begin();
       it != tok.end(); it++)
    {
      std::string::size_type pm = it->find('=');

      if (pm != std::string::npos) // we have a += or -= operator
	{
	  char c = (*it)[pm - 1] == '+' ? 'p' : 'm';

	  std::string lhs = it->substr(0, pm - 1); // skip trailing '-' resp. '+'
	  std::string rhs = it->substr(pm + 1);    // skip preceding '='

	  // strip whitespace
	  boost::trim(lhs);
	  boost::trim(rhs);

	  // std::cerr << "eq: " << extAtomNo << ' ' << c << ' ' << lhs << ' ' << rhs << std::endl;

	  // append a fresh RewriteRule
	  rewrittenDLAtoms.push_back
	    (new RewriteDLAtom(this->extAtomNo, c, lhs, rhs)
	     );
	}
      else // no '=' found, we have got a query and are done
	{
	  query = *it;

	  // std::cerr << "q: " << query << ' ' << *it << std::endl;

	  // strip whitespace
	  boost::trim(query);

	  // std::cerr << "q: " << query << ' ' << *it << std::endl;

	  break;
	}
    }

  // first commandment: stream thy strings
  std::ostringstream extAtom;

  // either a concept or a role external dl-atom
  if (dlAtom.find(',', brbegin) == std::string::npos)
    {
      extAtom << "&dlC";
    }
  else
    {
      extAtom << "&dlR";
    }

  // output external atoms input list
  extAtom << "[\"" 
	  << uri 
	  << "\",dl_pc_"
	  << extAtomNo
	  << ",dl_mc_"
	  << extAtomNo
	  << ",dl_pr_"
	  << extAtomNo
	  << ",dl_mr_"
	  << extAtomNo
	  << ",\""
	  << query
	  << "\"]";

  // append output list of the ext. atom
  extAtom << dlAtom.substr(brbegin + 1);

  //
  // now we are done, increment external atom counter and return the
  // external atom
  //

  this->extAtomNo++;

  return extAtom.str();
}
