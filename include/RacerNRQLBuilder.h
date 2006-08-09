/* -*- C++ -*- */

/**
 * @file   RacerNRQLBuilder.h
 * @author Thomas Krennwallner
 * @date   Thu Jul 27 13:22:18 2006
 * 
 * @brief  Various builders for nRQL requests.
 * 
 * 
 */


#ifndef _RACERNRQLBUILDER_H
#define _RACERNRQLBUILDER_H

#include "RacerError.h"

#include <iosfwd>

namespace dlvhex {
namespace racer {

  //
  // forward declarations
  //
  class Query;


  /**
   * @brief Base class for all the NRQL Builders.
   */
  class NRQLBuilder
  {
  public:
    virtual
    ~NRQLBuilder();

    /** 
     * Base class method does nothing.
     *
     * @param stream 
     * @param query 
     * 
     * @return false
     */
    virtual bool
    createBody(std::ostream& stream, const Query& query) const
      throw(RacerBuildingError);

    /** 
     * Uses the output list of @a query and put a nRQL head into @a stream.
     * 
     * @param stream output the head to this stream
     * @param query use this Query
     * 
     * @return true, if method created an output, false otherwise.
     */
    virtual bool
    createHead(std::ostream& stream, const Query& query) const
      throw(RacerBuildingError);

    /** 
     * Uses interpretation of @a query and put a list of ABox
     * assertions into @a stream.
     * 
     * @param stream output the ABox assertions to this stream
     * @param query use this query
     * 
     * @return true if method created an output, false otherwise.
     */
    virtual bool
    createPremise(std::ostream& stream, const Query& query) const
      throw(RacerBuildingError);
  };


  /**
   * @brief Builds conjunctive nRQL queries.
   */
  class NRQLConjunctionBuilder : public NRQLBuilder
  {
  public:
    /** 
     * Uses @a query to build a conjunctive query expression.
     * 
     * @param stream output the conjunctive query to this stream
     * @param query use this query
     * 
     * @return true if method created an output, false otherwise.
     */
    virtual bool
    createBody(std::ostream& stream, const Query& query) const
      throw(RacerBuildingError);
  };


  /**
   * @brief Builds datatype role queries.
   */
  class NRQLDatatypeBuilder : public NRQLBuilder
  {
  public:
    /** 
     * Uses @a query to build the head expression for a datatype role
     * query.
     * 
     * @param stream output the head expression to this stream
     * @param query  use this query
     * 
     * @return true
     */
    virtual bool
    createHead(std::ostream& stream, const Query& query) const
      throw(RacerBuildingError);

    /** 
     * Uses @a query to build a datatype role query.
     * 
     * @param stream output the datatype role query to this stream
     * @param query  use this query
     * 
     * @return true
     */
    virtual bool
    createBody(std::ostream& stream, const Query& query) const
      throw(RacerBuildingError);
  };

} // namespace racer
} // namespace dlvhex

#endif /* _RACERNRQLBUILDER_H */
