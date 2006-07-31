/* -*- C++ -*- */

/**
 * @file   RacerNRQLBuilder.h
 * @author Thomas Krennwallner
 * @date   Thu Jul 27 13:22:18 2006
 * 
 * @brief  
 * 
 * 
 */


#ifndef _RACERNRQLBUILDER_H
#define _RACERNRQLBUILDER_H

#include "RacerNRQL.h"
#include "RacerQuery.h"
#include "RacerError.h"

#include <iosfwd>

namespace dlvhex {
namespace racer {

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
     * Uses query's patterntuple to create a nRQL head.
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
     * Uses query's interpretation and create a list of ABox
     * assertions.
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
     * Uses query to build a conjunctive query expression.
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
     * Uses query to build the head expresision for a datatype role
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
     * Uses query to build a datatype role query.
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
