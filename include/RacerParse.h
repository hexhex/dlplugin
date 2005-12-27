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

    explicit
    RacerParse(std::istream&);

    virtual
    ~RacerParse();

    /**
     * Template method implemented in the children of RacerParse.
     *
     * @param answer set members accourding to ans
     * @param ans the parsed answer from parse()
     * @param ns the parsed namespace list from parse()
     */
    virtual void
    parseAnswer(Answer& answer, std::string& ans, std::string& ns) const = 0;

  public:
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
  protected:
    /// does nothing
    virtual void
    parseAnswer(Answer&, std::string&, std::string&) const;

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
   * @brief Parses simple answers from RACER.
   */
  class RacerSimpleAnswer : public RacerParse
  {
  protected:
    /// does nothing
    virtual void
    parseAnswer(Answer&, std::string&, std::string&) const throw (RacerParsingError);

  public:
    explicit
    RacerSimpleAnswer(std::istream&);

    virtual
    ~RacerSimpleAnswer();
  };


  /**
   * @brief Parses boolean answers from RACER.
   */
  class RacerBooleanAnswer : public RacerParse
  {
  protected:
    /**
     * checks RACERs boolean answer and sets Answer accordingly.
     *
     * @param answer
     * @param ans
     * @param ns
     */
    virtual void
    parseAnswer(Answer& answer, std::string& ans, std::string& ns) const throw (RacerParsingError);

  public:
    explicit
    RacerBooleanAnswer(std::istream&);

    virtual
    ~RacerBooleanAnswer();
  };


  /**
   * @brief Parses list answers from RACER.
   */
  class RacerAnswerList : public RacerParse
  {
  protected:
    /**
     * checks RACERs individual list answer and sets Answer accordingly.
     *
     * @param answer
     * @param ans
     * @param ns
     */
    virtual void
    parseAnswer(Answer& answer, std::string& ans, std::string& ns) const throw (RacerParsingError);

    /**
     * helper method processes a single individual parsed from a RACER
     * answer and strips xml namespace from it.
     *
     * @param term
     *
     * @return individual string
     */
    virtual std::string&
    parseTerm(std::string& term) const;

  public:
    explicit
    RacerAnswerList(std::istream&);

    virtual
    ~RacerAnswerList();
  };


  /**
   * @brief Parses list answerpairs from RACER.
   */
  class RacerAnswerPairList : public RacerAnswerList
  {
  protected:
    /**
     * checks RACERs pair list answer and sets Answer accordingly.
     *
     * @param answer
     * @param ans
     * @param ns
     */
    virtual void
    parseAnswer(Answer& answer, std::string& ans, std::string& ns) const throw (RacerParsingError);

  public:
    explicit
    RacerAnswerPairList(std::istream&);

    virtual
    ~RacerAnswerPairList();
  };


  /**
   * @brief A parser which does nothing. Mainly for testing purposes.
   */
  class RacerNullParser : public RacerParse
  {
  protected:
    /// does nothing
    virtual void
    parseAnswer(Answer&, std::string&, std::string&) const
    { }

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
