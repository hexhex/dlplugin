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

namespace dlvex_racer {

  /**
   * @brief Base class for the parsers.
   */
  class RacerParse
  {
  protected:
    std::istream& stream;

    explicit
    RacerParse(std::istream&);

    virtual
    ~RacerParse();

    /**
     * Template method.
     */
    virtual void
    parseAnswer(Answer&, std::string&, std::string&) const = 0;

  public:
    virtual void
    parse(Answer&) throw (RacerParsingError);
  };


  /**
   * @brief Ignores a RACER reply to parse answer and errors without
   * an exception.
   */
  class RacerIgnoreAnswer : public RacerParse
  {
  protected:
    virtual void
    parseAnswer(Answer&, std::string&, std::string&) const;

  public:
    explicit
    RacerIgnoreAnswer(std::istream&);

    virtual
    ~RacerIgnoreAnswer();

    virtual void
    parse(Answer&) throw (RacerParsingError);
  };


  /**
   * @brief Parses simple answers from RACER.
   */
  class RacerSimpleAnswer : public RacerParse
  {
  protected:
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
    virtual void
    parseAnswer(Answer&, std::string&, std::string&) const throw (RacerParsingError);

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
    virtual void
    parseAnswer(Answer&, std::string&, std::string&) const throw (RacerParsingError);

    virtual std::string&
    parseTerm(std::string&) const;

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
    virtual void
    parseAnswer(Answer&, std::string&, std::string&) const throw (RacerParsingError);

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

    virtual void
    parse(Answer&) throw (RacerParsingError)
    { }
  };

} /* namespace dlvex_racer */

#endif /* _RACERPARSE_H */
