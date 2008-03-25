/* dlvhex-dlplugin -- Integration of Answer-Set Programming and Description Logics.
 *
 * Copyright (C) 2005, 2006, 2007  Thomas Krennwallner
 * 
 * This file is part of dlvhex-dlplugin.
 *
 * dlvhex-dlplugin is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * dlvhex-dlplugin is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dlvhex-dlplugin; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


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
#include "DFRewriter.h"

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
HexDLDriver::setOntology(const Ontology::shared_pointer& o)
{
  ontology = o;
}

Ontology::shared_pointer
HexDLDriver::getOntology() const
{
  return this->ontology;
}


void
HexDLDriver::convert(std::istream& input, std::ostream& output)
{
  this->output = &output;
  getLexer()->switch_streams(&input, &output);

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


// Local Variables:
// mode: C++
// End:
