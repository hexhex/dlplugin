/* -*- C++ -*- */

/**
 * @file   RacerInterface.h
 * @author Thomas Krennwallner
 * @date   Sun Jul  3 16:57:51 2005
 * 
 * @brief  Interface to the RACER plugin.
 * 
 * @defgroup PluginOverview Overview of the Racer plugin 
 *
 * This is a brief overview of the Racer plugin with its components
 * and their relationships and information flow.
 *
 * @verbatim
     ________              ________________
    |        |            |                | instantiates and maintains the
    | DLVHEX | <========> | RacerInterface | TCP IOStream, the Racer Cache and
    |________|            |________________| creates the external atoms.
       |  ^
       |  |
 Query |  | Answer
       |  |
    ___v__|__________
   |                 | setup of Query objects and RacerDirectors, on-the-fly
   | RacerExtAtom    | RACER process creation
   |_________________|
   |                 | represents a Racer command and provides caching and
   | RacerDirector   | command chaining support as well as inconsistency handling
   |            ^^   |
   |__||________||___|
   |  ||     |  ||   | RacerBuilder uses the Query to create RACER commands
   |  vv     |       | 
   | Racer   | Racer | RacerParse fills Answer with the corresponding reply
   | Builder | Parse | of RACER
   |_________|_______|
   |                 | handles network connection
   | TCP IOStream    |
   |_________________|
           ^^
 __________||___________ Process
 __________||___________ boundary
           ||
    _______vv________
   |                 |
   |    RACER DL     |
   |   Inference     |
   |     Engine      |
   |_________________|
   @endverbatim
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
    /// the tcp streaming interface to the RACER server
    TCPIOStream stream;
    /// the cache for RACER queries
    RacerCachingDirector::RacerCache cache;

  public:
    RacerInterface();

    virtual
    ~RacerInterface();

    /// not implemented yet
    virtual PluginRewriter* 
    createRewriter(std::istream&, std::ostream&);

    /**
     * loads all individuals from a given owl kb.
     *
     * @param uri
     * @param uni
     */
    virtual void
    getUniverse(std::string& uri, std::list<Term>& uni);

    /**
     * Add new RACER external atoms with the according names to m.
     *
     * @param m
     */
    virtual void
    getAtoms(AtomFunctionMap& m);
  };

} // namespace racer
} // namespace dlvhex


/**
 * instantiates the PluginInterface singleton. Used as import function
 * when loading the plugin as shared library.
 */
extern "C" PluginInterface*
PLUGINIMPORTFUNCTION();


#endif /* _RACERINTERFACE_H */
