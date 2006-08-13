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

    /// protected ctor
    explicit
    RacerExtAtom(std::iostream&);

    /// dtor
    virtual
    ~RacerExtAtom();

    /** 
     * 
     * 
     * @param query 
     * 
     * @return 
     */
    virtual QueryCompositeDirector::shared_pointer
    getComposite(const dlvhex::dl::Query& query) const;

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

    /// fill a composite and add a caching director
    QueryBaseDirector::shared_pointer
    getCachedDirectors(const dlvhex::dl::Query&, QueryBaseDirector*) const;

  public:
    RacerCachingAtom(std::iostream&, BaseCache&);
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
    RacerConceptAtom(std::iostream&, BaseCache&);
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
    RacerRoleAtom(std::iostream&, BaseCache&);
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
    RacerConsistentAtom(std::iostream&);
  };


  /**
   * @brief Implements the datatype role retrieving atom
   * &dlDR[kb,plusC,minusC,plusR,minusR,query](X,Y).
   */
  class RacerDatatypeRoleAtom : public RacerCachingAtom
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
    RacerDatatypeRoleAtom(std::iostream&, BaseCache&);
  };


  /**
   * @brief Implements the conjunctive query atom
   * &dlCQn[kb,plusC,minusC,plusR,minusR,query](X_1,...,X_n), where n
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
    RacerCQAtom(std::iostream&, BaseCache&, unsigned n);
  };

} // namespace racer
} // namespace dl
} // namespace dlvhex

#endif /* _RACEREXTATOM_H */
