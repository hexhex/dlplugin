// -*- C++ -*-

#ifndef _RACERFLEXLEXER_H
#define _RACERFLEXLEXER_H

#ifndef __FLEX_LEXER_H
#include <FlexLexer.h>
#endif

class RacerFlexLexer : public yyFlexLexer
{
public:
  RacerFlexLexer(yy::RacerAnswerDriver* d) : driver(d) { }
  virtual ~RacerFlexLexer() { }
  yy::RacerAnswerDriver* driver;
  yy::location* lexloc;
  YYSTYPE* lexval;
  int yylex(); // implemented in lex.yy.cc
};


#endif
