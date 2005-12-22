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
#include <dlvhex/Interpretation.h>

#include "RacerQuery.h"
#include "RacerDirector.h"

namespace dlvhex {
namespace racer {

  /**
   * @brief Base class for RACER external atoms.
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
     * @brief Setup a new QueryCtx object.
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
    getDirectors(const PluginAtom::Query& query) const = 0;

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

    /// a template based method which fills the composite
    template<class Director, class Cacher>
    RacerBaseDirector::DirectorPtr
    getCachedDirectors(RacerCompositeDirector*) const;

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
   * @brief Concept external atom to send data to RACER.
   */
  class RacerConcept : public RacerCachingAtom
  {
  protected:
    /**
     *
     */
    virtual RacerBaseDirector::DirectorPtr
    getDirectors(const PluginAtom::Query& query) const;

  public:
    /**
     *
     */
    RacerConcept(std::iostream&, RacerCachingDirector::RacerCache&);
  };


  /**
   * @brief Role external atom to send data to RACER.
   */
  class RacerRole : public RacerCachingAtom
  {
  protected:
    /**
     *
     */
    virtual RacerBaseDirector::DirectorPtr
    getDirectors(const PluginAtom::Query& query) const;

  public:
    /**
     *
     */
    RacerRole(std::iostream&, RacerCachingDirector::RacerCache&);
  };



  /**
   * @brief Consistency external atom for RACER.
   */
  class RacerConsistent : public RacerExtAtom
  {
  protected:
    /// creates nested RacerBaseDirector objects
    virtual RacerBaseDirector::DirectorPtr
    getDirectors(const PluginAtom::Query&) const;

  public:
    /**
     *
     */
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
