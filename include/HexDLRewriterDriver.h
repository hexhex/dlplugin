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

#include <dlvhex/PluginInterface.h>

#include <iosfwd>
#include <string>
#include <map>

#include <boost/ptr_container/ptr_vector.hpp>

#include "HexDLRewriterParser.hpp"
#include "DLError.h"

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
  public:
    /// some information for the rewritten dl-atoms
    struct DLAtomOp
    {
      static const int minus = 0;
      static const int plus = 1;

      unsigned extAtomNo;	/// every external atom has its own number
      int op;		        /// 'p'(lus) or 'm'(inus) operator
      const std::string* lhs;	/// term of operator
      const std::string* rhs;	/// term of operator

      DLAtomOp(unsigned e, int o, const std::string* l, const std::string* r)
	: extAtomNo(e), op(o), lhs(l), rhs(r)
      {
	assert(op == minus || op == plus);
      }
    
      ~DLAtomOp()
      {
	delete lhs;
	delete rhs;
      }
    };


  private:
    /// lexer object which scans the stream
    HexDLRewriterFlexLexer* lexer;

    std::string uri; /// OWL URI

    unsigned extAtomNo; /// counter for external atoms
 
    /// keep track of rewritten dl-atoms
    boost::ptr_vector<DLAtomOp> rewrittenDLAtoms;

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

    const std::string&
    getUri() const;

    void
    addExtAtomNo(int offset);

    unsigned
    getExtAtomNo() const;

    void
    rewrite();

    void
    error(const yy::location& l, const std::string& m) const throw (DLParsingError);

    /// callback for dl-atoms
    void
    registerDLOp(DLAtomOp* op);
  };

} // namespace dl
} // namespace dlvhex

#endif // _HEXDLREWRITERDRIVER_H

