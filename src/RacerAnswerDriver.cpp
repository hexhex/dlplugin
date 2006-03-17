#include <iostream>

#include "RacerAnswerDriver.h"
#include "RacerQuery.h"


yy::RacerAnswerDriver::RacerAnswerDriver ()
{
  lexer = new RacerFlexLexer(this);
}
     
yy::RacerAnswerDriver::~RacerAnswerDriver ()
{
  delete lexer;
}

RacerFlexLexer*
yy::RacerAnswerDriver::getLexer()
{
  return lexer;
}

void
yy::RacerAnswerDriver::parse (std::istream& is, dlvhex::racer::Answer &a)
{
  scan_begin ();
  yy::RacerAnswerParser parser (*this);
  parser.set_debug_level(true);
  parser.parse ();
  scan_end ();
}
     
void
yy::RacerAnswerDriver::error (const yy::location& l, const std::string& m)
{
  std::cerr << "shit happened at " << l << ": " << m << std::endl;
}
     
void
yy::RacerAnswerDriver::error (const std::string& m)
{
  std::cerr << m << std::endl;
}
