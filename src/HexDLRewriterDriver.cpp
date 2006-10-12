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

#include <dlvhex/Program.h>
#include <dlvhex/GraphBuilder.h>
#include <dlvhex/DependencyGraph.h>
#include <dlvhex/ComponentFinder.h>

#include <iosfwd>

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/erase.hpp>

using namespace dlvhex::dl;


HexDLRewriterDriver::HexDLRewriterDriver(std::istream& i, std::ostream& o)
  : PluginRewriter(i, o),
    lexer(new HexDLRewriterFlexLexer(this)),
    ontology()
{
  lexer->switch_streams(&i, &o);
}


HexDLRewriterDriver::HexDLRewriterDriver(const HexDLRewriterDriver& d)
  : PluginRewriter(*d.input, *d.output),
    lexer(new HexDLRewriterFlexLexer(this)),
    ontology(d.ontology)
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
      setStreams(d.input, d.output);
    }

  return *this;
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

  Program prog;
  DLAtomInput dlinput;

  try
    {
      yy::HexDLRewriterParser parser(*this, prog, dlinput);
      parser.set_debug_level(Registry::getVerbose() > 2 ? true : false);
      parser.parse();
    }
  catch (DLParsingError& e)
    {
      throw PluginError(e.what());
    }

  //
  // build dependency graph
  //

  GraphBuilder gb;
  SimpleComponentFinder cf;

  DependencyGraph dg(prog, &gb, &cf);

  //  Subgraph* sg = 0;

//   while (sg = dg.getNextSubgraph())
//     {
//       sg->dump(std::cerr);
//     }

  //
  // now add parsed rules to the output
  //

  for (Program::const_iterator it = prog.begin();
       it != prog.end(); ++it)
    {
      const Rule* r = *it;

      getOutput() << *r << std::endl;

      ///@todo this burns the readers eyes, but for now it prevents
      //memory leaks

      // delete unmanaged objects in the program

      const RuleBody_t& body = r->getBody();

      // delete each literal in the body
      for (RuleBody_t::const_iterator it = body.begin(); it != body.end(); ++it)
	{
	  delete *it;
	}

      delete r;
    }
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
