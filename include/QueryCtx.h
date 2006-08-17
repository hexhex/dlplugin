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

#include "DLError.h"

#include <dlvhex/PluginInterface.h>

#include <boost/shared_ptr.hpp>

#include <iosfwd>

namespace dlvhex {
namespace dl {

  //
  // forward declarations
  //
  class Query;
  class Answer;
  class KBManager;


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
     * @param kb
     */
    explicit
    QueryCtx(const PluginAtom::Query& query, KBManager& kb) throw (DLError);


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

    typedef boost::shared_ptr<QueryCtx> shared_pointer; /// managed QueryCtx

    friend std::ostream&
    operator<< (std::ostream& os, const QueryCtx& q);

    friend bool
    operator< (const QueryCtx& q1, const QueryCtx& q2);


  };


  /**
   * @brief put the string representation of @a q into @a os.
   * @param os
   * @param q
   * @return @a os
   */
  std::ostream&
  operator<< (std::ostream& os, const QueryCtx& q);

  /**
   * @brief lexicographically compare @a q1 to @a q2 and check if @a
   * q1 is less than @a q2.
   *
   * @param q1
   * @param q2
   * @return true if @a q1 < @a q2, false otherwise.
   */
  bool
  operator< (const QueryCtx& q1, const QueryCtx& q2);


} // namespace dl
} // namespace dlvhex

#endif /* _RACERQUERY_H */
