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

#include "RacerInterface.h"
#include "QueryDirector.h"
#include "Cache.h"

#include <iosfwd>

namespace dlvhex {
namespace dl {

  //
  // forward declarations
  //
  class Query;
  class RacerKBManager;

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
    /// the kb-manager
    RacerKBManager& kbManager;

    /// protected ctor
    RacerExtAtom(std::iostream&, RacerKBManager&);

    /// dtor
    virtual
    ~RacerExtAtom()
    { }

    /** 
     * Setup Racer instance.
     * 
     * @param comp add QueryDirector to this composite
     */
    virtual void
    setupRacer(QueryCompositeDirector::shared_pointer& comp) const;

    /** 
     * Add QueryDirectors to @a comp in order to open an ontology.
     * 
     * @param query 
     * @param comp 
     */
    virtual void
    openOntology(const dlvhex::dl::Query& query,
		 QueryCompositeDirector::shared_pointer& comp) const;

    /** 
     * Add QueryDirectors to @a comp in order to increase the ABox.
     * 
     * @param query 
     * @param comp 
     */
    virtual void
    increaseABox(const dlvhex::dl::Query& query,
		 QueryCompositeDirector::shared_pointer& comp) const;

    /**
     * children of RacerExtAtom implement this method to create a
     * director or a chain of directors with help of the composite
     * director classes.
     *
     * @param query
     *
     * @return a QueryBaseDirector::shared_pointer representing the
     * appropriate command to send to the RACER process
     */
    virtual QueryBaseDirector::shared_pointer
    getDirectors(const dlvhex::dl::Query& query) const = 0;

  public:
    /**
     * Retrieve method used in all external atoms.
     *
     * Calls #getDirectors() in order to get an PluginAtom::Answer for
     * PluginAtom::Query.
     *
     * @param query
     * @param answer
     */
    virtual void
    retrieve(const PluginAtom::Query& query,
	     PluginAtom::Answer& answer) throw(PluginError);
  };

  /**
   * @brief Base class for cached external atoms.
   */
  class RacerCachingAtom : public RacerExtAtom
  {
  protected:
    /// reference to the cache of QueryCtx objects
    BaseCache& cache;

    /// @return a caching director for @a comp
    virtual QueryBaseDirector::shared_pointer
    cacheQuery(QueryCompositeDirector::shared_pointer comp) const;

  public:
    RacerCachingAtom(std::iostream&, RacerKBManager&, BaseCache&);
  };


  /**
   * @brief Implements the concept retrieving atom
   * &dlC[kb,plusC,minusC,plusR,minusR,query](X).
   */
  class RacerConceptAtom : public RacerCachingAtom
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
    virtual QueryBaseDirector::shared_pointer
    getDirectors(const dlvhex::dl::Query& query) const;

  public:
    RacerConceptAtom(std::iostream&, RacerKBManager&, BaseCache&);
  };


  /**
   * @brief Implements the role retrieving atom
   * &dlR[kb,plusC,minusC,plusR,minusR,query](X,Y).
   */
  class RacerRoleAtom : public RacerCachingAtom
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
    virtual QueryBaseDirector::shared_pointer
    getDirectors(const dlvhex::dl::Query& query) const;

  public:
    RacerRoleAtom(std::iostream&, RacerKBManager&, BaseCache&);
  };



  /**
   * @brief Implements the consistency checking atom
   * &dlConsistent[kb,plusC,minusC,plusR,minusR]().
   */
  class RacerConsistentAtom : public RacerExtAtom
  {
  protected:
    /**
     * creates a director chain to check the consistency of the ABox.
     *
     * @param query
     *
     * @return
     */
    virtual QueryBaseDirector::shared_pointer
    getDirectors(const dlvhex::dl::Query& query) const;

  public:
    explicit
    RacerConsistentAtom(std::iostream&, RacerKBManager&);
  };


  /**
   * @brief Implements the datatype role retrieving atom
   * &dlDR[kb,plusC,minusC,plusR,minusR,query](X,Y).
   */
  class RacerDatatypeRoleAtom : public RacerCachingAtom
  {
  protected:
    /** 
     * Enable substrate mirroring.
     * 
     * @param comp 
     */
    virtual void
    setupRacer(QueryCompositeDirector::shared_pointer& comp) const;

    /**
     * checks whether the pattern tuple requests a boolean, a
     * retrieval or a individual filler query and creates the director
     * chain accordingly.
     *
     * @param query
     *
     * @return
     */
    virtual QueryBaseDirector::shared_pointer
    getDirectors(const dlvhex::dl::Query& query) const;

  public:
    RacerDatatypeRoleAtom(std::iostream&, RacerKBManager&, BaseCache&);
  };


  /**
   * @brief Implements the conjunctive query atom
   * &dlCQn[kb,plusC,minusC,plusR,minusR,query](X_1,-,X_n), where n
   * is given at instantiation time.
   */
  class RacerCQAtom : public RacerCachingAtom
  {
  protected:
    /**
     * Create a conjunctive query director chain.
     *
     * @param query
     *
     * @return
     */
    virtual QueryBaseDirector::shared_pointer
    getDirectors(const dlvhex::dl::Query& query) const;

  public:
    RacerCQAtom(std::iostream&, RacerKBManager&, BaseCache&, unsigned n);
  };

} // namespace racer
} // namespace dl
} // namespace dlvhex

#endif /* _RACEREXTATOM_H */
