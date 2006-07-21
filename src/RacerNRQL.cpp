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

#include <iosfwd>

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

using namespace dlvhex::racer;


namespace dlvhex {
namespace racer {

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
  list.push_back(const_cast<NRQLBody*>(e));
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
  list.push_back(const_cast<NRQLBody*>(e));
}



NRQLQuery::NRQLQuery(const std::string& a, const std::string &t)
  : abox(a), tbox(t)
{ }

std::ostream&
NRQLQuery::output(std::ostream& s) const
{
  if (!abox.empty())
    {
      s << " :abox " << abox;
    }

  if (!tbox.empty())
    {
      s << " :tbox " << tbox;
    }

  return s;
}

void
NRQLQuery::setHead(boost::shared_ptr<boost::ptr_vector<ABoxQueryObject> > sp)
{
  // move ownership of the ABoxQueryObject list to head
  head = sp->release();
}

void
NRQLQuery::addHead(ABoxQueryObject::const_pointer e)
{
  head.push_back(const_cast<ABoxQueryObject*>(e));
}

void
NRQLQuery::setBody(NRQLBody::shared_pointer sp)
{
  body = sp;
}


NRQLRetrieve::NRQLRetrieve(const std::string& abox)
  : NRQLQuery(abox, "")
{ }

std::ostream&
NRQLRetrieve::output(std::ostream& s) const
{
  s << "(retrieve (" 
    << head
    << ") "
    << *body;
  
  NRQLQuery::output(s);

  return s << ')';
}



NRQLTBoxRetrieve::NRQLTBoxRetrieve(const std::string& tbox)
  : NRQLQuery("", tbox)
{ }

std::ostream&
NRQLTBoxRetrieve::output(std::ostream& s) const
{
  s << "(tbox-retrieve ("
    << head
    << ") "
    << *body;

  NRQLQuery::output(s);

  return s << ')';
}


NRQLRetrieveUnderPremise::NRQLRetrieveUnderPremise(const std::string& abox)
  : NRQLQuery(abox, "")
{ }

std::ostream&
NRQLRetrieveUnderPremise::output(std::ostream& s) const
{
  s << "(retrieve-under-premise ("
    << premise
    << ") ("
    << head
    << ") "
    << *body;

  NRQLQuery::output(s);

  return s << ')';
}

void
NRQLRetrieveUnderPremise::addPremise(ABoxAssertion::const_pointer e)
{
  premise.push_back(const_cast<ABoxAssertion*>(e));
}
