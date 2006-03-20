/* -*- C++ -*- */

/**
 * @file   RacerAnswerDriver.h
 * @author Thomas Krennwallner
 * @date   Fri Mar 17 22:26:20 2006
 * 
 * @brief  
 * 
 * 
 */


#ifndef _RACERANSWERDRIVER_H
#define _RACERANSWERDRIVER_H


// Bison doesn't #define YY_DECL, so we do.
#ifndef YY_DECL

//
// Forward declarations.
//

union YYSTYPE;

namespace yy
{
  class location;
  class RacerAnswerParser;
}

namespace dlvhex {
namespace racer {
  class RacerAnswerDriver;
}
}

// Announce to Flex the prototype we want for lexing function, ...
#define YY_DECL								\
  int yylex (YYSTYPE* yylval,						\
	     yy::location* yylloc,					\
	     dlvhex::racer::RacerAnswerDriver& driver)

// ... and declare it for the parser's sake.
YY_DECL;
#endif /* YY_DECL */

#include <dlvhex/Term.h>

#include "RacerQuery.h"
#include "RacerFlexLexer.h"
#include "RacerError.h"

#include <iostream>
#include <string>

namespace dlvhex {
namespace racer {

  /**
   * @brief Conducting the whole scanning and parsing of Racer
   * Answers.
   */
  class RacerAnswerDriver
  {
  private:
    /// parse this stream
    std::istream& is;
    /// lexer object which scans the stream
    RacerFlexLexer* lexer;

    void
    syncStream();

  public:
    RacerAnswerDriver(std::istream& is);

    virtual
    ~RacerAnswerDriver();

    // Handling the parser.
    void
    parse(dlvhex::racer::Answer& a) throw (RacerParsingError);

    // Error handling.
    void
    error(const yy::location& l, const std::string& m) throw (RacerParsingError);

    void
    error(const std::string& m) throw (RacerParsingError);

    RacerFlexLexer*
    getLexer();
  };

} // namespace racer
} // namespace dlvhex

#endif // _RACERANSWERDRIVER_H

