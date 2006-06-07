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

#include <iosfwd>
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


NRQLRetrieve::NRQLRetrieve(const std::string& abox)
  : NRQLQuery(abox, "")
{ }

std::ostream&
NRQLRetrieve::output(std::ostream& s) const
{
  s << "(retrieve (" 
    << head
    << ") "
    << body;
  
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
    << body;

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
    << body;

  NRQLQuery::output(s);

  return s << ')';
}

void
NRQLRetrieveUnderPremise::addPremise(ABoxAssertion::const_pointer e)
{
  ABoxAssertion::shared_pointer sp(e);
  premise.push_back(sp);
}
