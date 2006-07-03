/* -*- C++ -*- */

/**
 * @file   RacerDirector.h
 * @author Thomas Krennwallner
 * @date   Wed Jul  6 09:49:23 2005
 * 
 * @brief  Director classes for the communication with RACER.
 * 
 * 
 */

#ifndef _RACERDIRECTOR_H
#define _RACERDIRECTOR_H

#include <dlvhex/Atom.h>
#include <dlvhex/Term.h>

#include "RacerError.h"
#include "RacerQuery.h"
#include "RacerAnswerDriver.h"
#include "RacerBuilder.h"
#include "Cache.h"

#include <iosfwd>

namespace dlvhex {
namespace racer {

  /**
   * @brief Base class for the director classes.
   */
  class RacerBaseDirector
  {
  public:
    typedef RacerBaseDirector value_type;
    typedef boost::shared_ptr<value_type> shared_pointer; /// managed
							  /// RacerBaseDirector

  protected:
    RacerBaseDirector()
    { }

  public:
    virtual
    ~RacerBaseDirector()
    { }

    /**
     * query method implemented in the children of RacerBaseDirector.
     *
     * @param qctx
     */
    virtual QueryCtx::shared_pointer
    query(QueryCtx::shared_pointer qctx) throw(RacerError) = 0;
  };


  /**
   * @brief Template Director for building RACER commands and parsing
   * their result.
   */
  template <class Builder, class Parser>
  class RacerDirector : public RacerBaseDirector
  {
  protected:
    /// Builder object
    Builder builder;
    /// Parser object
    Parser parser;

  public:
    /**
     * Ctor.
     *
     * @param s use s to instantiate Builder and Parser
     */
    explicit
    RacerDirector(std::iostream& s);

    /**
     * @brief builds/parses RACER commands/answers using the Builder
     * and Parser members.
     *
     * @param qctx pass the appropriate QueryCtx::shared_pointer
     * member to the Builder/Parser.
     *
     * @return the QueryCtx::shared_pointer with the corresponding
     * Answer to the Query
     */
    virtual QueryCtx::shared_pointer
    query(QueryCtx::shared_pointer qctx) throw(RacerError);
  };



  /**
   * @brief A base composite director for querying lists of RACER
   * commands.
   *
   * Provides a list of RacerBaseDirector and takes care of
   * inconsistency during execution of the RACER commands.
   */
  class RacerCompositeDirector : public RacerBaseDirector
  {
  public:
    typedef RacerCompositeDirector value_type;
    typedef boost::shared_ptr<value_type> shared_pointer;

  protected:
    /// managed RacerBaseDirector pointers
    typedef std::vector<RacerBaseDirector*> DirectorList;

    /// holds a list of RacerBaseDirector objects
    DirectorList dirs;

    /// keep a reference to the iostream just in case we need to
    /// create a dedicated director
    std::iostream& stream;

    /**
     * handleInconsistency() implemented in the children of
     * RacerCompositeDirector.
     *
     * @param qctx
     *
     * @return the QueryCtx::shared_pointer with the corresponding
     * Answer to the Query
     */
    virtual QueryCtx::shared_pointer
    handleInconsistency(QueryCtx::shared_pointer qctx);

  public:
    /// Ctor
    explicit
    RacerCompositeDirector(std::iostream&);

    /**
     * Dtor deletes managed RacerBaseDirector objects.
     */
    virtual
    ~RacerCompositeDirector();

    /**
     * adds a new RacerBaseDirector to dirs.
     *
     * @param d add it to the managed list of RacerBaseDirector
     * objects
     */
    virtual void
    add(RacerBaseDirector* d);

    /**
     * iterates through the list of RacerBaseDirectors and calls their
     * query() method. If RACERs ABox gets inconsistent during the
     * iteration query() calls handleInconsitency() in order to
     * generate the appropriate answer.
     *
     * @param qctx
     *
     * @return the QueryCtx::shared_pointer with the corresponding
     * Answer to the Query
     */
    virtual QueryCtx::shared_pointer
    query(QueryCtx::shared_pointer qctx) throw(RacerError);
  };


  /**
   * @brief Provides caching support for RACER Queries.
   */
  class RacerCachingDirector : public RacerBaseDirector
  {
  protected:
    /// the underlying director, usually a RacerCompositeDirector
    RacerBaseDirector::shared_pointer director;

    /// reference to the cache of QueryCtx objects
    BaseCache& cache;

  public:
    explicit
    RacerCachingDirector(BaseCache&, RacerBaseDirector::shared_pointer d);

    virtual
    ~RacerCachingDirector();

    /**
     * Tries to lookup the query in its cache and determines
     * BaseCache::cacheHit() if its really a cache-hit. Otherwise it
     * delegates the querying to its underlying director and caches
     * its QueryCtx object.
     *
     * @param qctx
     *
     * @return the QueryCtx::shared_pointer with the corresponding
     * Answer to the Query
     */
    virtual QueryCtx::shared_pointer
    query(QueryCtx::shared_pointer qctx) throw(RacerError);
  };


  // following typedefs are here for easy of use

  /// builds nothing and parses nothing. Mainly for testing purposes.
  typedef RacerDirector<RacerNullBuilder, RacerNullParser> RacerNullDirector;
  /// requests all individuals and parses the corresponding answer
  typedef RacerDirector<RacerAllIndividualsBuilder, RacerAnswerDriver> RacerAllIndQuery;
  /// turn on unique name assumption
  typedef RacerDirector<RacerUNABuilder, RacerIgnoreAnswer> RacerUNA;
  /// create a temporary ABox called DEFAULT
  typedef RacerDirector<RacerTempABoxBuilder, RacerAnswerDriver> RacerTempABox;
  /// request to open an OWL document
  typedef RacerDirector<RacerOpenOWLBuilder, RacerAnswerDriver> RacerOpenOWL;
  /// ask whether ABox is consistent
  typedef RacerDirector<RacerABoxConsistentBuilder, RacerAnswerDriver> RacerABoxConsistent;
  ///
  typedef RacerDirector<RacerCheckABoxConsistencyOffBuilder, RacerIgnoreAnswer> RacerABoxConsistencyOff;

  /// extend ABox by a given set of individuals/pairs
  typedef RacerDirector<RacerStateBuilder, RacerAnswerDriver> RacerConceptRolePM;
  /// request a list of individuals from a specified concept
  typedef RacerDirector<RacerConceptInstancesBuilder, RacerAnswerDriver> RacerConceptQuery;
  /// request a list of pairs from a specified role
  typedef RacerDirector<RacerRoleIndividualsBuilder, RacerAnswerDriver> RacerRoleQuery;
  /// ask whether a given individual is member of a specified concept
  typedef RacerDirector<RacerIsConceptMemberBuilder, RacerAnswerDriver> RacerIsConceptQuery;
  /// ask whether a given pair is member of a specified role
  typedef RacerDirector<RacerIsRoleMemberBuilder, RacerAnswerDriver> RacerIsRoleQuery;
  /// request a list of individuals which are fillers of a role for a
  /// specified individual
  typedef RacerDirector<RacerIndividualFillersBuilder, RacerAnswerDriver> RacerIndvFillersQuery;
  /// request a list of datatype individuals which are fillers of a
  /// role for a specified individual
  typedef RacerDirector<RacerIndividualDatatypeFillersBuilder,RacerAnswerDriver> RacerIndvDataFillersQuery;

} // namespace racer
} // namespace dlvhex


// include the implemantation of the templates
#include "RacerDirector.tcc"


#endif /* _RACERDIRECTOR_H */
