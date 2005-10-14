/* -*- C++ -*- */

/**
 * @file   RacerExtAtom.h
 * @author Thomas Krennwallner
 * @date   Mon Sep  5 11:07:28 2005
 * 
 * @brief  External atoms for dlvex.
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

#include "RacerQuery.h"
#include "RacerDirector.h"

namespace dlvex_racer {

  /**
   * @brief Base class for RACER external atoms.
   */
  class RacerExtAtom : public PluginAtom
  {
  protected:
    std::iostream& stream;
    RacerCachingDirector::RacerCache& cache;

    RacerExtAtom(std::iostream&, RacerCachingDirector::RacerCache&);

    virtual
    ~RacerExtAtom();

    /**
     * @brief Setup QueryCtx object.
     *
     * @param in
     * @return a new managed QueryCtx pointer
     */
    virtual RacerBaseDirector::QueryCtxPtr
    setupQuery(const FACTSETVECTOR& in) const;

    /// a template based method which creates nested RacerBaseDirector
    /// objects
    template<class Director, class Cacher>
    RacerBaseDirector::DirectorPtr
    getDirectors() const;

    /// template method for retrieve()
    virtual RacerBaseDirector::DirectorPtr
    getRetrievalDirectors() const = 0;

    /// template method for query()
    virtual RacerBaseDirector::DirectorPtr
    getQueryDirectors() const = 0;

  public:
    virtual void
    retrieve(FACTSETVECTOR&, TUPLEVECTOR&) throw(PluginError);

    virtual bool
    query(FACTSETVECTOR&, Tuple&) throw(PluginError);
  };


  /**
   * @brief Concept external atom to send data to RACER.
   */
  class RacerConcept : public RacerExtAtom
  {
  protected:
    virtual RacerBaseDirector::DirectorPtr
    getRetrievalDirectors() const;

    virtual RacerBaseDirector::DirectorPtr
    getQueryDirectors() const;

  public:
    RacerConcept(std::iostream&, RacerCachingDirector::RacerCache&);

    virtual
    ~RacerConcept();
  };


  /**
   * @brief Role external atom to send data to RACER.
   */
  class RacerRole : public RacerExtAtom
  {
  protected:
    virtual RacerBaseDirector::DirectorPtr
    getRetrievalDirectors() const;

    virtual RacerBaseDirector::DirectorPtr
    getQueryDirectors() const;

  public:
    RacerRole(std::iostream&, RacerCachingDirector::RacerCache&);

    virtual
    ~RacerRole();
  };


} // namespace dlvex_racer

#endif /* _RACEREXTATOM_H */
