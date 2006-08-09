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

#include <ace/Null_Mutex.h>
#include <ace/Singleton.h>


namespace dlvhex {
namespace racer {

  //
  // forward declarations
  //
  class TCPIOStream;
  class BaseCache;
  class HexDLRewriterDriver;



  /**
   * @brief Concrete factory for the Plugin infrastructure.
   */
  class RacerInterface : public PluginInterface
  {
  private:
    /// the tcp streaming interface to the RACER server
    TCPIOStream* stream;
    /// the cache for RACER queries
    BaseCache* cache;
    /// DL Rewriter facility
    HexDLRewriterDriver* rewriter;

    //
    // keep those ctors private, we don't want multiple instantiations
    //

    /// private copy ctor
    RacerInterface(const RacerInterface&)
      : PluginInterface()
    { }

    /// private default ctor
    RacerInterface();

  public:
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
     * @param doHelp
     * @param argv 
     * @param out
     */
    virtual void
    setOptions(bool doHelp, std::vector<std::string>& argv, std::ostream& out);

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
 * instantiates the RacerInterface singleton. Used as import function
 * when loading the plugin as shared library.
 */
extern "C" PluginInterface*
PLUGINIMPORTFUNCTION();


#endif /* _RACERINTERFACE_H */
