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


namespace dlvhex {

  //
  // forward declaration
  //
  namespace util {
    class TCPIOStream;
  } //namespace util

namespace dl {

  //
  // forward declarations
  //
  class BaseCache;
  class CacheStats;
  class HexDLRewriterDriver;

namespace racer {

  //
  // forward declarations
  //
  class RacerKBManager;


  /**
   * @brief Concrete factory for the Plugin infrastructure.
   */
  class RacerInterface : public PluginInterface
  {
  private:

    //
    // We instantiate these objects here so we don't have to keep them
    // as static variable or singletons which is always problematic
    // due to the unforseeable nature of destruction order of global
    // static objects. Here, we can control the destruction time
    // explicitly in the dtor of RacerInterface.
    //

    /// the tcp streaming interface to the RACER server
    dlvhex::util::TCPIOStream* stream;
    /// the cache statistics
    CacheStats* stats;
    /// the cache for RACER queries
    BaseCache* cache;
    /// DL Rewriter facility
    HexDLRewriterDriver* rewriter;
    /// the kb-manager for RACER
    RacerKBManager* kbManager;

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

    static RacerInterface*
    instance();

    ///@return the current cache
    BaseCache*
    getCache() const
    {
      return cache;
    }

    /**
     * @return the rewriter for this plugin.
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

  };

} // namespace racer
} // namespace dl
} // namespace dlvhex


/**
 * instantiates the RacerInterface singleton. Used as import function
 * when loading the plugin as shared library.
 */
extern "C" PluginInterface*
PLUGINIMPORTFUNCTION();


#endif /* _RACERINTERFACE_H */
