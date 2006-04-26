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

    std::set<RewriteRule*> rules;

    static const char* re;


  public:
    void
    rewriteLine(const std::string&);

    void
    rewriteConcept(const std::string&);

    void
    rewriteRole(const std::string&);


    DLRewriter(std::istream& i, std::ostream& o);

    void
    setStreams(std::istream* i, std::ostream* o);

//     std::istream&
//     getInput();

//     std::ostream&
//     getOutput();

    void
    setUri(const std::string& u);

    void
    rewrite();
  };

} // namespace racer
} // namespace dlvhex

#endif /* _DLREWRITER_H */
