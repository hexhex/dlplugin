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

    void
    setNAF(bool n);
    
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
   * Rewrites &dlCQ ext-atoms to &dlCQn ext-atoms.
   */
  class CQAtomRewriter : public HexDLRewriterBase
  {
  private:
    Tuple* const input;
    Tuple* const output;

    /// private assignment op
    CQAtomRewriter&
    operator= (const CQAtomRewriter&);

  protected:
    std::ostream&
    rewrite(std::ostream& os) const;

  public:
    /// ctor
    CQAtomRewriter(Tuple* input, Tuple* output);

    /// copy ctor
    CQAtomRewriter(const CQAtomRewriter&);

    virtual
    ~CQAtomRewriter();
  };



  /**
   * Rewrites dl-atoms to &dlC/&dlR/&dlDR external atoms.
   */
  class DLAtomRewriter : public HexDLRewriterBase
  {
  private:
    const Ontology::shared_pointer ontology;

    const std::string* const query;
    const Tuple* const out;

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

    unsigned
    getInputNo(const AtomSet& as) const;


  public:
    /// ctor
    DLAtomRewriter(const Ontology::shared_pointer& onto,
		   const AtomSet& o,
		   const std::string* q,
		   const Tuple* out);

    ~DLAtomRewriter();

    std::vector<Rule*>
    getDLInputRules() const;
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
    boost::ptr_vector<HexDLRewriterBase> dlbody;

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
    add(CQAtomRewriter* atom);

    void
    add(DLAtomRewriter* atom);

    RuleBody_t*
    getBody() const;
  };


} // namespace dl
} // namespace dlvhex

#endif // _HEXDLREWRITER_H

