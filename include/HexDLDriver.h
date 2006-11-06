/* -*- C++ -*- */

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
  class HexDLDriver : public PluginRewriter
  {
  private:
    /// lexer object which scans the stream
    HexDLFlexLexer* lexer;

    /// current Ontology
    Ontology::shared_pointer ontology;

  public:
    /// ctor
    HexDLDriver(std::istream& i, std::ostream& o);

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
    setStreams(std::istream* i, std::ostream* o);

    void
    setURI(const std::string& u);

    Ontology::shared_pointer
    getOntology() const;

    void
    rewrite();

    void
    error(const yy::location& l, const std::string& m) const throw (DLParsingError);
  };

} // namespace dl
} // namespace dlvhex

#endif // _HEXDLDRIVER_H

