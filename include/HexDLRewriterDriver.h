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
      /// minus operator
      static const int minus = 0;
      /// plus operator
      static const int plus = 1;

      /// every external atom has its own number
      unsigned extAtomNo;
      /// plus or minus operator
      int op;
      /// LHS of operator
      const std::string* lhs;
      /// RHS of operator
      const std::string* rhs;

      /// ctor
      DLAtomOp(unsigned e, int o, const std::string* l, const std::string* r)
	: extAtomNo(e), op(o), lhs(l), rhs(r)
      {
	assert(op == minus || op == plus);
      }

      /// copy ctor
      DLAtomOp(const DLAtomOp& o)
	: extAtomNo(o.extAtomNo),
	  op(o.op),
	  lhs(new std::string(*o.lhs)),
	  rhs(new std::string(*o.rhs))
      { }

      DLAtomOp&
      operator= (const DLAtomOp& o)
      {
	if (this != &o)
	  {
	    extAtomNo = o.extAtomNo;
	    op = o.op;
	    delete lhs;
	    delete rhs;
	    lhs = new std::string(*o.lhs);
	    rhs = new std::string(*o.rhs);
	  }
	return *this;
      }

      /// dtor
      ~DLAtomOp()
      {
	delete lhs;
	delete rhs;
      }
    };


  private:
    /// lexer object which scans the stream
    HexDLRewriterFlexLexer* lexer;

    /// Ontology URI
    std::string uri;

    /// counter for external atoms
    unsigned extAtomNo;
 
    /// keep track of rewritten dl-atoms
    boost::ptr_vector<DLAtomOp> rewrittenDLAtoms;

    /// reset counter and other stuff
    void
    reset();

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

