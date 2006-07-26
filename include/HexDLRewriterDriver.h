/* -*- C++ -*- */

/**
 * @file   HexDLRewriterDriver.h
 * @author Thomas Krennwallner
 * @date   Tue Jul 25 09:16:44 2006
 * 
 * @brief  
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

// some forward declaration 
class HexDLRewriterFlexLexer;


namespace dlvhex {
namespace racer {


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

    /// keep track of dl atoms, just needed in case of an error
    std::map<unsigned, std::string> dlAtoms;

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

    /// callback for dl-atoms
    std::string
    rewriteDLAtom(const std::string&);

    /// callback for cq-atoms
    std::string
    rewriteCQAtom(const std::string&);
  };

} // namespace racer
} // namespace dlvhex

#endif // _HEXDLREWRITERDRIVER_H

