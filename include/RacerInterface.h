/* -*- C++ -*- */

/**
 * @file   RacerInterface.h
 * @author Thomas Krennwallner
 * @date   Sun Jul  3 16:57:51 2005
 * 
 * @brief  Interface to the RACER plugin.
 * 
 * 
 */

#ifndef _RACERINTERFACE_H
#define _RACERINTERFACE_H

#include <dlvhex/PluginInterface.h>
#include <dlvhex/Atom.h>
#include <dlvhex/Term.h>

#include "RacerRunner.h"
#include "RacerDirector.h"
#include "TCPStream.h"

namespace dlvhex {
namespace racer {

  /**
   * @brief Concrete factory for the Plugin infrastructure.
   */
  class RacerInterface : public PluginInterface
  {
  private:
    TCPIOStream stream;
    RacerRunner racer;
    RacerCachingDirector::RacerCache cache;

  public:
    RacerInterface();

    virtual
    ~RacerInterface();

    virtual PluginRewriter* 
    createRewriter(std::istream&, std::ostream&);

    virtual void
    getUniverse(std::list<Term>&);

    virtual void
    getAtoms(AtomFunctionMap&);
  };

} // namespace racer
} // namespace dlvhex


/**
 * @brief
 */
extern "C" PluginInterface*
PLUGINIMPORTFUNCTION();


#endif /* _RACERINTERFACE_H */
