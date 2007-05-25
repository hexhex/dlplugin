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
 * @file   QueryDirector.h
 * @author Thomas Krennwallner
 * @date   Wed Jul  6 09:49:23 2005
 * 
 * @brief  Director classes for the communication with a dl engine.
 * 
 * 
 */

#ifndef _QUERYDIRECTOR_H
#define _QUERYDIRECTOR_H

#include "QueryCtx.h"
#include "DLError.h"
#include "Cache.h"

#include <iosfwd>

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/shared_ptr.hpp>

namespace dlvhex {
namespace dl {

  /**
   * @brief Base class for the director classes.
   */
  class QueryBaseDirector
  {
  public:
    virtual
    ~QueryBaseDirector()
    { }

    /**
     * query method implemented in the children of QueryBaseDirector.
     *
     * @param qctx
     */
    virtual QueryCtx::shared_pointer
    query(QueryCtx::shared_pointer qctx) throw(DLError) = 0;

    typedef boost::shared_ptr<QueryBaseDirector> shared_pointer; 
  };


  /**
   * @brief Template Director for building DL querying commands and
   * parsing their result.
   */
  template <class Builder, class Parser>
  class QueryDirector : public QueryBaseDirector
  {
  private:
    /// Builder object
    Builder builder;
    /// Parser object
    Parser parser;

  public:
    /**
     * Ctor.
     *
     * @param s use @a s to instantiate Builder and Parser
     */
    explicit
    QueryDirector(std::iostream& s);

    /**
     * @brief builds/parses DL queries/answers using the Builder and
     * Parser members.
     *
     * @param qctx pass the appropriate QueryCtx::shared_pointer
     * member to the Builder/Parser.
     *
     * @return the QueryCtx::shared_pointer with the corresponding
     * Answer to the Query found in @a qctx
     */
    virtual QueryCtx::shared_pointer
    query(QueryCtx::shared_pointer qctx) throw(DLError);
  };



  /**
   * @brief Base class for building queries.
   *
   * @see QueryDirector
   */
  class QueryBaseBuilder
  {
  protected:
    /// the builders write commands to this stream
    std::ostream& stream;

    /**
     * protected ctor.
     *
     * @param os used as output stream
     */
    explicit
    QueryBaseBuilder(std::ostream& os)
      : stream(os)
    { }

  public:
    /// Dtor
    virtual
    ~QueryBaseBuilder()
    { }

    /**
     * Building method implemented by the children of QueryBuilder.
     *
     * @param q contains the information in order to create a query
     *
     * @return true if a command was built, false otherwise.
     */
    virtual bool
    buildCommand(Query& q) throw (DLBuildingError) = 0;
  };


  /**
   * @brief Base class for the parsing answers.
   *
   * @see QueryDirector
   */
  class QueryBaseParser
  {
  protected:
    /// the parser read the answers from this stream
    std::istream& stream;

    /**
     * protected ctor.
     *
     * @param is used as input stream
     */
    explicit
    QueryBaseParser(std::istream& is)
      : stream(is)
    { }

  public:
    /// dtor
    virtual
    ~QueryBaseParser()
    { }

    /**
     * Parsing method implemented by the children of QueryParser.
     *
     * @param answer add answers to a query to @a answer
     */
    virtual void
    parse(Answer& answer) throw (DLParsingError) = 0;
  };


  /**
   * @brief A base composite director for querying lists of
   * QueryBaseDirector objects.
   *
   * Provides a list of QueryBaseDirector and takes care of
   * inconsistency during execution of the query commands.
   */
  class QueryCompositeDirector : public QueryBaseDirector
  {
  private:
    /// holds a list of QueryBaseDirector objects
    boost::ptr_vector<QueryBaseDirector> dirs;

    /// keep a reference to the iostream just in case we need to
    /// create a dedicated director
    std::iostream& stream;

    /**
     * If @a qctx leads to inconsistency, this method is called to
     * handle that case.
     *
     * @param qctx
     *
     * @return the QueryCtx::shared_pointer with the appropriate
     * Answer to the Query found in @a qctx.
     */
    virtual QueryCtx::shared_pointer
    handleInconsistency(QueryCtx::shared_pointer qctx);

  public:
    /// Ctor
    explicit
    QueryCompositeDirector(std::iostream&);

    /**
     * adds a new QueryBaseDirector to dirs.
     *
     * @param d add it to #dirs
     */
    virtual void
    add(QueryBaseDirector* d);

    /**
     * iterates through the list of QueryBaseDirectors and calls their
     * query() method. If the ABox gets inconsistent during the
     * iteration query() calls handleInconsitency() in order to
     * generate the appropriate answer.
     *
     * @param qctx
     *
     * @return the QueryCtx::shared_pointer with the corresponding
     * Answer to the Query
     */
    virtual QueryCtx::shared_pointer
    query(QueryCtx::shared_pointer qctx) throw(DLError);

    typedef boost::shared_ptr<QueryCompositeDirector> shared_pointer;
  };


  /**
   * @brief Provides caching support for the QueryCtx objects.
   */
  class QueryCachingDirector : public QueryBaseDirector
  {
  private:
    /// the underlying director, usually a QueryCompositeDirector
    QueryBaseDirector::shared_pointer director;

    /// reference to the cache of QueryCtx objects
    BaseCache& cache;

  public:
    /** 
     * Ctor.
     * 
     * @param c the cache
     * @param d delegation director
     */
    QueryCachingDirector(BaseCache& c, QueryBaseDirector::shared_pointer d);

    /**
     * Tries to lookup the query @a qctx in its cache and determines
     * BaseCache::cacheHit() if its really a cache-hit. Otherwise it
     * delegates the querying to its underlying @a director and caches
     * the new QueryCtx object.
     *
     * @param qctx
     *
     * @return the QueryCtx::shared_pointer with the corresponding
     * Answer to the Query
     */
    virtual QueryCtx::shared_pointer
    query(QueryCtx::shared_pointer qctx) throw(DLError);
  };


} // namespace dl
} // namespace dlvhex


// include the implemantation of the templates
#include "QueryDirector.tcc"


#endif /* _QUERYDIRECTOR_H */


// Local Variables:
// mode: C++
// End:
