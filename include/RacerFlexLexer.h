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

#include "RacerAnswerParser.hpp"

// put FlexLexer.h into its own include guards or yyFlexLexer gets
// redefined
#ifndef __FLEX_LEXER_H
#include <FlexLexer.h>
#endif

//
// some forward declarations
//

namespace dlvhex {
namespace racer {
  class RacerBaseAnswerDriver;
} // namespace racer
} // namespace dlvhex

  /**
   * @brief Use a refined yyFlexLexer.
   *
   * Sad but true, we can't put RacerFlexLexer into the dlvhex::racer
   * namespace, flex is not aware of namespaces.
   */
  struct RacerFlexLexer : public yyFlexLexer
  {
    RacerFlexLexer(dlvhex::racer::RacerBaseAnswerDriver* d) : lexdrv(d) { }
    virtual ~RacerFlexLexer() { }
    dlvhex::racer::RacerBaseAnswerDriver* lexdrv;
    yy::RacerAnswerParser::location_type* lexloc;
    yy::RacerAnswerParser::semantic_type* lexval;
    int yylex(); // implemented in RacerAnswerScanner.lpp
  };

#endif // _RACERFLEXLEXER_H
