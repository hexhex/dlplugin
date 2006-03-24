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

#include "RacerAnswerDriver.h"
#include "RacerFlexLexer.h"
#include "RacerAnswerParser.hpp"
#include "RacerQuery.h"
#include "RacerError.h"

using namespace dlvhex::racer;


RacerBaseAnswerDriver::RacerBaseAnswerDriver(std::istream& i)
  : stream(i),
    lexer(new RacerFlexLexer(this))
{
}
     
RacerBaseAnswerDriver::~RacerBaseAnswerDriver()
{
  delete lexer;
}

RacerFlexLexer*
RacerBaseAnswerDriver::getLexer()
{
  return lexer;
}

void
RacerBaseAnswerDriver::syncStream()
{
  // sync and clear stream s.t. consecutive reading on the stream
  // works. Otherwise we would need to create a dedicated iostream for
  // each Racer command.

  stream.sync();
  stream.clear();
}


void
RacerBaseAnswerDriver::error(const yy::location& l,
			     const std::string& m) throw (RacerParsingError)
{
  syncStream();
  std::stringstream s;
  s << "Parsing error at " << l << ": " << m;
  throw RacerParsingError(s.str());
}
     
void
RacerBaseAnswerDriver::error(const std::string& m) throw (RacerParsingError)
{
  syncStream();
  throw RacerParsingError("Parsing error: " + m);
}



RacerAnswerDriver::RacerAnswerDriver(std::istream& i)
  : RacerBaseAnswerDriver(i)
{ }

RacerAnswerDriver::~RacerAnswerDriver()
{ }

void
RacerAnswerDriver::parse(Answer &a) throw (RacerParsingError)
{
  if (!stream.eof())
    {
      yy::RacerAnswerParser parser(*this, a);
      parser.set_debug_level(false);
      lexer->switch_streams(&stream, &std::cerr);
      parser.parse();
      syncStream();
    }
}



RacerIgnoreAnswer::RacerIgnoreAnswer(std::istream& s)
  : RacerBaseAnswerDriver(s)
{ }

RacerIgnoreAnswer::~RacerIgnoreAnswer()
{ }

void
RacerIgnoreAnswer::parse(Answer&) throw (RacerParsingError)
{
  // Just skip the answer. This is useful for such "glorious" Racer
  // responses as "T" for a (set-unique-name-assumption t). Otherwise
  // we would end up with an additional empty tuple in the Answer
  // object.

  char tmp[256];

  while (!stream.eof() && !stream.fail())
    {
      stream.read(tmp, 256);
    }

  // reset stream so we can reutilize it
  syncStream();
}
