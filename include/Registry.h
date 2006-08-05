/* -*- C++ -*- */

/**
 * @file   Registry.h
 * @author Thomas Krennwallner
 * @date   Tue May 30 11:53:17 2006
 * 
 * @brief  Global state.
 * 
 * 
 */


#ifndef _REGISTRY_H
#define _REGISTRY_H

#include <string>

namespace dlvhex {
namespace racer {

  class Registry
  {
  private:
    static unsigned verbose;
    static bool una;
    static bool datasubstrate;
    static std::string kbName;
    static std::string openURI;

    Registry();

  public:
    static unsigned
    getVerbose();

    static void
    setVerbose(unsigned);

    static std::string
    getKBName();

    static void
    setKBName(const std::string&);

    static std::string
    getOpenURI();

    static void
    setOpenURI(const std::string&);

    static bool
    getUNA();

    static void
    setUNA(bool);

    static bool
    getDataSubstrateMirroring();

    static void
    setDataSubstrateMirroring(bool);
  };

} // namespace racer
} // namespace dlvhex

#endif /* _REGISTRY_H */
