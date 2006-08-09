/* -*- C++ -*- */

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

#include <dlvhex/PluginInterface.h>
#include <dlvhex/Term.h>

#include <string>

namespace dlvhex {
namespace racer {

  //
  // forward declaration
  //
  class Query;



  /**
   * @brief The Answer to a Query.
   */
  class Answer : public PluginAtom::Answer
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
    /// to fill tuples in some query flavours
    const Query* query;

  public:
    explicit
    Answer(const Query* q);

    virtual
    ~Answer();

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
    addTuple(const Tuple& out);

  };

} // namespace racer
} // namespace dlvhex

#endif /* _ANSWER_H */
