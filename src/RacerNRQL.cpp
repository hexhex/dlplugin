/* -*- C++ -*- */

/**
 * @file   RacerNRQL.cpp
 * @author Thomas Krennwallner
 * @date   Thu Apr 13 12:11:21 2006
 * 
 * @brief  
 * 
 * @todo this is so not documented
 */

#include "RacerNRQL.h"

#include <boost/shared_ptr.hpp>

#include <iostream>
#include <iterator>
#include <vector>

using namespace dlvhex::racer;


namespace dlvhex {
namespace racer {

  std::ostream&
  operator<< (std::ostream& s, const NRQLBase& b)
  {
    return b.output(s);
  }

  
  struct Dereference
  {
    template<typename T>
    T&
    operator() (const boost::shared_ptr<T>& ptr) const
    {
      return *ptr;
    }
  };

  template<typename T>
  std::ostream&
  operator<< (std::ostream& s,
	      const std::vector<boost::shared_ptr<T> >& v)
  {
    if (!v.empty())
      {
	std::transform(v.begin(),
		       v.end() - 1,
		       std::ostream_iterator<T>(s, " "),
		       Dereference()
		       );

	s << *v.back();
      }
    
    return s;
  }
  
}
}



NRQLQueryAtom::NRQLQueryAtom(ABoxQueryAtom::const_pointer a)
  : atom(a)
{ }

std::ostream&
NRQLQueryAtom::output(std::ostream& s) const
{
  return s << "(" << *atom << ")";
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
  list.push_back(NRQLBody::shared_pointer(e));
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
  list.push_back(NRQLBody::shared_pointer(e));
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

void
NRQLRetrieve::addHead(ABoxQueryExpr::const_pointer e)
{
  head.push_back(ABoxQueryExpr::shared_pointer(e));
}

void
NRQLRetrieve::addBody(NRQLBody::const_pointer e)
{
  body.push_back(NRQLBody::shared_pointer(e));
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

void
NRQLTBoxRetrieve::addHead(ABoxQueryExpr::const_pointer e)
{
  head.push_back(ABoxQueryExpr::shared_pointer(e));
}

void
NRQLTBoxRetrieve::addBody(NRQLBody::const_pointer e)
{
  body.push_back(NRQLBody::shared_pointer(e));
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
  premise.push_back(ABoxAssertion::shared_pointer(e));
}

void
NRQLRetrieveUnderPremise::addHead(ABoxQueryExpr::const_pointer e)
{
  head.push_back(ABoxQueryExpr::shared_pointer(e));
}

void
NRQLRetrieveUnderPremise::addBody(NRQLBody::const_pointer e)
{
  body.push_back(NRQLBody::shared_pointer(e));
}
