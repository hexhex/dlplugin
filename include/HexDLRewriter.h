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

#include <iosfwd>
#include <string>

#include <boost/ptr_container/ptr_vector.hpp>

#include <dlvhex/Term.h>

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
    const std::string* const literal;

  protected:
    std::ostream&
    rewrite(std::ostream& os) const;

  public:
    explicit
    LiteralRewriter(const std::string* l);

    virtual
    ~LiteralRewriter();
  };


  /**
   * Composite for HexDLRewriterBase objects.
   *
   * @todo add body optimization code.
   */
  class BodyRewriter : public HexDLRewriterBase
  {
  protected:
    boost::ptr_vector<HexDLRewriterBase> body;

    std::ostream&
    rewrite(std::ostream& os) const;

  public:
    void
    add(HexDLRewriterBase* atom);
  };


  /**
   * Rewrites &dlCQ ext-atoms to &dlCQn ext-atoms.
   */
  class CQAtomRewriter : public HexDLRewriterBase
  {
  private:
    Tuple* const input;
    Tuple* const output;

  protected:
    std::ostream&
    rewrite(std::ostream& os) const;

  public:
    CQAtomRewriter(Tuple* input, Tuple* output);

    virtual
    ~CQAtomRewriter();
  };


  /**
   * Rewrites dl-atoms to &dlC/&dlR external atoms.
   */
  class DLAtomRewriter : public HexDLRewriterBase
  {
  protected:
    const std::string uri;
    const int extAtomNo;

    const std::string* query;
    const std::string* out1;
    const std::string* out2;

    std::ostream&
    rewrite(std::ostream& os) const;

  public:
    DLAtomRewriter(const std::string& uri,
		   int extAtomNo,
		   const std::string* q,
		   const std::string* t1,
		   const std::string* t2 = 0);

    ~DLAtomRewriter();
  };

} // namespace dl
} // namespace dlvhex

#endif // _HEXDLREWRITER_H

