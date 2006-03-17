// -*- C++ -*-

#ifndef _RACERANSWERDRIVER_H
#define _RACERANSWERDRIVER_H


namespace yy
{
  class location;
  class RacerAnswerParser;
  class RacerAnswerDriver;
}

#include "RacerAnswerParser.tab.hh"

// Forward declarations.
union YYSTYPE;

#ifndef YY_DECL
// Announce to Flex the prototype we want for lexing function, ...
#define YY_DECL	\
       int yylex (YYSTYPE* yylval, yy::location* yylloc, yy::RacerAnswerDriver& driver)
// ... and declare it for the parser's sake.
YY_DECL;
#endif 

#include "dlvhex/Term.h"
#include "RacerQuery.h"
#include <iostream>
#include <string>
#include "RacerFlexLexer.h"

namespace yy {


// Conducting the whole scanning and parsing of Racer Answers
class RacerAnswerDriver
{
private:
  RacerFlexLexer* lexer;

public:
  RacerAnswerDriver ();
  virtual ~RacerAnswerDriver ();
     
  // Handling the scanner.
  void scan_begin ();
  void scan_end ();
  
  // Handling the parser.
  void parse (std::istream& is, dlvhex::racer::Answer& a);

  // Error handling.
  void error (const yy::location& l, const std::string& m);
  void error (const std::string& m);

  RacerFlexLexer* getLexer();
};

}

#endif // _RACERANSWERDRIVER_H

