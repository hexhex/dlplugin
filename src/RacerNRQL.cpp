/* -*- C++ -*- */

/**
 * @file   RacerNRQL.cpp
 * @author Thomas Krennwallner
 * @date   Thu Apr 13 12:11:21 2006
 * 
 * @brief  Class hierarchy for nRQL query expressions.
 * 
 */

#include "RacerNRQL.h"

#include <boost/shared_ptr.hpp>

#include <iostream>
#include <iterator>
#include <vector>

using namespace dlvhex::racer;


namespace dlvhex {
namespace racer {

  /** 
   * Calls b.output().
   * 
   * @param s 
   * @param b 
   * 
   * @return the stream result of b.output().
   */
  std::ostream&
  operator<< (std::ostream& s, const NRQLBase& b)
  {
    return b.output(s);
  }
  
}
}



NRQLQueryAtom::NRQLQueryAtom(ABoxQueryAtom::const_pointer a)
  : atom(a)
{ }

std::ostream&
NRQLQueryAtom::output(std::ostream& s) const
{
  return s << *atom;
}



std::ostream&
NRQLConjunction::output(std::ostream& s) const
{
  return s << "(and "
 	   << list
	   << ")";
}

void
NRQLConjunction::addAtom(NRQLBody::const_pointer e)
{
  NRQLBody::shared_pointer sp(e);
  list.push_back(sp);
}


std::ostream&
NRQLUnion::output(std::ostream& s) const
{
  return s << "(union "
	   << list
	   << ")";
}

void
NRQLUnion::addAtom(NRQLBody::const_pointer e)
{
  NRQLBody::shared_pointer sp(e);
  list.push_back(sp);
}




void
NRQLQuery::addHead(ABoxQueryObject::const_pointer e)
{
  ABoxQueryObject::shared_pointer sp(e);
  head.push_back(sp);
}

void
NRQLQuery::addBody(NRQLBody::const_pointer e)
{
  NRQLBody::shared_pointer sp(e);
  body.push_back(sp);
}



std::ostream&
NRQLRetrieve::output(std::ostream& s) const
{
  return s << "(retrieve (" 
	   << head
	   << ") "
	   << body
	   << ")";
}



std::ostream&
NRQLTBoxRetrieve::output(std::ostream& s) const
{
  return s << "(tbox-retrieve ("
	   << head
	   << ") "
	   << body
	   << ")";
}


std::ostream&
NRQLRetrieveUnderPremise::output(std::ostream& s) const
{
  return s << "(retrieve-under-premise ("
	   << premise
 	   << ") ("
	   << head
 	   << ") "
	   << body
	   << ")";
}

void
NRQLRetrieveUnderPremise::addPremise(ABoxAssertion::const_pointer e)
{
  ABoxAssertion::shared_pointer sp(e);
  premise.push_back(sp);
}
