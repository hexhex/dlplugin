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
 * @file   RacerNRQL.tcc
 * @author Thomas Krennwallner
 * @date   Thu Jul 27 16:55:28 2006
 * 
 * @brief  Class hierarchy for nRQL query expressions.
 * 
 */

#ifndef _RACERNRQL_TCC
#define _RACERNRQL_TCC

#include "RacerNRQLBuilder.h"
#include "Query.h"
#include "KBManager.h"

namespace dlvhex {
namespace dl {
namespace racer {


  template <class Builder>
  NRQLQuery<Builder>::NRQLQuery(const Query& q)
    : builder(), query(q)
  { }


  template <class Builder>
  NRQLRetrieve<Builder>::NRQLRetrieve(const Query& q)
    : NRQLQuery<Builder>(q)
  { }


  template <class Builder>
  std::ostream&
  NRQLRetrieve<Builder>::output(std::ostream& s) const
  {
    s << "(retrieve (";

    this->builder.createHead(s, this->query);

    s << ") ";

    this->builder.createBody(s, this->query);
  
    return s << " :abox " << this->query.getKBManager().getKBName() << ')';
  }
  

  template <class Builder>
  NRQLTBoxRetrieve<Builder>::NRQLTBoxRetrieve(const Query& q)
    : NRQLQuery<Builder>(q)
  { }


  template <class Builder>
  std::ostream&
  NRQLTBoxRetrieve<Builder>::output(std::ostream& s) const
  {
    s << "(tbox-retrieve (";

    this->builder.createHead(s, this->query);

    s  << ") ";

    this->builder.createBody(s, this->query);
    
    return s << " :tbox |" << this->query.getDLQuery().getOntology()->getRealURI() << "|)";
  }
  

  template <class Builder>
  NRQLRetrieveUnderPremise<Builder>::NRQLRetrieveUnderPremise(const Query& q)
    : NRQLQuery<Builder>(q)
  { }
  

  template <class Builder>
  std::ostream&
  NRQLRetrieveUnderPremise<Builder>::output(std::ostream& s) const
  {
    s << "(retrieve-under-premise (";

    this->builder.createPremise(s, this->query);

    s << ") (";

    this->builder.createHead(s, this->query);

    s << ") ";

    this->builder.createBody(s, this->query);
    
    return s << " :abox |" << this->query.getDLQuery()->getOntology()->getRealURI() << "|)";
  }

} // namespace racer
} // namespace dl
} // namespace dlvhex

#endif /* _RACERNRQL_TCC */


// Local Variables:
// mode: C++
// End:
