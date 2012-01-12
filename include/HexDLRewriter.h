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
 * @file   HexDLRewriter.h
 * @author Thomas Krennwallner
 * @date   Thu Aug 31 13:54:22 2006
 * 
 * @brief  Keeps track of rewriting the body of rules.
 * 
 * 
 */

#if 0

#ifndef _HEXDLREWRITER_H
#define _HEXDLREWRITER_H

#include "Ontology.h"

#include <iosfwd>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_deque.hpp>

#include <dlvhex/ComfortPluginInterface.hpp>
#include <dlvhex/Rule.hpp>

namespace dlvhex {
namespace dl {

  /**
   * Base class for the HexDLRewriter classes.
   * Resembles the Interpreter pattern.
   */
  class HexDLRewriterBase
  {
  protected:
    /// is this rewriter negated?
    bool naf;

  public:
    /// default ctor
    HexDLRewriterBase();

    /// dtor
    virtual
    ~HexDLRewriterBase() = 0;

    inline void
    setNAF(bool n)
    {
      this->naf = n;
    }

    inline bool
    isNAF() const
    {
      return this->naf;
    }
    
    virtual Literal*
    getLiteral() const
    {
      return 0;
    }
  };
  typedef boost::shared_ptr<HexDLRewriterBase> HexDLRewriterBasePtr;


  /**
   * Rewriter for &dlC, &dlR, &dlDR, &dlCQ, and &dlUCQ ext-atoms.
   */
  class ExtAtomRewriter : public HexDLRewriterBase
  {
  protected:
    ComfortAtom extAtom;

    ExtAtomRewriter(const ExtAtomRewriter& b);

    ExternalAtom*
    getExtAtom() const;

    void
    getCQ(const std::string& query, const ComfortTuple& output, ComfortInterpretation& cq) const;

    virtual const ComfortTuple&
    getInputTuple() const;

    virtual inline ComfortTuple
    getOutputTuple() const
    {
      return extAtom->getArguments();
    }

  public:
    explicit
    ExtAtomRewriter(const ComfortAtom& ea);

    virtual
    ~ExtAtomRewriter();

    /// perform Query Pushing
    std::auto_ptr<ExtAtomRewriter>
    push(const std::auto_ptr<ExtAtomRewriter>& b) const;

    virtual Literal*
    getLiteral() const;
  };


  /**
   * Composite for ExtAtomRewriter objects.
   */
  class BodyRewriter : public HexDLRewriterBase
  {
  protected:
    // the dl- and cq-atoms are stored here
    mutable boost::ptr_deque<ExtAtomRewriter> dlbody;

  public:
    /// default ctor
    BodyRewriter();

    virtual
    ~BodyRewriter();

    virtual void
    add(ExtAtomRewriter* atom);

    void
    bodyOptimizer(RuleBody_t&) const;
  };


  /**
   * Keeps track of dl-input operations.
   */
  class DLAtomInput
  {
  private:
    typedef std::map<ComfortInterpretation,unsigned> AtomSetMap;
    AtomSetMap asmap;

    unsigned ncnt;

  public:
    DLAtomInput();

    unsigned
    getInputNo(const AtomSet& as);

    std::vector<Rule*>
    getDLInputRules() const;
  };



  /**
   * Rewrites dl-atoms to &dlC/&dlR/&dlDR external atoms.
   */
  class DLAtomRewriter : public HexDLRewriterBase
  {
  private:
    const Ontology::shared_pointer ontology;
    
    const std::string* const query;
    const std::vector<ID>* const cq;
    const std::vector<std::vector<ID> >* const ucq;

    DLAtomInput& dlinput;
    const ComfortTuple* output;
    mutable ComfortTuple* input;

    ComfortInterpretation pc;
    ComfortInterpretation mc;
    ComfortInterpretation pr;
    ComfortInterpretation mr;

    /// private copy ctor
    DLAtomRewriter(const DLAtomRewriter&);

    /// private assignment op
    DLAtomRewriter&
    operator= (const DLAtomRewriter&);

  public:
    /// dl-atom ctor
    DLAtomRewriter(const Ontology::shared_pointer& onto,
		   DLAtomInput& dlinput,
		   const ComfortInterpretation& o,
		   const std::string* q,
		   const ComfortTuple* out);

    /// cq-atom ctor
    DLAtomRewriter(const Ontology::shared_pointer& onto,
		   DLAtomInput& dlinput,
		   const ComfortInterpretation& o,
		   const std::vector<ID>* cq,
		   const ComfortTuple* out);

    /// ucq-atom ctor
    DLAtomRewriter(const Ontology::shared_pointer& onto,
		   DLAtomInput& dlinput,
		   const ComfortInterpretation& o,
		   const std::vector<std::vector<ID> >* ucq,
		   const ComfortTuple* out);

    ~DLAtomRewriter();

    virtual Literal*
    getLiteral() const;

    const Tuple&
    getInputTuple() const;

    std::string
    getName() const;
  };


} // namespace dl
} // namespace dlvhex

#endif // _HEXDLREWRITER_H


// Local Variables:
// mode: C++
// End:

#endif
