/* dlvhex-dlplugin -- Integration of Answer-Set Programming and Description Logics.
 *
 * Copyright (C) 2005, 2006, 2007  Thomas Krennwallner
 * 
 * This file is part of dlvhex-dlplugin.
 *
 * dlvhex-dlplugin is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * dlvhex-dlplugin is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dlvhex-dlplugin; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


/**
 * @file   RacerAnswerDriver.cpp
 * @author Thomas Krennwallner
 * @date   Sat Mar 18 15:04:27 2006
 * 
 * @brief Driver classes which encapsulates the bison parser and flex
 * lexer.
 * 
 * 
 */

#include "RacerAnswerDriver.h"
#include "Answer.h"
#include "DLError.h"
#include "Registry.h"

#include <iostream>
#include <sstream>

using namespace dlvhex::dl::racer;


RacerBaseAnswerDriver::RacerBaseAnswerDriver(std::istream& i)
  : QueryBaseParser(i)
{ }


RacerBaseAnswerDriver::RacerBaseAnswerDriver(const RacerBaseAnswerDriver& d)
  : QueryBaseParser(d)
{ }


RacerBaseAnswerDriver&
RacerBaseAnswerDriver::operator= (const RacerBaseAnswerDriver&)
{
  return *this; // ignore
}


RacerBaseAnswerDriver::~RacerBaseAnswerDriver()
{
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


RacerAnswerDriver::RacerAnswerDriver(std::istream& i)
  : RacerBaseAnswerDriver(i)
{ }


void
RacerAnswerDriver::parse(Answer &a) throw (DLParsingError)
{
  try
    {
#if 0
      if (!stream.eof())
	{
	  yy::RacerAnswerParser parser(*this, a);
	  parser.set_debug_level(Registry::getVerbose() > 2 ? true : false);
	  lexer->switch_streams(&stream, &std::cerr);
	  parser.parse();
	  syncStream();
	}
#endif
    }
  catch (std::ios_base::failure& f)
    {
      throw DLParsingError(f.what());
    }
}



RacerIgnoreAnswer::RacerIgnoreAnswer(std::istream& s)
  : RacerBaseAnswerDriver(s)
{ }


void
RacerIgnoreAnswer::parse(Answer&) throw (DLParsingError)
{
  // Just skip the answer. This is useful for such "glorious" Racer
  // responses as "T" for a (set-unique-name-assumption t). Otherwise
  // we would end up with an additional empty tuple in the Answer
  // object.

  try
    {
      char tmp[256];
      
      while (!stream.eof() && !stream.fail())
	{
	  stream.read(tmp, 256);
	}

      // reset stream so we can reutilize it
      syncStream();
    }
  catch (std::ios_base::failure& f)
    {
      throw DLParsingError(f.what());
    }

}


// Local Variables:
// mode: C++
// End:
