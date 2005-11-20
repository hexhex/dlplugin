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
    std::iostream& stream;

    RacerExtAtom(std::iostream&);

    virtual
    ~RacerExtAtom();

    /**
     * @brief Setup QueryCtx object.
     *
     * @param query
     * @return a new managed QueryCtx pointer
     */
    virtual RacerBaseDirector::QueryCtxPtr
    setupQuery(const PluginAtom::Query& query) const;

    /// template method used in retrieve()
    virtual RacerBaseDirector::DirectorPtr
    getDirectors(const PluginAtom::Query& query) const = 0;

  public:
    virtual void
    retrieve(const PluginAtom::Query& query,
	     Answer& answer) throw(PluginError) = 0;
  };

  /**
   * @brief Base class for cached external atoms.
   */
  class RacerCachingAtom : public RacerExtAtom
  {
  protected:
    RacerCachingDirector::RacerCache& cache;

    /// a template based method which fills the composite
    template<class Director, class Cacher>
    RacerBaseDirector::DirectorPtr
    getCachedDirectors(RacerCompositeDirector*) const;

  public:
    RacerCachingAtom(std::iostream&, RacerCachingDirector::RacerCache&);

    virtual void
    retrieve(const PluginAtom::Query& query,
	     Answer& answer) throw(PluginError);
  };


  /**
   * @brief Concept external atom to send data to RACER.
   */
  class RacerConcept : public RacerCachingAtom
  {
  protected:
    virtual RacerBaseDirector::DirectorPtr
    getDirectors(const PluginAtom::Query& query) const;

  public:
    RacerConcept(std::iostream&, RacerCachingDirector::RacerCache&);
  };


  /**
   * @brief Role external atom to send data to RACER.
   */
  class RacerRole : public RacerCachingAtom
  {
  protected:
    virtual RacerBaseDirector::DirectorPtr
    getDirectors(const PluginAtom::Query& query) const;

  public:
    RacerRole(std::iostream&, RacerCachingDirector::RacerCache&);
  };



  /**
   * @brief Consitency external atom for RACER.
   */
  class RacerConsistent : public RacerExtAtom
  {
  protected:
    /// creates nested RacerBaseDirector objects
    virtual RacerBaseDirector::DirectorPtr
    getDirectors(const PluginAtom::Query&) const;

  public:
    explicit
    RacerConsistent(std::iostream&);

    virtual void
    retrieve(const PluginAtom::Query& query,
	     Answer& answer) throw(PluginError);
  };


} // namespace racer
} // namespace dlvhex

#endif /* _RACEREXTATOM_H */
