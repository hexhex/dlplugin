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
 * @file   HexDLDriver.h
 * @author Thomas Krennwallner
 * @date   Mon Nov  6 11:32:58 2006
 * 
 * @brief  Driver class for the Hex DL Rewriters bison/flex parser.
 * 
 */


#ifndef _HEXDLDRIVER_H
#define _HEXDLDRIVER_H


#include "HexDLParser.hpp"
#include "Ontology.h"
#include "DLError.h"

#include <dlvhex/PluginInterface.h>

#include <iosfwd>
#include <string>
#include <map>

//
// forward declarations
//
class HexDLFlexLexer;


namespace dlvhex {
namespace dl {

  /**
   * A PluginRewriter which parses the HEX input program and rewrites
   * dl-atoms to external atoms.
   */
  class HexDLDriver : public PluginConverter
  {
  private:
    /// lexer object which scans the stream
    HexDLFlexLexer* lexer;

    /// current Ontology
    Ontology::shared_pointer ontology;

    /// current output stream
    std::ostream* output;

  public:
    /// ctor
    HexDLDriver();

    /// copy ctor
    HexDLDriver(const HexDLDriver&);

    /// assignment op
    HexDLDriver&
    operator= (const HexDLDriver&);

    /// dtor
    virtual
    ~HexDLDriver();

    HexDLFlexLexer*
    getLexer() const;

    std::ostream&
    getOutput() const;

    void
    setURI(const std::string& u);

    Ontology::shared_pointer
    getOntology() const;

    void
    convert(std::istream& i, std::ostream& o);

    void
    error(const yy::location& l, const std::string& m) const throw (DLParsingError);
  };

} // namespace dl
} // namespace dlvhex

#endif // _HEXDLDRIVER_H


// Local Variables:
// mode: C++
// End:
