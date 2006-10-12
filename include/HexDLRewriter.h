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
    bool naf;

    virtual std::ostream&
    rewrite(std::ostream& os) const = 0;

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
    getLiteral() const;

    friend std::ostream&
    operator<< (std::ostream&, const HexDLRewriterBase&);
  };


  /** 
   * Output @a r to @a os.
   * 
   * @param os 
   * @param r 
   * 
   * @return @a os
   */
  inline std::ostream&
  operator<< (std::ostream& os, const HexDLRewriterBase& r)
  {
    os << (r.naf ? "not " : "");
    return r.rewrite(os);
  }


  /**
   * Just output the literal string.
   */
  class LiteralRewriter : public HexDLRewriterBase
  {
  private:
    AtomPtr literal;

    /// private assignment op
    LiteralRewriter&
    operator= (const LiteralRewriter&);

  protected:
    std::ostream&
    rewrite(std::ostream& os) const;

  public:
    /// ctor
    explicit
    LiteralRewriter(const std::string* l);

    explicit
    LiteralRewriter(Atom* a);

    /// copy ctor
    LiteralRewriter(const LiteralRewriter&);

    /// dtor
    virtual
    ~LiteralRewriter();

    Literal*
    getLiteral() const;
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

    std::auto_ptr<DLAtomRewriterBase>
    push(const std::auto_ptr<DLAtomRewriterBase>& b) const;

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
  protected:
    std::ostream&
    rewrite(std::ostream& os) const;

  public:
    SimpleDLAtomRewriter(const Tuple* i, const Tuple* o);
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

  protected:
    std::ostream&
    rewrite(std::ostream& os) const;

  public:
    /// ctor
    CQAtomRewriter(const Tuple* input, const Tuple* output);

    /// copy ctor
    CQAtomRewriter(const CQAtomRewriter&);
  };



  class DLAtomInput
  {
  private:
    typedef std::map<AtomSet,unsigned> AtomSetMap;
    AtomSetMap asmap;

    unsigned ncnt;

  public:
    DLAtomInput() : asmap(), ncnt(1) { }

    unsigned
    getInputNo(const AtomSet& as)
    {
      if (as.empty())
	{
	  return 0; // no dl-atom-ops
	}

      std::pair<AtomSetMap::iterator,bool> p = asmap.insert(std::make_pair(as, ncnt));
      
      if (p.second) // dl-atom-ops number found
	{
	  ncnt++;
	}

      return p.first->second;
    }
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

    std::ostream&
    rewrite(std::ostream& os) const;

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

    std::vector<Rule*>
    getDLInputRules() const;

    const Tuple*
    getInputTuple() const;
  };


  /**
   * Composite for HexDLRewriterBase objects.
   *
   * @todo add body optimization code, i.e. go through dlbody and push
   * each compatible dl- resp. cq-atom.
   */
  class BodyRewriter : public HexDLRewriterBase
  {
  private:
    // just plain literals
    boost::ptr_vector<HexDLRewriterBase> body;
    // the dl- and cq-atoms are stored here
    mutable boost::ptr_deque<DLAtomRewriterBase> dlbody;

    std::ostream&
    rewrite(std::ostream& os) const;

  public:
    /// default ctor
    BodyRewriter();

    void
    add(BodyRewriter* body0);

    void
    add(LiteralRewriter* atom);

    void
    add(DLAtomRewriterBase* atom);

    RuleBody_t*
    getBody() const;
  };


} // namespace dl
} // namespace dlvhex

#endif // _HEXDLREWRITER_H

