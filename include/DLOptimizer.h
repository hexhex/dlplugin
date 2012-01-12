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
 * @file   DLOptimizer.h
 * @author Thomas Krennwallner
 * @date   Tue Jul 25 09:16:44 2006
 * 
 * @brief  Optimizes the DL part of HEX programs.
 * 
 */

#if 0

#ifndef _DLOPTIMIZER_H
#define _DLOPTIMIZER_H


#include <dlvhex/ComfortPluginInterface.hpp>


namespace dlvhex {
namespace dl {

  /**
   * A PluginOptimizer which tries to optimize dl-queries by rewriting
   * cq-atoms.
   */
  class DLOptimizer : public PluginOptimizer
  {
  private:
    /// set to true if you want to turn on the rewriting facility
    bool doRewriting;

  public:
    /// ctor
    DLOptimizer();

    /// copy ctor
    DLOptimizer(const DLOptimizer&);

    /// assignment op
    DLOptimizer&
    operator= (const DLOptimizer&);

    /// dtor
    virtual
    ~DLOptimizer();

    void
    setRewriting(bool = true);

    bool
    getRewriting();

    void
    optimize(NodeGraph& dg, ComfortInterpretation& edb);
  };

} // namespace dl
} // namespace dlvhex

#endif // _DLOPTIMIZER_H


// Local Variables:
// mode: C++
// End:

#endif
