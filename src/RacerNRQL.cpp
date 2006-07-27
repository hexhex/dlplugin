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
