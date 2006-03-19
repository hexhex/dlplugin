/* -*- C++ -*- */

/**
 * @file   RacerParse.h
 * @author Thomas Krennwallner
 * @date   Sat Jul 16 11:39:59 2005
 * 
 * @brief  Various parsers for RACER answers.
 * 
 * 
 */

#ifndef _RACERPARSE_H
#define _RACERPARSE_H

#include <iostream>

#include <dlvhex/Atom.h>
#include <dlvhex/Term.h>

#include "RacerQuery.h"
#include "RacerError.h"

namespace dlvhex {
namespace racer {

  /**
   * @brief Base class for the parsers.
   */
  class RacerParse
  {
  protected:
    /// the parsers read the answers from this stream
    std::istream& stream;

  public:
    explicit
    RacerParse(std::istream&);

    virtual
    ~RacerParse();

    /**
     * Parses the answers from RACER and sets various members of
     * Answer with help of parseAnswer().
     *
     * @param answer
     */
    virtual void
    parse(Answer& answer) throw (RacerParsingError);
  };


  /**
   * @brief Ignores a RACER reply to parse answer and errors without
   * an exception.
   */
  class RacerIgnoreAnswer : public RacerParse
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
  class RacerNullParser : public RacerParse
  {
  public:
    explicit
    RacerNullParser(std::istream& s)
      : RacerParse(s)
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

#endif /* _RACERPARSE_H */
