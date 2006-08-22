/* -*- C++ -*- */

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
    : query(q)
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
