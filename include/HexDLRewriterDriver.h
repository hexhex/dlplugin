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

//
// forward declarations
//
class HexDLRewriterFlexLexer;
namespace dlvhex {
namespace dl {
  class HexDLDriver;
}
}


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

    ///@todo this is a temporary solution
    HexDLDriver* rewriter;

    /// set to true if you want to turn on the rewriting facility
    bool doRewriting;

  public:
    /// ctor
    HexDLRewriterDriver(HexDLDriver* d, std::istream& i, std::ostream& o);

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

    void
    setStreams(std::istream* i, std::ostream* o);

    void
    setRewriting(bool = true);

    bool
    getRewriting();

    void
    rewrite();

    void
    error(const yy::location& l, const std::string& m) const throw (DLParsingError);
  };

} // namespace dl
} // namespace dlvhex

#endif // _HEXDLREWRITERDRIVER_H

