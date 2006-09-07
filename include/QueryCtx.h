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
#include "Query.h"

#include <dlvhex/PluginInterface.h>

#include <boost/shared_ptr.hpp>

#include <iosfwd>

namespace dlvhex {
namespace dl {

  //
  // forward declarations
  //
  class Answer;
  class KBManager;


  /**
   * @brief Encapsulates a Query and the corresponding Answer.
   *
   * Used for wrapping a whole querying context into a handy
   * object. Also allows to instantiate Query and Answer objects from
   * PluginAtom::Query.
   */
  class QueryCtx
  {
  private:
    /// the encapsulated Query object
    Query* q;
    /// the encapsulated Answer object
    Answer* a;

    //
    // keep copy ctor and assignment operator private, we don't want
    // to copy QueryCtx objects
    //

    /// private copy ctor
    QueryCtx(const QueryCtx&);

    /// private assignment operator
    QueryCtx&
    operator= (const QueryCtx&);

  public:
    /** 
     * Ctor which utilizes PluginAtom::Query to fill the members of #q
     * and creates #a with a reference to #q.
     * 
     * @param query 
     * @param kb
     */
    QueryCtx(const PluginAtom::Query& query, KBManager& kb) throw (DLError);


    /** 
     * Ctor which takes a Query and an Answer.
     * 
     * @param qq 
     * @param aa 
     */
    QueryCtx(Query* qq, Answer* aa);

    /// dtor.
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

    /// managed QueryCtx
    typedef boost::shared_ptr<QueryCtx> shared_pointer;

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
  inline std::ostream&
  operator<< (std::ostream& os, const QueryCtx& q)
  {
    return os << q.getQuery();
  }

  /**
   * @brief lexicographically compare @a q1 to @a q2 and check if @a
   * q1 is less than @a q2.
   *
   * @param q1
   * @param q2
   * @return true if @a q1 < @a q2, false otherwise.
   */
  inline bool
  operator< (const QueryCtx& q1, const QueryCtx& q2)
  {
    return q1.getQuery() < q2.getQuery();
  }


} // namespace dl
} // namespace dlvhex

#endif /* _RACERQUERY_H */
