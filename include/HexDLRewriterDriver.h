/* -*- C++ -*- */

/**
 * @file   HexDLRewriterDriver.h
 * @author Thomas Krennwallner
 * @date   Tue Jul 25 09:16:44 2006
 * 
 * @brief  Driver class for the Hex DL Rewriters bison/flex parser.
 * 
 * 
 */


#ifndef _HEXDLREWRITERDRIVER_H
#define _HEXDLREWRITERDRIVER_H

#include <dlvhex/PluginInterface.h>

#include <iosfwd>
#include <string>
#include <map>

#include <boost/ptr_container/ptr_vector.hpp>

#include "HexDLRewriterParser.hpp"
#include "RacerError.h"

//
// forward declarations
//
class HexDLRewriterFlexLexer;


namespace dlvhex {
namespace racer {

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

    std::string uri; /// OWL URI

    unsigned extAtomNo; /// counter for external atoms

    /// some information for the rewritten dl-atoms
    struct RewriteDLAtom
    {
      unsigned extAtomNo;	/// every external atom has its own number
      char opChar;		/// 'p'(lus) or 'm'(inus) operator
      std::string lhs;		/// term of operator
      std::string rhs;		/// term of operator

      RewriteDLAtom(unsigned e, char o,
		    const std::string& l,
		    const std::string& r)
	: extAtomNo(e), opChar(o), lhs(l), rhs(r)
      { }
    };
 
    /// keep track of rewritten dl-atoms
    boost::ptr_vector<RewriteDLAtom> rewrittenDLAtoms;

    /// reset counter and other stuff
    void
    reset();

  public:
    HexDLRewriterDriver(std::istream& i, std::ostream& o);

    virtual
    ~HexDLRewriterDriver();

    HexDLRewriterFlexLexer*
    getLexer() const;

    std::ostream&
    getOutput() const;

    void
    setStreams(std::istream* i, std::ostream* o);

    void
    setUri(const std::string& u);

    void
    rewrite();

    void
    error(const yy::location& l, const std::string& m) const throw (RacerParsingError);

    /// callback for dl-atoms
    std::string
    rewriteDLAtom(const std::string& query, const std::string& t1);

    /// callback for dl-atoms
    std::string
    rewriteDLAtom(const std::string& query, const std::string& t1, const std::string& t2);

    /// callback for dl-atoms
    void
    registerDLOp(char op, const std::string& lhs, const std::string& rhs);
  };

} // namespace racer
} // namespace dlvhex

#endif // _HEXDLREWRITERDRIVER_H

