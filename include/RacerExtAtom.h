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
    RacerCachingDirector::RacerCache& cache;

    RacerExtAtom(std::iostream&, RacerCachingDirector::RacerCache&);

    virtual
    ~RacerExtAtom();

    /**
     * @brief Setup QueryCtx object.
     *
     * @param in
     * @param parms
     * @param indv
     * @return a new managed QueryCtx pointer
     */
    virtual RacerBaseDirector::QueryCtxPtr
    setupQuery(const Interpretation& in,
	       const Tuple& parms,
	       const Tuple& indv) const;

    /// a template based method which fills the composite
    template<class Director, class Cacher>
    RacerBaseDirector::DirectorPtr
    getDirectors(RacerCompositeDirector*) const;

    /// template method for retrieve()
    virtual RacerBaseDirector::DirectorPtr
    getRetrievalDirectors() const = 0;

    /// template method for query()
    virtual RacerBaseDirector::DirectorPtr
    getQueryDirectors() const = 0;

  public:
    virtual void
    retrieve(const Interpretation& in,
	     const Tuple& parms,
	     std::vector<Tuple>& out) throw(PluginError);

    virtual bool
    query(const Interpretation& in,
	  const Tuple& parms,
	  Tuple& indv) throw(PluginError);
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
  };



  /**
   * @brief Consitency external atom for RACER.
   */
  class RacerConsistent : public PluginAtom
  {
  private:
    std::iostream& stream;

  protected:
    /**
     * @brief Setup QueryCtx object.
     *
     * @param in
     * @param parms
     * @return a new managed QueryCtx pointer
     */
    virtual RacerBaseDirector::QueryCtxPtr
    setupQuery(const Interpretation& in,
	       const Tuple& parms) const;

    /// creates nested RacerBaseDirector objects
    RacerBaseDirector::DirectorPtr
    getDirectors() const;

  public:
    explicit
    RacerConsistent(std::iostream&);

    virtual void
    retrieve(const Interpretation& in,
	     const Tuple& parms,
	     std::vector<Tuple>& out) throw(PluginError);

    virtual bool
    query(const Interpretation& in,
	  const Tuple& parms,
	  Tuple& indv) throw(PluginError);
  };


} // namespace racer
} // namespace dlvhex

#endif /* _RACEREXTATOM_H */
