/* -*- C++ -*- */

/**
 * @file   HexDLRewriterDriver.h
 * @author Thomas Krennwallner
 * @date   Tue Jul 25 09:16:44 2006
 * 
 * @brief  Driver class for the Hex DL Rewriters bison/flex parser.
 * 
 */


#ifndef _HEXDLREWRITERDRIVER_H
#define _HEXDLREWRITERDRIVER_H


#include <dlvhex/PluginInterface.h>


namespace dlvhex {
namespace dl {

  /**
   * A PluginRewriter which parses the HEX input program and rewrites
   * dl-atoms to external atoms and tries to optimize queries by
   * rewriting cq-atoms.
   */
  class HexDLRewriterDriver : public PluginOptimizer
  {
  private:
    /// set to true if you want to turn on the rewriting facility
    bool doRewriting;

  public:
    /// ctor
    HexDLRewriterDriver();

    /// copy ctor
    HexDLRewriterDriver(const HexDLRewriterDriver&);

    /// assignment op
    HexDLRewriterDriver&
    operator= (const HexDLRewriterDriver&);

    /// dtor
    virtual
    ~HexDLRewriterDriver();

    void
    setRewriting(bool = true);

    bool
    getRewriting();

    void
    optimize(NodeGraph& dg, AtomSet& edb);
  };

} // namespace dl
} // namespace dlvhex

#endif // _HEXDLREWRITERDRIVER_H

