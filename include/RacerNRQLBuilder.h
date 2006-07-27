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

  class NRQLBuilder
  {
  public:
    virtual
    ~NRQLBuilder();

    virtual bool
    createBody(std::ostream& stream, const Query& query) const
      throw(RacerBuildingError);

    virtual bool
    createHead(std::ostream& stream, const Query& query) const
      throw(RacerBuildingError);

    virtual bool
    createPremise(std::ostream& stream, const Query& query) const
      throw(RacerBuildingError);
  };


  class NRQLConjunctionBuilder : public NRQLBuilder
  {
  public:
    virtual bool
    createBody(std::ostream& stream, const Query& query) const
      throw(RacerBuildingError);
  };


  class NRQLDatatypeBuilder : public NRQLBuilder
  {
  public:
    virtual bool
    createHead(std::ostream& stream, const Query& query) const
      throw(RacerBuildingError);

    virtual bool
    createBody(std::ostream& stream, const Query& query) const
      throw(RacerBuildingError);
  };

} // namespace racer
} // namespace dlvhex

#endif /* _RACERNRQLBUILDER_H */
