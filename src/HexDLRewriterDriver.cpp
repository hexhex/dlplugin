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
    ontology(),
    extAtomNo(0),
    rewrittenDLAtoms()
{
  lexer->switch_streams(&i, &o);
}


HexDLRewriterDriver::HexDLRewriterDriver(const HexDLRewriterDriver& d)
  : PluginRewriter(*d.input, *d.output),
    lexer(new HexDLRewriterFlexLexer(this)),
    ontology(d.ontology),
    extAtomNo(d.extAtomNo),
    rewrittenDLAtoms()
{
  lexer->switch_streams(d.input, d.output);
}


HexDLRewriterDriver&
HexDLRewriterDriver::operator= (const HexDLRewriterDriver& d)
{
  if (this != &d)
    {
      delete lexer;
      lexer = new HexDLRewriterFlexLexer(this);
      ontology = d.ontology;
      extAtomNo = d.extAtomNo;
      setStreams(d.input, d.output);
    }

  return *this;
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
  try
    {
      this->ontology = Ontology::createOntology(s);
    }
  catch (DLError& e)
    {
      throw PluginError(e.what());
    }
}


Ontology::shared_pointer
HexDLRewriterDriver::getOntology() const
{
  return this->ontology;
}


void
HexDLRewriterDriver::addExtAtomNo(int offset)
{
  this->extAtomNo += offset;
}


unsigned
HexDLRewriterDriver::getExtAtomNo() const
{
  return this->extAtomNo;
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
  // if we don't have an Ontology, we skip the dl-atom rewriting
  //
  
  if (!ontology)
    {
      return;
    }

  //
  // otherwise we get all concept and role names and add the
  // corresponding additional rules to the HEX program
  //

  TBox::ObjectsPtr concepts = ontology->getTBox().getConcepts();
  TBox::ObjectsPtr roles = ontology->getTBox().getRoles();
  TBox::ObjectsPtr datatypeRoles = ontology->getTBox().getDatatypeRoles();

  // output rewritten dl-atoms

  for (boost::ptr_vector<DLAtomOp>::const_iterator it = rewrittenDLAtoms.begin();
       it != rewrittenDLAtoms.end(); ++it)
    {
      std::ostringstream aux;

      aux << "dl_" << (it->op == DLAtomOp::plus ? 'p' : 'm');

      std::string s = ontology->getNamespace() + *(it->lhs); ///@todo always add namespace?
      Term t(s);

      if (concepts->find(t) != concepts->end())
	{
	  aux << "c_" << it->extAtomNo;

	  *output << aux.str()
		  << "(\""
		  << *(it->lhs)
		  << "\",X) :- "
		  << *(it->rhs)
		  << "(X)."
		  << std::endl;
	}
      else if (roles->find(t) != roles->end() || 
	       datatypeRoles->find(t) != datatypeRoles->end()
	       )
	{
	  aux << "r_" << it->extAtomNo;

	  *output << aux.str()
		  << "(\""
		  << *(it->lhs)
		  << "\",X,Y) :- "
		  << *(it->rhs)
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
HexDLRewriterDriver::registerDLOp(DLAtomOp* op)
{
  rewrittenDLAtoms.push_back(op);
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
