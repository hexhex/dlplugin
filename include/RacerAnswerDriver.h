/* -*- C++ -*- */

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

#include "RacerAnswerParser.hpp"

#include <iosfwd>
#include <string>

//
// forward declarations
//
class RacerFlexLexer;


namespace dlvhex {
namespace dl {

  //
  // forward declarations
  //
  class Answer;

namespace racer {


  /**
   * @brief Base class for the Racer parsers.
   */
  class RacerBaseAnswerDriver : public QueryBaseParser
  {
  protected:
    /// lexer object which scans the stream
    RacerFlexLexer* lexer;

    /// sync and clear #stream
    void
    syncStream();

    /// protected ctor.
    explicit
    RacerBaseAnswerDriver(std::istream&);

  public:
    ~RacerBaseAnswerDriver();

    /// Error handling.
    void
    error(const yy::location& l, const std::string& m) throw (DLParsingError);

    /// Error handling.
    void
    error(const std::string& m) throw (DLParsingError);

    RacerFlexLexer*
    getLexer() const;
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

