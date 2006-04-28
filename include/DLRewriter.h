/* -*- C++ -*- */

/**
 * @file   DLRewriter.h
 * @author Thomas Krennwallner
 * @date   Tue Apr 25 13:43:35 2006
 * 
 * @brief  
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

#include <boost/shared_ptr.hpp>

namespace dlvhex {
namespace racer {


  /**
   * @brief 
   */
  class DLRewriter : public PluginRewriter
  {
  private:
    std::string uri;
    std::istream* in;
    std::ostream* out;

    unsigned extAtomNo;

    struct RewriteRule;

    std::vector<boost::shared_ptr<RewriteRule> > rules;

    void
    rewriteLine(const std::string&);

    std::string
    filterInput(const std::string&);

  public:

    DLRewriter(std::istream& i, std::ostream& o);

    void
    setStreams(std::istream* i, std::ostream* o);

    std::istream&
    getInput() const;

    std::ostream&
    getOutput() const;

    void
    setUri(const std::string& u);

    void
    rewrite();
  };

} // namespace racer
} // namespace dlvhex

#endif /* _DLREWRITER_H */
