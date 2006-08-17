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
namespace dl {

  /**
   * @brief The Registry keeps track of global state.
   */
  class Registry
  {
  private:
    static unsigned verbose;
    static bool una;
    static bool datasubstrate;

    /// pure virtual dtor, we don't want an instance or a child
    virtual
    ~Registry() = 0;

  public:
    static unsigned
    getVerbose();

    static void
    setVerbose(unsigned);

    static bool
    getUNA();

    static void
    setUNA(bool);

    static bool
    getDataSubstrateMirroring();

    static void
    setDataSubstrateMirroring(bool);
  };

} // namespace dl
} // namespace dlvhex

#endif /* _REGISTRY_H */
