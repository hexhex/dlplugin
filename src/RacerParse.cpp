/* -*- C++ -*- */

/**
 * @file   RacerParse.cpp
 * @author Thomas Krennwallner
 * @date   Sat Jul 16 11:39:20 2005
 * 
 * @brief  Various parsers for RACER answers.
 * 
 * 
 */

#include "RacerParse.h"
#include "RacerAnswerDriver.h"
#include "RacerQuery.h"

#include <iterator>
#include <iostream>
#include <sstream>

using namespace dlvhex::racer;



RacerParse::RacerParse(std::istream& s)
  : stream(s)
{ }

RacerParse::~RacerParse()
{ }

void
RacerParse::parse(Answer& answer) throw (RacerParsingError)
{
  try
    {
      if (!stream.eof())
	{
	  RacerAnswerDriver driver(stream);
	  driver.parse(answer);
	}
    }
  catch (std::exception& e)
    {
      throw RacerParsingError(e.what());
    }
}



RacerIgnoreAnswer::RacerIgnoreAnswer(std::istream& s)
  : RacerParse(s)
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
  stream.sync();
  stream.clear();
}
