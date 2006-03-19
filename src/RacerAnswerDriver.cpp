/**
 * @file   RacerAnswerDriver.cpp
 * @author Thomas Krennwallner
 * @date   Sat Mar 18 15:04:27 2006
 * 
 * @brief  
 * 
 * 
 */

#include <iostream>

#include "RacerFlexLexer.h"
#include "RacerAnswerParser.hpp"
#include "RacerQuery.h"
#include "RacerError.h"

using namespace dlvhex::racer;

RacerAnswerDriver::RacerAnswerDriver(std::istream& i)
  : is(i),
    lexer(new RacerFlexLexer(this))
{
}
     
RacerAnswerDriver::~RacerAnswerDriver()
{
  delete lexer;
}

RacerFlexLexer*
RacerAnswerDriver::getLexer()
{
  return lexer;
}

void
RacerAnswerDriver::syncStream()
{
  // sync and clear stream s.t. consecutive reading on the stream
  // works. Otherwise we would need to create a dedicated iostream for
  // each Racer command.

  is.sync();
  is.clear();
}

void
RacerAnswerDriver::parse(Answer &a) throw (RacerParsingError)
{
  yy::RacerAnswerParser parser(*this, a);
  parser.set_debug_level(false);
  lexer->switch_streams(&is, &std::cerr);
  parser.parse();
  syncStream();
}

void
RacerAnswerDriver::error(const yy::location& l,
			 const std::string& m) throw (RacerParsingError)
{
  syncStream();
  std::stringstream s;
  s << "Parsing error at " << l << ": " << m;
  throw RacerParsingError(s.str());
}
     
void
RacerAnswerDriver::error(const std::string& m) throw (RacerParsingError)
{
  syncStream();
  throw dlvhex::racer::RacerParsingError("Parsing error: " + m);
}
