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

#include "RacerAnswerParser.hpp"
#include "RacerError.h"

#include <iosfwd>
#include <string>

//
// forward declarations
//
class RacerFlexLexer;


namespace dlvhex {
namespace racer {

  //
  // forward declarations
  //
  class Answer;


  /**
   * @brief Base class for the parsers.
   */
  class RacerBaseAnswerDriver
  {
  protected:
    /// the parser read the answers from this stream
    std::istream& stream;
    /// lexer object which scans the stream
    RacerFlexLexer* lexer;

    /// sync and clear #stream
    void
    syncStream();

    /// protected ctor.
    explicit
    RacerBaseAnswerDriver(std::istream&);

  public:
    virtual
    ~RacerBaseAnswerDriver();

    /**
     * Parses the answers from RACER.
     *
     * @param answer
     */
    virtual void
    parse(Answer& answer) throw (RacerParsingError) = 0;

    /// Error handling.
    void
    error(const yy::location& l, const std::string& m) throw (RacerParsingError);

    /// Error handling.
    void
    error(const std::string& m) throw (RacerParsingError);

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
    RacerAnswerDriver(std::istream& is);

    virtual void
    parse(Answer& answer) throw (RacerParsingError);
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
    parse(Answer&) throw (RacerParsingError);
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
    parse(Answer&) throw (RacerParsingError)
    { }
  };



} // namespace racer
} // namespace dlvhex

#endif // _RACERANSWERDRIVER_H

