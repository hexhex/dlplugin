/* -*- C++ -*- */

/**
 * @file   RacerExtAtom.h
 * @author Thomas Krennwallner
 * @date   Mon Sep  5 11:07:28 2005
 * 
 * @brief  External atoms for dlvhex.
 * 
 * 
 */

#ifndef _RACEREXTATOM_H
#define _RACEREXTATOM_H

#include <iostream>
#include <memory>

#include <dlvhex/PluginInterface.h>
#include <dlvhex/Atom.h>
#include <dlvhex/Term.h>
#include <dlvhex/AtomSet.h>

#include "RacerQuery.h"
#include "RacerDirector.h"

namespace dlvhex {
namespace racer {

  /**
   * @brief Base class for RACER external atoms.
   *
   * Starts RACER in a lazy manner, i.e. when it's actually needed.
   */
  class RacerExtAtom : public PluginAtom
  {
  protected:
    /// keep a reference to the iostream in order to create the
    /// director instances
    std::iostream& stream;
    
    /// Ctor
    RacerExtAtom(std::iostream&);

    /// Dtor
    virtual
    ~RacerExtAtom();

    /**
     * Setup a new QueryCtx object and start the RACER process.
     *
     * @param query use query to setup the QueryCtx
     *
     * @return a new managed QueryCtx pointer
     */
    virtual RacerBaseDirector::QueryCtxPtr
    setupQuery(const PluginAtom::Query& query) const;

    /**
     * children of RacerExtAtom implement this method to create a
     * director or a chain of directors with help of the composite
     * director classes.
     *
     * @param query
     *
     * @return a RacerBaseDirector::DirectorPtr representing the
     * appropriate command to send to the RACER process
     */
    virtual RacerBaseDirector::DirectorPtr
    getDirectors(const dlvhex::racer::Query& query) const = 0;

  public:
    /**
     * Base retrieve method for all RACER external atoms.
     *
     * @param query
     * @param answer
     */
    virtual void
    retrieve(const PluginAtom::Query& query,
	     PluginAtom::Answer& answer) throw(PluginError) = 0;
  };

  /**
   * @brief Base class for cached external atoms.
   */
  class RacerCachingAtom : public RacerExtAtom
  {
  protected:
    /// reference to the cache of QueryCtx objects
    RacerCachingDirector::RacerCache& cache;

    /// fill a composite and add a caching director
    RacerBaseDirector::DirectorPtr
    getCachedDirectors(RacerBaseDirector*) const;

  public:
    RacerCachingAtom(std::iostream&, RacerCachingDirector::RacerCache&);

    /**
     * Retrieve method used in all caching external atoms.
     *
     * calls setupQuery() and getCachedDirectors() in order to get an
     * Answer to the Query.
     *
     * @param query
     * @param answer
     */
    virtual void
    retrieve(const PluginAtom::Query& query,
	     PluginAtom::Answer& answer) throw(PluginError);
  };


  /**
   * @brief Implements the concept retrieving atom
   * &dlC[kb,plusC,minusC,plusR,minusR,query](X).
   */
  class RacerConcept : public RacerCachingAtom
  {
  protected:
    /**
     * checks whether the pattern tuple requests a boolean or a
     * retrieval query and creates the director chain accordingly.
     *
     * @param query
     *
     * @return
     */
    virtual RacerBaseDirector::DirectorPtr
    getDirectors(const dlvhex::racer::Query& query) const;

  public:
    RacerConcept(std::iostream&, RacerCachingDirector::RacerCache&);
  };


  /**
   * @brief Implements the role retrieving atom
   * &dlR[kb,plusC,minusC,plusR,minusR,query](X,Y).
   */
  class RacerRole : public RacerCachingAtom
  {
  protected:
    /**
     * checks whether the pattern tuple requests a boolean, a
     * retrieval or a individual filler query and creates the director
     * chain accordingly.
     *
     * @param query
     *
     * @return
     */
    virtual RacerBaseDirector::DirectorPtr
    getDirectors(const dlvhex::racer::Query& query) const;

  public:
    RacerRole(std::iostream&, RacerCachingDirector::RacerCache&);
  };



  /**
   * @brief Implements the consistency checking atom
   * &dlConsistent[kb,plusC,minusC,plusR,minusR]().
   */
  class RacerConsistent : public RacerExtAtom
  {
  protected:
    /**
     * creates a director chain to check the consistency of the ABox.
     *
     * @param query
     *
     * @return
     */
    virtual RacerBaseDirector::DirectorPtr
    getDirectors(const dlvhex::racer::Query& query) const;

  public:
    explicit
    RacerConsistent(std::iostream&);

    /**
     * Retrieve method checks whether ABox is consistent.
     *
     * @param query
     * @param answer
     */
    virtual void
    retrieve(const PluginAtom::Query& query,
	     PluginAtom::Answer& answer) throw(PluginError);
  };


} // namespace racer
} // namespace dlvhex

#endif /* _RACEREXTATOM_H */
