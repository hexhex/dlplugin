/* -*- C++ -*- */

/**
 * @file   HexDLRewriterFlexLexer.h
 * @author Thomas Krennwallner
 * @date   Tue Jul 25 09:38:15 2006
 * 
 * @brief  A FlexLexer class for the Hex DL Rewriters flex scanner.
 * 
 * 
 */

#ifndef _HEXDLREWRITERFLEXLEXER
#define _HEXDLREWRITERFLEXLEXER

// put FlexLexer.h into its own include guards or yyFlexLexer gets
// redefined
#ifndef __FLEX_LEXER_H
#undef yyFlexLexer
#define yyFlexLexer yyHexFlexLexer
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
 * Sad but true, we can't put HexDLRewriterFlexLexer into the
 * dlvhex::racer namespace, flex is not aware of namespaces.
 */
struct HexDLRewriterFlexLexer : public yyHexFlexLexer
{
  explicit
  HexDLRewriterFlexLexer(dlvhex::dl::HexDLRewriterDriver* d) : lexdrv(d) { }
  virtual
  ~HexDLRewriterFlexLexer() { }
  dlvhex::dl::HexDLRewriterDriver* lexdrv;
  yy::HexDLRewriterParser::location_type* lexloc;
  yy::HexDLRewriterParser::semantic_type* lexval;
  int yylex(); // implemented in HexDLRewriterScanner.lpp
};


#endif // _HEXDLREWRITERFLEXLEXER_H
