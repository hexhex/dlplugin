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
 * @file   Answer.h
 * @author Thomas Krennwallner
 * @date   Thu Aug  3 12:01:39 2006
 * 
 * @brief  The Answer to a Query.
 * 
 * 
 */


#ifndef _ANSWER_H
#define _ANSWER_H

#include <dlvhex2/ComfortPluginInterface.h>

#include <string>
#include <iosfwd>

namespace dlvhex {
namespace dl {

  //
  // forward declaration
  //
  class Query;



  /**
   * @brief The Answer to a Query.
   */
  class Answer : public ComfortPluginAtom::ComfortAnswer
  {
  private:
    /// a RACER error message 
    std::string errorMsg;
    /// a RACER warning message
    std::string warningMsg;
    /// ABox is inconsistent
    bool isIncoherent;
    /// a boolean answer
    bool answer;
    /// keep a reference to our query, so we can determine if we need
    /// to fill tuples in a specific way
    const Query* query;

  public:
    /// ctor
    explicit
    Answer(const Query* q);

    /// copy ctor
    Answer(const Answer& a);

    /// assignment op
    Answer&
    operator= (const Answer& a);

    /// dtor
    virtual
    ~Answer()
    { }

    virtual void
    setQuery(const Query* q);

    virtual void
    setIncoherent(bool isIncoherent);

    virtual bool
    getIncoherent() const;

    virtual void
    setErrorMessage(const std::string& errorMsg);

    virtual const std::string&
    getErrorMessage() const;

    virtual void
    setWarningMessage(const std::string& warningMsg);

    virtual const std::string&
    getWarningMessage() const;

    virtual void
    setAnswer(bool answer);

    virtual bool
    getAnswer() const;

    virtual void
    addTuple(const ComfortTuple& out);

    friend std::ostream&
    operator<< (std::ostream& os, const Answer& a);

  };


  /** 
   * Determines whether @a a is a boolean or a non-boolean answer and
   * then puts the string-representation of @a a into @a os.
   * 
   * @param os 
   * @param a 
   * 
   * @return @a os
   */
  std::ostream&
  operator<< (std::ostream& os, const Answer& a);

  

} // namespace dl
} // namespace dlvhex

#endif /* _ANSWER_H */



// Local Variables:
// mode: C++
// End:
