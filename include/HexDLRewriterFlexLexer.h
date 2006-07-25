/* -*- C++ -*- */

/**
 * @file   HexDLRewriterFlexLexer.h
 * @author Thomas Krennwallner
 * @date   Tue Jul 25 09:38:15 2006
 * 
 * @brief  
 * 
 * 
 */

#ifndef _HEXDLREWRITERFLEXLEXER
#define _HEXDLREWRITERFLEXLEXER

//#include "HexDLRewriterParser.hpp"

// put FlexLexer.h into its own include guards or yyFlexLexer gets
// redefined
#ifndef __FLEX_LEXER_H
#undef yyFlexLexer
#define yyFlexLexer zzFlexLexer
#include <FlexLexer.h>
#endif

/// forward declaration
namespace dlvhex {
namespace racer {
  class HexDLRewriterDriver;
} // namespace racer
} // namespace dlvhex

/**
 * @brief Use a refined yyFlexLexer.
 *
 * Sad but true, we can't put HexDLRewriterFlexLexer into the
 * dlvhex::racer namespace, flex is not aware of namespaces.
 */
class HexDLRewriterFlexLexer : public zzFlexLexer
{
public:
  HexDLRewriterFlexLexer(dlvhex::racer::HexDLRewriterDriver* d) : lexdrv(d) { }
  virtual ~HexDLRewriterFlexLexer() { }
  dlvhex::racer::HexDLRewriterDriver* lexdrv;
//   yy::HexParser::location_type* lexloc;
//   yy::HexParser::semantic_type* lexval;
  int yylex(); // implemented in HexDLRewriterScanner.lpp
};


#endif // _HEXDLREWRITERFLEXLEXER_H
