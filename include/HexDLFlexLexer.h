/* -*- C++ -*- */

/**
 * @file   HexDLFlexLexer.h
 * @author Thomas Krennwallner
 * @date   Sat Nov  4 15:21:58 2006
 * 
 * @brief  
 * 
 * 
 */

#ifndef _HEXDLFLEXLEXER
#define _HEXDLFLEXLEXER

// put FlexLexer.h into its own include guards or yyFlexLexer gets
// redefined
#ifndef __FLEX_LEXER_H
#undef yyFlexLexer
#define yyFlexLexer yyDLFlexLexer
#include <FlexLexer.h>
#endif

namespace dlvhex {
  namespace dl {
    // forward declaration
    class HexDLRewriterDriver;
  } // namespace dl
} // namespace dlvhex

/**
 * @brief Use a refined yyFlexLexer.
 *
 * Sad but true, we can't put HexDLFlexLexer into the
 * dlvhex::dl namespace, flex is not aware of namespaces.
 */
struct HexDLFlexLexer : public yyDLFlexLexer
{
  explicit
  HexDLFlexLexer(dlvhex::dl::HexDLRewriterDriver* d)
    : lexdrv(d), lexloc(0), lexval(0)
  { }

  HexDLFlexLexer(const HexDLRewriterFlexLexer& r)
    : yyDLFlexLexer(), lexdrv(r.lexdrv), lexloc(r.lexloc), lexval(r.lexval)
  { }

  HexDLRewriterFlexLexer&
  operator= (const HexDLRewriterFlexLexer& r)
  {
    if (this != &r)
      {
	lexdrv = r.lexdrv;
	lexloc = r.lexloc;
	lexval = r.lexval;
      }
    return *this;
  }

  ~HexDLRewriterFlexLexer() { }

  dlvhex::dl::HexDLRewriterDriver* lexdrv;
  yy::HexDLRewriterParser::location_type* lexloc;
  yy::HexDLRewriterParser::semantic_type* lexval;

  int yylex(); // implemented in HexDLScanner.lpp
};


#endif // _HEXDLFLEXLEXER_H
