/* dlvhex-dlplugin -- Integration of Answer-Set Programming and Description Logics.
 *
 * Copyright (C) 2005, 2006, 2007  Thomas Krennwallner
 * 
 * This file is part of dlvhex-dlplugin.
 *
 * dlvhex-dlplugin is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * dlvhex-dlplugin is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dlvhex-dlplugin; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


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

#include "DLError.h"

#include <iosfwd>

namespace dlvhex {
namespace dl {

  //
  // forward declarations
  //
  class Query;

namespace racer {

  /**
   * @brief Base class for all the NRQL Builders.
   */
  class NRQLBaseBuilder
  {
  protected:
    /// protected ctor
    NRQLBaseBuilder()
    { }

  public:
    virtual
    ~NRQLBaseBuilder()
    { }

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
      throw(DLBuildingError);

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
      throw(DLBuildingError);

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
      throw(DLBuildingError);
  };


  /**
   * @brief Base class for all the NRQL Builders.
   */
  class NRQLStateBuilder : public NRQLBaseBuilder
  {
  public:
    NRQLStateBuilder () : NRQLBaseBuilder()
    { }

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
      throw(DLBuildingError);
  };


  /**
   * @brief Builds conjunctive nRQL queries.
   */
  class NRQLConjunctionBuilder : public NRQLBaseBuilder
  {
  public:
    NRQLConjunctionBuilder () : NRQLBaseBuilder()
    { }

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
      throw(DLBuildingError);
  };



  /**
   * @brief Builds union of conjunctive queries nRQL queries.
   */
  class NRQLDisjunctionBuilder : public NRQLBaseBuilder
  {
  public:
    NRQLDisjunctionBuilder () : NRQLBaseBuilder()
    { }

    /** 
     * Uses @a query to build a union of conjunctive queries expression.
     * 
     * @param stream output the union of conjunctive queries to this stream
     * @param query use this query
     * 
     * @return true if method created an output, false otherwise.
     */
    virtual bool
    createBody(std::ostream& stream, const Query& query) const
      throw(DLBuildingError);
  };



  /**
   * @brief Builds datatype role queries.
   */
  class NRQLDatatypeBuilder : public NRQLBaseBuilder
  {
  public:
    NRQLDatatypeBuilder () : NRQLBaseBuilder()
    { }

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
      throw(DLBuildingError);

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
      throw(DLBuildingError);
  };

} // namespace racer
} // namespace dl
} // namespace dlvhex

#endif /* _RACERNRQLBUILDER_H */


// Local Variables:
// mode: C++
// End:
