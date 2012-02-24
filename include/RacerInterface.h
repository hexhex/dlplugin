/* dlvhex-dlplugin -- Integration of Answer-Set Programming and Description Logics.
 *
 * Copyright (C) 2005, 2006, 2007  Thomas Krennwallner
 * 
 * This file is part of dlvhex-dlplugin.
 *
 * dlvhex-dlplugin is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * dlvhex-dlplugin is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dlvhex-dlplugin; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


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

#include <dlvhex2/ComfortPluginInterface.h>
#include "Ontology.h"
#include "DFConverter.h"
#include "DFOutputBuilder.h"
#include "HexDLConverter.h"

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
  class DLOptimizer;
  class KBManager;

namespace racer {


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
    /// DL converter facility
    HexDLConverter* dlconverter;
    /// DF converter facility
    dlvhex::df::DFConverter* dfconverter;
    dlvhex::df::DFOutputBuilder* dfoutputbuilder;
    /// DL optimizer facility
    DLOptimizer* dloptimizer;
    /// the kb-manager for RACER
    KBManager* kbManager;

    /// current ontology; moved to this level to be shared between the
    /// df-rewriter and the hex-rewriter
    dlvhex::dl::Ontology::shared_pointer ontology;

    //
    // keep those ctors private, we don't want multiple instantiations
    //

    /// private copy ctor
    RacerInterface(const RacerInterface&);

    /// private assignment op
    RacerInterface&
    operator= (const RacerInterface&);

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

    ///@return the current KBManager
    KBManager*
    getKBManager() const
    {
      return kbManager;
    }

    /**
     * @return the DL converter.
     */
    virtual std::vector<PluginConverterPtr>
    createConverters(ProgramCtx& ctx);

    /**
     * @return the DL optimizer.
     */
/* @TODO
    virtual PluginOptimizer* 
    createOptimizer();
*/

    /**
     * Add new RACER external atoms with the according names to m.
     *
     * @param m
     */
    virtual std::vector<PluginAtomPtr> createAtoms(ProgramCtx& ctx) const;

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
     *  Refine output if default rules are in the input
     *
     */
/* @TODO
    virtual OutputBuilder*
    createOutputBuilder();
*/

  };

} // namespace racer
} // namespace dl
} // namespace dlvhex


/**
 * instantiates the RacerInterface singleton. Used as import function
 * when loading the plugin as shared library.
 */
//extern "C" DLVHEX_NAMESPACE PluginInterface*
//PLUGINIMPORTFUNCTION();
extern "C"
void * PLUGINIMPORTFUNCTION();

#endif /* _RACERINTERFACE_H */


// Local Variables:
// mode: C++
// End:
