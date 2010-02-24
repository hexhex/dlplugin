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
 * @file   RacerAnswerDriver.h
 * @author Thomas Krennwallner
 * @date   Fri Mar 17 22:26:20 2006
 * 
 * @brief  Driver classes for the RacerAnswer bison/flex parser.
 * 
 * 
 */


#ifndef _RACERANSWERDRIVER_H
#define _RACERANSWERDRIVER_H

#include "DLError.h"
#include "QueryDirector.h"

#include <iosfwd>
#include <string>

namespace dlvhex {
namespace dl {

class Answer;

namespace racer {


  /**
   * @brief Base class for the Racer parsers.
   */
  class RacerBaseAnswerDriver : public QueryBaseParser
  {
  protected:
    /// sync and clear #stream
    void
    syncStream();

    /// protected ctor.
    explicit
    RacerBaseAnswerDriver(std::istream&);

    /// protected copy ctor
    RacerBaseAnswerDriver(const RacerBaseAnswerDriver&);

    /// protected assignment op
    RacerBaseAnswerDriver&
    operator= (const RacerBaseAnswerDriver&);

  public:
    /// dtor
    ~RacerBaseAnswerDriver();
  };


  /**
   * @brief Conducting the whole scanning and parsing of Racer
   * Answers.
   */
  class RacerAnswerDriver : public RacerBaseAnswerDriver
  {
  public:
    explicit
    RacerAnswerDriver(std::istream& is);

    virtual void
    parse(Answer& answer) throw (DLParsingError);
  };


  /**
   * @brief Ignores a RACER reply to parse answer and errors without
   * an exception.
   */
  class RacerIgnoreAnswer : public RacerBaseAnswerDriver
  {
  public:
    explicit
    RacerIgnoreAnswer(std::istream&);

    /// just remove pending input from #stream without parsing it.
    virtual void
    parse(Answer&) throw (DLParsingError);
  };


  /**
   * @brief A parser which does nothing. Mainly for testing purposes.
   */
  class RacerNullParser : public RacerBaseAnswerDriver
  {
  public:
    explicit
    RacerNullParser(std::istream& s)
      : RacerBaseAnswerDriver(s)
    { }

    /// does nothing
    virtual void
    parse(Answer&) throw (DLParsingError)
    { }
  };



} // namespace racer
} // namespace dl
} // namespace dlvhex

#endif // _RACERANSWERDRIVER_H


// Local Variables:
// mode: C++
// End:
