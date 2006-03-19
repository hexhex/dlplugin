// -*- C++ -*-

/**
 * @file   RacerFlexLexer.h
 * @author Thomas Krennwallner
 * @date   Fri Mar 17 22:06:12 2006
 * 
 * @brief  
 * 
 * 
 */

#ifndef _RACERFLEXLEXER_H
#define _RACERFLEXLEXER_H

// put FlexLexer.h into its own include guards, some kind of kludge
#ifndef __FLEX_LEXER_H
#include <FlexLexer.h>
#endif

//
// some forward declarations
//

namespace yy {
  class location;
} // namespace yy

union YYSTYPE;

namespace dlvhex {
namespace racer {
  class RacerAnswerDriver;
} // namespace racer
} // namespace dlvhex

  /**
   * @brief Use a refined yyFlexLexer.
   */
  struct RacerFlexLexer : public yyFlexLexer
  {
    RacerFlexLexer(dlvhex::racer::RacerAnswerDriver* d) : lexdrv(d) { }
    virtual ~RacerFlexLexer() { }
    dlvhex::racer::RacerAnswerDriver* lexdrv;
    yy::location* lexloc;
    YYSTYPE* lexval;
    int yylex(); // implemented in lex.yy.cc
  };

#endif // _RACERFLEXLEXER_H
