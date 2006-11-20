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

#include "HexDLDriver.h"
#include "HexDLFlexLexer.h"
#include "Ontology.h"
#include "Registry.h"
#include "DLError.h"

#include <sstream>

using namespace dlvhex::dl;


HexDLDriver::HexDLDriver()
  : PluginConverter(),
    lexer(new HexDLFlexLexer(this)),
    ontology(),
    output(&std::cerr)
{ }


HexDLDriver::HexDLDriver(const HexDLDriver& d)
  : PluginConverter(),
    lexer(new HexDLFlexLexer(this)),
    ontology(d.ontology),
    output(d.output)
{ }


HexDLDriver&
HexDLDriver::operator= (const HexDLDriver& d)
{
  if (this != &d)
    {
      delete lexer;
      lexer = new HexDLFlexLexer(this);
      ontology = d.ontology;
      output = d.output;
    }

  return *this;
}


HexDLDriver::~HexDLDriver()
{
  delete this->lexer;
}


HexDLFlexLexer*
HexDLDriver::getLexer() const
{
  return this->lexer;
}


std::ostream&
HexDLDriver::getOutput() const
{
  return *this->output;
}


void
HexDLDriver::setURI(const std::string& s)
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
HexDLDriver::getOntology() const
{
  return this->ontology;
}


void
HexDLDriver::convert(std::istream& input, std::ostream& output)
{
  //
  // setup streams
  //

  getLexer()->switch_streams(&input, &output);
  this->output = &output;

  //
  // parse and rewrite that thing to HEX syntax
  //

  DLAtomInput dlinput;


  try
    {
      yy::HexDLParser parser(*this, dlinput);
      getLexer()->set_debug(Registry::getVerbose() > 2);
      parser.set_debug_level(Registry::getVerbose() > 2);
      parser.parse();

      //
      // alright, we are done rewriting all dl-atoms, so lets append
      // the input rules to the output stream
      //

      std::vector<Rule*> rules = dlinput.getDLInputRules();

      for (std::vector<Rule*>::const_iterator it = rules.begin(); it != rules.end(); ++it)
	{
	  getOutput() << *(*it) << std::endl;
	  delete *it;
	}
    }
  catch (DLParsingError& e)
    {
      throw PluginError(e.what());
    }
}


void
HexDLDriver::error(const yy::location& l, const std::string& m) const
  throw (DLParsingError)
{
  std::ostringstream s;
  s << "Parsing error at " << l << ": " << m;

  throw DLParsingError(s.str());
}
