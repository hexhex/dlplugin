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
     ________             ________________
    |        |           |                | instantiates and maintains the
    | DLVHEX |<--------->| RacerInterface | TCP IOStream, the DL Cache and
    |________|           |________________| creates the external atoms.
       | ^ ^              ________________
       | | |             |                | Rewrites DL programs to HEX
       | | +------------>| DL Rewriter    | programs.
       | |               |________________|
       | |
       | |
 Query | | Answer
       | |
    ___v_|____________
   |                  | setup of Query objects and RacerDirectors, on-the-fly
   | RacerExtAtom     | RACER process creation
   |__________________|
   |                  | represents a Racer command and provides caching and
   | RacerDirector    | command chaining support as well as inconsistency handling
   |            ^^    |
   |__||________||____|
   |  ||     |  ||    | RacerBuilder uses the Query to create RACER commands
   |  vv     |        | 
   | Racer   | Racer  | RacerParser fills Answer with the corresponding reply
   | Builder | Parser | of RACER
   |_________|________|
   |                  | handles network connection
   | TCP IOStream     |
   |__________________|
           ^^
 __________||___________ Process
 __________||___________ boundary
           ||
    _______vv_________
   |                  |
   |    RACER DL      |
   |   Inference      |
   |     Engine       |
   |__________________|
   @endverbatim
 *
 *
 */

#ifndef _RACERINTERFACE_H
#define _RACERINTERFACE_H

#include "RacerDirector.h"
#include "TCPStream.h"
#include "HexDLRewriterDriver.h"
#include "Registry.h"
#include "Cache.h"

#include <dlvhex/PluginInterface.h>
#include <dlvhex/Atom.h>
#include <dlvhex/Term.h>

#include <ace/Null_Mutex.h>
#include <ace/Singleton.h>

#include <map>

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
    BaseCache* cache;
    /// DL Rewriter facility
    HexDLRewriterDriver* rewriter;
    /// saves properties
    Registry registry;

  public:
    RacerInterface();

    virtual
    ~RacerInterface();

    /**
     * returns the rewriter.
     */
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

    /**
     * Set the command line arguments here.
     *
     * @param argv 
     */
    virtual void
    setOptions(std::vector<std::string>& argv);

    /**
     * Start RACER and setup the stream.
     */
    virtual void
    startRacer();

    /// transparently allow access from ACE_Singleton<>
    friend class ACE_Singleton<RacerInterface, ACE_Null_Mutex>;
  };

  /// adapt RacerRunnerAdaptee to a singleton and register it to the
  /// ACE_Object_Manager facility for automatic object deletion at
  /// program exit time
  typedef ACE_Singleton<RacerInterface, ACE_Null_Mutex> TheRacerInterface;

} // namespace racer
} // namespace dlvhex


/**
 * instantiates the PluginInterface singleton. Used as import function
 * when loading the plugin as shared library.
 */
extern "C" PluginInterface*
PLUGINIMPORTFUNCTION();


#endif /* _RACERINTERFACE_H */
