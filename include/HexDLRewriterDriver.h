/* -*- C++ -*- */

/**
 * @file   HexDLRewriterDriver.h
 * @author Thomas Krennwallner
 * @date   Tue Jul 25 09:16:44 2006
 * 
 * @brief  Driver class for the Hex DL Rewriters bison/flex parser.
 * 
 */


#ifndef _HEXDLREWRITERDRIVER_H
#define _HEXDLREWRITERDRIVER_H


#include "HexDLRewriterParser.hpp"
#include "Ontology.h"
#include "DLError.h"

#include <dlvhex/PluginInterface.h>

#include <iosfwd>
#include <string>
#include <map>

#include <boost/ptr_container/ptr_vector.hpp>

//
// forward declarations
//
class HexDLRewriterFlexLexer;


namespace dlvhex {
namespace dl {

  /**
   * A PluginRewriter which parses the HEX input program and rewrites
   * dl-atoms to external atoms and tries to optimize queries by
   * rewriting cq-atoms.
   */
  class HexDLRewriterDriver : public PluginRewriter
  {
  private:
    /// lexer object which scans the stream
    HexDLRewriterFlexLexer* lexer;

    /// current Ontology
    Ontology::shared_pointer ontology;

    /// set to true if you want to turn on the rewriting facility
    bool doRewriting;

  public:
    /// ctor
    HexDLRewriterDriver(std::istream& i, std::ostream& o);

    /// copy ctor
    HexDLRewriterDriver(const HexDLRewriterDriver&);

    /// assignment op
    HexDLRewriterDriver&
    operator= (const HexDLRewriterDriver&);

    /// dtor
    virtual
    ~HexDLRewriterDriver();

    HexDLRewriterFlexLexer*
    getLexer() const;

    std::ostream&
    getOutput() const;

    void
    setStreams(std::istream* i, std::ostream* o);

    void
    setURI(const std::string& u);

    void
    setRewriting(bool = true);

    bool
    getRewriting();

    Ontology::shared_pointer
    getOntology() const;

    void
    rewrite();

    void
    error(const yy::location& l, const std::string& m) const throw (DLParsingError);
  };

} // namespace dl
} // namespace dlvhex

#endif // _HEXDLREWRITERDRIVER_H

