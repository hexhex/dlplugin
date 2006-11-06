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


HexDLDriver::HexDLDriver(std::istream& i, std::ostream& o)
  : PluginRewriter(i, o),
    lexer(new HexDLFlexLexer(this)),
    ontology()
{
  lexer->switch_streams(&i, &o);
}


HexDLDriver::HexDLDriver(const HexDLDriver& d)
  : PluginRewriter(*d.input, *d.output),
    lexer(new HexDLFlexLexer(this)),
    ontology(d.ontology)
{
  lexer->switch_streams(d.input, d.output);
}


HexDLDriver&
HexDLDriver::operator= (const HexDLDriver& d)
{
  if (this != &d)
    {
      delete lexer;
      lexer = new HexDLFlexLexer(this);
      ontology = d.ontology;
      setStreams(d.input, d.output);
    }

  return *this;
}


HexDLDriver::~HexDLDriver()
{
  delete lexer;
}


HexDLFlexLexer*
HexDLDriver::getLexer() const
{
  return lexer;
}


std::ostream&
HexDLDriver::getOutput() const
{
  return *output;
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
HexDLDriver::setStreams(std::istream* i, std::ostream* o)
{
  input = i;
  output = o;
  getLexer()->switch_streams(i, o);
}


void
HexDLDriver::rewrite()
{
  //
  // parse and rewrite that thing
  //

  DLAtomInput dlinput;


  try
    {
      yy::HexDLParser parser(*this, dlinput);
      lexer->set_debug(Registry::getVerbose() > 2);
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
