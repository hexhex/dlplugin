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

#include "QueryDirector.h"

#include <iosfwd>

namespace dlvhex {
namespace dl {

  //
  // forward declarations
  //
  class Query;

namespace racer {

  /**
   * @brief Base class for RACER external atoms.
   */
  template <class GetKBManager>
  class RacerExtAtom : public PluginAtom
  {
  protected:
    /// keep a reference to the iostream in order to create the
    /// director instances
    std::iostream& stream;

    /// get a reference to the KBManager
    GetKBManager getKBManager;

    /// protected ctor
    RacerExtAtom(std::iostream&);

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
    getDirectors(const dlvhex::dl::Query&  query) const = 0;


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
  template <class GetKBManager, class GetCache>
  class RacerCachingAtom : public RacerExtAtom<GetKBManager>
  {
  protected:
    /// get a reference to the cache of QueryCtx objects
    GetCache getCache;

    /// @return a caching director for @a comp
    virtual QueryBaseDirector::shared_pointer
    cacheQuery(QueryCompositeDirector::shared_pointer comp) const;

  public:
    RacerCachingAtom(std::iostream&);
  };


  /**
   * @brief Implements the concept retrieving atom
   * &dlC[kb,plusC,minusC,plusR,minusR,query](X).
   */
  template <class GetKBManager, class GetCache>
  class RacerConceptAtom : public RacerCachingAtom<GetKBManager,GetCache>
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
    RacerConceptAtom(std::iostream&);
  };


  /**
   * @brief Implements the role retrieving atom
   * &dlR[kb,plusC,minusC,plusR,minusR,query](X,Y).
   */
  template <class GetKBManager, class GetCache>
  class RacerRoleAtom : public RacerCachingAtom<GetKBManager,GetCache>
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
    RacerRoleAtom(std::iostream&);
  };



  /**
   * @brief Implements the consistency checking atom
   * &dlConsistent[kb,plusC,minusC,plusR,minusR]().
   */
  template <class GetKBManager>
  class RacerConsistentAtom : public RacerExtAtom<GetKBManager>
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
    RacerConsistentAtom(std::iostream&);
  };


  /**
   * @brief Implements the datatype role retrieving atom
   * &dlDR[kb,plusC,minusC,plusR,minusR,query](X,Y).
   */
  template <class GetKBManager, class GetCache>
  class RacerDatatypeRoleAtom : public RacerCachingAtom<GetKBManager,GetCache>
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
    RacerDatatypeRoleAtom(std::iostream&);
  };


  /**
   * @brief Implements the conjunctive query atom
   * &dlCQn[kb,plusC,minusC,plusR,minusR,query](X_1,-,X_n), where n
   * is given at instantiation time.
   */
  template <class GetKBManager, class GetCache>
  class RacerCQAtom : public RacerCachingAtom<GetKBManager,GetCache>
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
    RacerCQAtom(std::iostream&, unsigned n);
  };


  /**
   * @brief Implements the union of conjunctive queries atom
   * &dlUCQn[kb,plusC,minusC,plusR,minusR,query](X_1,-,X_n), where n
   * is given at instantiation time.
   */
  template <class GetKBManager, class GetCache>
  class RacerUCQAtom : public RacerCachingAtom<GetKBManager,GetCache>
  {
  protected:
    /**
     * Create a union of conjunctive queries director chain.
     *
     * @param query
     *
     * @return
     */
    virtual QueryBaseDirector::shared_pointer
    getDirectors(const dlvhex::dl::Query& query) const;

  public:
    RacerUCQAtom(std::iostream&, unsigned n);
  };



} // namespace racer
} // namespace dl
} // namespace dlvhex


// now include the template implementation
#include "RacerExtAtom.tcc"

#endif /* _RACEREXTATOM_H */
