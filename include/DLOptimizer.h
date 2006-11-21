/* -*- C++ -*- */

/**
 * @file   DLOptimizer.h
 * @author Thomas Krennwallner
 * @date   Tue Jul 25 09:16:44 2006
 * 
 * @brief  Optimizes the DL part of HEX programs.
 * 
 */


#ifndef _DLOPTIMIZER_H
#define _DLOPTIMIZER_H


#include <dlvhex/PluginInterface.h>


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
    optimize(NodeGraph& dg, AtomSet& edb);
  };

} // namespace dl
} // namespace dlvhex

#endif // _DLOPTIMIZER_H

