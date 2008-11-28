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
 * @file   Pred2Dim.cpp
 * @author DAO Tran Minh 
 * @date   Tue Dec 18 17:20:24 2007 
 *  
 * @brief  A list of Pred1Dim(s), i.e., a 2-dimensional list of Predicate(s).
 *  
 *  
 */ 

#include "Pred2Dim.h"

namespace dlvhex {
namespace df {

  Pred2Dim::Pred2Dim(Predicate& p)
  {
    Pred1Dim ps(p);
    pred2dim.push_back(ps);
  }

  Pred2Dim::Pred2Dim(Pred1Dim& ps)
  {
    pred2dim.push_back(ps);
  }

  Pred2Dim::Pred2Dim(std::list<Pred1Dim>& pred2dim_)
  {
    pred2dim.clear();
    for (std::list<Pred1Dim>::iterator i = pred2dim_.begin(); i != pred2dim_.end(); ++i)
      {
	Pred1Dim ps = *i;
	pred2dim.push_back(ps);
      }
  }

  void
  Pred2Dim::setContent(std::list<Pred1Dim>& pred2dim_)
  {
    pred2dim.clear();
    for (std::list<Pred1Dim>::iterator i = pred2dim_.begin(); i != pred2dim_.end(); ++i)
      {
	Pred1Dim ps = *i;
	pred2dim.push_back(ps);
      }
  }

  Terms
  Pred2Dim::getAllDistinctTerms()
  {
    Terms ts;

    for (std::list<Pred1Dim>::iterator i = pred2dim.begin(); i != pred2dim.end(); ++i)
      {
	Terms t1 = i->getAllDistinctTerms();
	ts.insertNewTerms(t1);
      }
    return ts;
  }

  int
  Pred2Dim::size()
  {
    return pred2dim.size();
  }

  std::list<Pred1Dim>::iterator
  Pred2Dim::begin()
  {
    return pred2dim.begin();
  }

  std::list<Pred1Dim>::iterator
  Pred2Dim::end()
  {
    return pred2dim.end();
  }

  void
  Pred2Dim::push_back(Pred1Dim& ps)
  {
    pred2dim.push_back(ps);
  }

  std::string
  Pred2Dim::toString(PredicateMode m)
  {
    std::string tmp = "";
    std::string t;
    switch (m)
      {
      case mRule:
      case mDefault:
	t = ", ";
	break;
      case mQuery:
	t = " v ";
      }

    for (std::list<Pred1Dim>::iterator i = pred2dim.begin(); i != pred2dim.end(); ++i)
      {
	tmp = tmp + i->toString(m) + t;
      }
    return tmp.substr(0, tmp.length() - t.length());
  }

} // namespace df
} // namespace dlvhex
