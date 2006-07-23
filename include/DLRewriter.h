/* -*- C++ -*- */

/**
 * @file   DLRewriter.h
 * @author Thomas Krennwallner
 * @date   Tue Apr 25 13:43:35 2006
 * 
 * @brief  The plugins rewriting facility.
 * 
 * 
 */


#ifndef _DLREWRITER_H
#define _DLREWRITER_H

#include <dlvhex/PluginInterface.h>
#include <dlvhex/Atom.h>
#include <dlvhex/Term.h>

#include <iosfwd>
#include <string>
#include <map>

#include <boost/ptr_container/ptr_vector.hpp>

namespace dlvhex {
namespace racer {


  /**
   * @brief rewrites DL programs to HEX programs.
   */
  class DLRewriter : public PluginRewriter
  {
  private:
    std::string uri; /// OWL URI

    unsigned extAtomNo; /// counter for external atoms

    /// some information for the rewritten rules
    struct RewriteRule
    {
      unsigned extAtomNo;	/**< every external atom has its own number */
      char opChar;		/**< either 'p'(lus) or 'm'(inus) operator */
      std::string lhs;		/**< left term of operator */
      std::string rhs;		/**< right term of operator */

      RewriteRule(unsigned e, char o,
		  const std::string& l,
		  const std::string& r)
	: extAtomNo(e), opChar(o), lhs(l), rhs(r)
      { }
    };
 
    /// keep track of rewritten rules
    boost::ptr_vector<RewriteRule> rules;

    /// keep track of dl atoms, just needed in case of an error
    std::map<unsigned, std::string> dlAtoms;

    void
    rewriteLine(const std::string&);

    std::string
    filterInput(const std::string&);

  public:
    DLRewriter(std::istream& i, std::ostream& o);

    void
    setStreams(std::istream* i, std::ostream* o);

    void
    setUri(const std::string& u);

    void
    rewrite();
  };

} // namespace racer
} // namespace dlvhex

#endif /* _DLREWRITER_H */
