/* -*- C++ -*- */

/**
 * @file   HexDLRewriter.h
 * @author Thomas Krennwallner
 * @date   Thu Aug 31 13:54:22 2006
 * 
 * @brief  Keeps track of rewriting the body of rules.
 * 
 * 
 */

#ifndef _HEXDLREWRITER_H
#define _HEXDLREWRITER_H

#include "Ontology.h"

#include <iosfwd>
#include <string>

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_deque.hpp>

#include <dlvhex/Atom.h>
#include <dlvhex/Literal.h>
#include <dlvhex/Term.h>
#include <dlvhex/Rule.h>

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
    ~HexDLRewriterBase();

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
    { return 0; }
  };


  /**
   * Base class for dl- and cq-atom rewriters.
   */
  class DLAtomRewriterBase : public HexDLRewriterBase
  {
  private:
    const Tuple* input;
    const Tuple* output;

  protected:
    explicit
    DLAtomRewriterBase(const Tuple* input = 0, const Tuple* output = 0);

    DLAtomRewriterBase(const DLAtomRewriterBase& b);

    void
    getCQ(const std::string& query,
	  const Tuple& output,
	  AtomSet& cq) const;

  public:
    virtual
    ~DLAtomRewriterBase();

    virtual Literal*
    getLiteral() const;

    std::auto_ptr<DLAtomRewriterBase>
    push(const std::auto_ptr<DLAtomRewriterBase>& b) const;

    virtual std::string
    getName() const = 0;

    virtual const Tuple*
    getInputTuple() const;

    virtual const Tuple*
    getOutputTuple() const;

    inline void
    setInputTuple(const Tuple* in)
    {
      delete input;
      input = in;
    }

    inline void
    setOutputTuple(const Tuple* out)
    {
      delete output;
      output = out;
    }
  };


  /**
   * Rewriter for &dlC and &dlR ext-atoms.
   */
  class SimpleDLAtomRewriter : public DLAtomRewriterBase
  {
  private:
    const std::string* name;

  public:
    SimpleDLAtomRewriter(const std::string* n, const Tuple* i, const Tuple* o);

    ~SimpleDLAtomRewriter();

    std::string
    getName() const;
  };


  /**
   * Rewrites &dlCQ ext-atoms to &dlCQn ext-atoms.
   */
  class CQAtomRewriter : public DLAtomRewriterBase
  {
  private:
    /// private assignment op
    CQAtomRewriter&
    operator= (const CQAtomRewriter&);

  public:
    /// ctor
    CQAtomRewriter(const Tuple* i, const Tuple* o);

    /// copy ctor
    CQAtomRewriter(const CQAtomRewriter& c);

    std::string
    getName() const;
  };



  /**
   * Keeps track of dl-input operations.
   */
  class DLAtomInput
  {
  private:
    typedef std::map<AtomSet,unsigned> AtomSetMap;
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
  class DLAtomRewriter : public DLAtomRewriterBase
  {
  private:
    const Ontology::shared_pointer ontology;
    
    const std::string* const query;

    DLAtomInput& dlinput;

    AtomSet pc;
    AtomSet mc;
    AtomSet pr;
    AtomSet mr;

    /// private copy ctor
    DLAtomRewriter(const DLAtomRewriter&);

    /// private assignment op
    DLAtomRewriter&
    operator= (const DLAtomRewriter&);

    std::string
    addNamespace(const std::string& s) const;

  public:
    /// ctor
    DLAtomRewriter(const Ontology::shared_pointer& onto,
		   DLAtomInput& dlinput,
		   const AtomSet& o,
		   const std::string* q,
		   const Tuple* out);

    ~DLAtomRewriter();

    const Tuple*
    getInputTuple() const;

    std::string
    getName() const;
  };


  /**
   * Composite for HexDLRewriterBase objects.
   */
  class BodyRewriter : public HexDLRewriterBase
  {
  protected:
    // the dl- and cq-atoms are stored here
    mutable boost::ptr_deque<DLAtomRewriterBase> dlbody;

  public:
    /// default ctor
    BodyRewriter();

    virtual
    ~BodyRewriter();

    virtual void
    add(DLAtomRewriterBase* atom);

    RuleBody_t*
    getBody() const;
  };


} // namespace dl
} // namespace dlvhex

#endif // _HEXDLREWRITER_H

