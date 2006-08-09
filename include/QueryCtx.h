/* -*- C++ -*- */

/**
 * @file   QueryCtx.h
 * @author Thomas Krennwallner
 * @date   Thu Aug  3 12:13:40 2006
 * 
 * @brief  Encapsulates a Query and the corresponding Answer.
 * 
 * 
 */


#ifndef _QUERYCTX_H
#define _QUERYCTX_H

#include "RacerError.h"

#include <dlvhex/PluginInterface.h>

#include <boost/shared_ptr.hpp>

namespace dlvhex {
namespace racer {

  //
  // forward declarations
  //
  class Query;
  class Answer;


  /**
   * @brief Encapsulates a Query and the corresponding Answer.
   */
  class QueryCtx
  {
  private:
    Query* q;  /// the Query object
    Answer* a; /// the Answer object

  public:
    /** 
     * Ctor which utilizes PluginAtom::Query to fill the members of
     * @a q and creates @a a with a reference to @a q.
     * 
     * @param query 
     */
    explicit
    QueryCtx(const PluginAtom::Query& query) throw (RacerError);


    /** 
     * Ctor which takes a Query and an Answer.
     * 
     * @param qq 
     * @param aa 
     */
    QueryCtx(Query* qq, Answer* aa)
      : q(qq), a(aa)
    { }

    virtual
    ~QueryCtx();

    virtual void
    setQuery(Query* q);

    virtual Query&
    getQuery() const;

    virtual void
    setAnswer(Answer* a);

    virtual Answer&
    getAnswer() const;

    typedef QueryCtx value_type;
    typedef boost::shared_ptr<value_type> shared_pointer; /// managed QueryCtx
  };

} // namespace racer
} // namespace dlvhex

#endif /* _RACERQUERY_H */
