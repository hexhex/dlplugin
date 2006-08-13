/* -*- C++ -*- */

/**
 * @file   HexDLRewriterDriver.cpp
 * @author Thomas Krennwallner
 * @date   Tue Jul 25 09:23:09 2006
 * 
 * @brief  Driver class for the Hex DL Rewriters bison/flex parser.
 * 
 * 
 */

#include "HexDLRewriterDriver.h"
#include "HexDLRewriterFlexLexer.h"
#include "Ontology.h"
#include "Registry.h"
#include "DLError.h"

#include <iosfwd>

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/erase.hpp>

using namespace dlvhex::dl;


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


void
HexDLRewriterDriver::reset()
{
  // reset counter and rewritten dl-atoms
  extAtomNo = 0;
  rewrittenDLAtoms.clear();
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
  //
  // parse and rewrite that thing
  //

  try
    {
      yy::HexDLRewriterParser parser(*this);
      parser.set_debug_level(Registry::getVerbose() > 2 ? true : false);
      parser.parse();
    }
  catch (DLParsingError& e)
    {
      throw PluginError(e.what());
    }

  //
  // if we don't have an URI, we skip the dl-atom rewriting
  //
  
  if (uri.empty())
    {
      return;
    }

  //
  // otherwise we get all concept and role names and add the
  // corresponding additional rules to the HEX program
  //

  Ontology::shared_pointer onto;

  try
    {
      onto = Ontology::createOntology(uri);
    }
  catch (DLError& e)
    {
      throw PluginError(e.what());
    }

  Ontology::ObjectsPtr concepts = onto->getConcepts();
  Ontology::ObjectsPtr roles = onto->getRoles();

  // output rewritten dl-atoms

  for (boost::ptr_vector<RewriteDLAtom>::const_iterator it = rewrittenDLAtoms.begin();
       it != rewrittenDLAtoms.end(); ++it)
    {
      std::ostringstream aux;

      aux << "dl_" << it->opChar;

      std::string s = onto->getNamespace() + it->lhs;
      Term t(s);

      if (concepts->find(t) != concepts->end())
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
      else if (roles->find(t) != roles->end())
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
	      << it->extAtomNo
	      << ", "
	      << s
	      << " is not a concept and not a role.";
	  throw PluginError(err.str());
	}

      // we don't want dl_XY_N to show up in the answer set
      Term::registerAuxiliaryName(aux.str());
    }

  reset();
}



void
HexDLRewriterDriver::registerDLOp(char op, const std::string& lhs, const std::string& rhs)
{
  if (op != 'p' && op != 'm')
    {
      return;
    }

  // append a fresh RewriteRule
  rewrittenDLAtoms.push_back(new RewriteDLAtom(this->extAtomNo, op, lhs, rhs));
}


std::string
HexDLRewriterDriver::rewriteDLAtom(const std::string& query, const std::string& t1)
{
  ///@todo let the HexDLRewriter bison parser rewrite the dl-atoms

  if (uri.empty())
    {
      throw PluginError("Couldn't rewrite dl-atom, ontology URI is empty.");
    }

  // first commandment: stream thy strings
  std::ostringstream extAtom;

  extAtom << "&dlC";

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
	  << ','
	  << (query[0] != '"' ? "\"" + query + "\"" : query)
	  << ']';

  // append output list of the ext. atom
  extAtom << '(' << t1 << ')';

  //
  // now we are done, increment external atom counter and return the
  // external atom
  //

  this->extAtomNo++;

  return extAtom.str();
}


std::string
HexDLRewriterDriver::rewriteDLAtom(const std::string& query,
				   const std::string& t1,
				   const std::string& t2)
{
  ///@todo let the HexDLRewriter bison parser rewrite the dl-atoms

  // first commandment: stream thy strings
  std::ostringstream extAtom;

  extAtom << "&dlR";

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
	  << ','
	  << (query[0] != '"' ? "\"" + query + "\"" : query)
	  << ']';

  // append output list of the ext. atom
  extAtom << '(' << t1 << ',' << t2 << ')';

  //
  // now we are done, increment external atom counter and return the
  // external atom
  //

  this->extAtomNo++;

  return extAtom.str();
}


void
HexDLRewriterDriver::error(const yy::location& l,
			   const std::string& m) const
  throw (DLParsingError)
{
  std::ostringstream s;
  s << "Parsing error at " << l << ": " << m;

  throw DLParsingError(s.str());
}
