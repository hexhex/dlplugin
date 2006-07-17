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
#include <vector>

#include <boost/shared_ptr.hpp>

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

    /// forward declaration
    struct RewriteRule;

    /// keep track of rewritten rules
    std::vector<boost::shared_ptr<RewriteRule> > rules;

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
