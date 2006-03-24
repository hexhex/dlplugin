/* -*- C++ -*- */

/**
 * @file   RacerAnswerDriver.h
 * @author Thomas Krennwallner
 * @date   Fri Mar 17 22:26:20 2006
 * 
 * @brief  
 * 
 * 
 */


#ifndef _RACERANSWERDRIVER_H
#define _RACERANSWERDRIVER_H

#include <dlvhex/Term.h>

#include "RacerQuery.h"
#include "RacerFlexLexer.h"
#include "RacerError.h"

#include <iostream>
#include <string>

namespace dlvhex {
namespace racer {

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

    void
    syncStream();

    explicit
    RacerBaseAnswerDriver(std::istream&);

    virtual
    ~RacerBaseAnswerDriver();

  public:

    /**
     * Parses the answers from RACER.
     *
     * @param answer
     */
    virtual void
    parse(Answer& answer) throw (RacerParsingError) = 0;

    // Error handling.
    void
    error(const yy::location& l, const std::string& m) throw (RacerParsingError);

    void
    error(const std::string& m) throw (RacerParsingError);

    RacerFlexLexer*
    getLexer();
  };


  /**
   * @brief Conducting the whole scanning and parsing of Racer
   * Answers.
   */
  class RacerAnswerDriver : public RacerBaseAnswerDriver
  {
  public:
    RacerAnswerDriver(std::istream& is);

    virtual
    ~RacerAnswerDriver();

    // Handling the parser.
    virtual void
    parse(Answer& a) throw (RacerParsingError);
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

    virtual
    ~RacerIgnoreAnswer();

    /// just remove pending input from stream without parsing it
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

    virtual
    ~RacerNullParser()
    { }

    /// does nothing
    virtual void
    parse(Answer&) throw (RacerParsingError)
    { }
  };



} // namespace racer
} // namespace dlvhex

#endif // _RACERANSWERDRIVER_H

