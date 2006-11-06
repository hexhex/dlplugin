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
    class HexDLDriver;
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
  HexDLFlexLexer(dlvhex::dl::HexDLDriver* d)
    : lexdrv(d), lexloc(0), lexval(0)
  { }

  HexDLFlexLexer(const HexDLFlexLexer& r)
    : yyDLFlexLexer(), lexdrv(r.lexdrv), lexloc(r.lexloc), lexval(r.lexval)
  { }

  HexDLFlexLexer&
  operator= (const HexDLFlexLexer& r)
  {
    if (this != &r)
      {
	lexdrv = r.lexdrv;
	lexloc = r.lexloc;
	lexval = r.lexval;
      }
    return *this;
  }

  ~HexDLFlexLexer() { }

  dlvhex::dl::HexDLDriver* lexdrv;
  yy::HexDLParser::location_type* lexloc;
  yy::HexDLParser::semantic_type* lexval;

  int yylex(); // implemented in HexDLScanner.lpp
};


#endif // _HEXDLFLEXLEXER_H
