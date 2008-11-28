/* dlvhex-dlplugin -- Integration of Answer-Set Programming and Description Logics. 
 * 
 * Copyright (C) 2007, 2008 DAO Tran Minh 
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
 * @file   Pred1Dim.cpp 
 * @author DAO Tran Minh 
 * @date   Tue Dec 18 17:20:24 2007 
 *  
 * @brief  A list of Predicate(s). 
 *  
 *  
 */ 

#include <iostream>
#include <sstream>
#include "Pred1Dim.h"
#include "Pred2Dim.h"

namespace dlvhex {
namespace df {

  Pred1Dim::Pred1Dim(Predicate& p)
  {
    pred1dim.push_back(p);
  }

  Pred1Dim::Pred1Dim(std::list<Predicate>& pred1dim_)
  {
    pred1dim.clear();
    for (std::list<Predicate>::iterator i = pred1dim_.begin(); i != pred1dim_.end(); ++i)
      {
	Predicate p = *i;
	pred1dim.push_back(p);
      }
  }

  Pred1Dim&
  Pred1Dim::operator=(const Pred1Dim& p2)
  {
    if (this != &p2)
      {
	pred1dim.clear();
	for (std::list<Predicate>::const_iterator i = p2.pred1dim.begin(); i != p2.pred1dim.end(); ++i)
	  {
	    Predicate p = *i;
	    pred1dim.push_back(p);
	  }
      }
    return *this;
  }
  
  void 
  Pred1Dim::setContent(std::list<Predicate>& pred1dim_)
  {
    pred1dim.clear();
    for (std::list<Predicate>::iterator i = pred1dim_.begin(); i != pred1dim_.end(); ++i)
      {
	Predicate p = *i;
	pred1dim.push_back(p);
      }
  }

  Terms
  Pred1Dim::getAllDistinctTerms()
  {
    Terms ts;

    for (std::list<Predicate>::iterator i = pred1dim.begin(); i != pred1dim.end(); ++i)
      {
	Terms t1 = i->getTerms();
	ts.insertNewTerms(t1);
      }
    
    return ts;
  }

  void
  Pred1Dim::push_back(Predicate& p)
  {
    pred1dim.push_back(p);
  }

  int
  Pred1Dim::size()
  {
    return pred1dim.size();
  }

  std::list<Predicate>::iterator
  Pred1Dim::begin()
  {
    return pred1dim.begin();
  }

  std::list<Predicate>::iterator
  Pred1Dim::end()
  {
    return pred1dim.end();
  }

  Updates
  Pred1Dim::getUpdates(std::string prefix)
  {
    Operator op;
    Updates us;

    for (std::list<Predicate>::iterator i = pred1dim.begin(); i != pred1dim.end(); ++i)
      {
	if (i->isStronglyNegated())
	  {
	    op = UMINUS;
	  }
	else
	  {
	    op = UPLUS;
	  }
	Update u(i->getPredicateNameWithNS(), op, prefix + i->getSignedPredicateName());
	us.push_back(u);
      }
    return us;
  }

  Pred2Dim
  Pred1Dim::deMorgan()
  {
    Pred2Dim dm;
    
    for (std::list<Predicate>::iterator i = pred1dim.begin(); i != pred1dim.end(); ++i)
      {
	Predicate p(!i->isStronglyNegated(), i->getPredicateName(), i->getPrefix(), i->getTerms());
	Pred1Dim ps(p);
	dm.push_back(ps);
      }

    return dm;
  }

  std::string
  Pred1Dim::behalf_name(std::string prefix, int id1, int id2)
  {
    std::string tmp = "";

    if (pred1dim.size() == 1)
      {
	tmp = prefix + pred1dim.begin()->getSignedPredicateName();
      }
    else
      {
	if (id1 > 0)
	  {
	    std::stringstream sid1;
	    sid1 << id1;
	    tmp = "all_" + prefix + sid1.str();
	    if (id2 > 0)
	      {
		std::stringstream sid2;
		sid2 << id2;
		tmp = tmp + "_" + sid2.str();
	      }
	  }
      }

    return tmp;
  }

  std::string 
  Pred1Dim::toString(PredicateMode m, bool naf)
  {
    std::string tmp = "";
    std::string s = "";
    std::string t;
    if (m == mRule && naf) s = " not ";
    switch (m)
      {
      case mRule:
      case mQuery:
	t = ", ";
	break;
      case mDefault:
	t = " & ";
      }

    for (std::list<Predicate>::iterator i = pred1dim.begin(); i != pred1dim.end(); ++i)
      {
	tmp = tmp + s + i->toString() + t;
      }
    return tmp.substr(0, tmp.length() - t.length());
  }
  
} // namespace df
} // namespace dlvhex
