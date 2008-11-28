/* dlvhex-dlplugin -- Integration of Answer-Set Programming and Description Logics
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
 * @file   Terms.cpp 
 * @author DAO Tran Minh 
 * @date   Tue Dec 18 17:20:24 2007 
 *  
 * @brief  A set of Terms. 
 *  
 *  
 */ 
 
#include "Terms.h" 
 
namespace dlvhex { 
namespace df { 
 
  Terms::Terms()
  { }
  
  Terms::Terms(std::list<MTerm>& terms_)
  {
    terms.clear();
    for (std::list<MTerm>::iterator i = terms_.begin(); i != terms_.end(); ++i)
      {
	MTerm t = *i;
	terms.push_back(t);
      }  
  } 

  bool  
  Terms::isEmpty()  
  { 
    return (terms.size() == 0); 
  }

  bool
  Terms::gotThisTerm(MTerm& term_)
  {
    return (find(terms.begin(), terms.end(), term_) != terms.end());
  }

  std::list<MTerm>::iterator
  Terms::begin()
  {
    return terms.begin();
  }

  std::list<MTerm>::iterator
  Terms::end()
  {
    return terms.end();
  }
  
  void 
  Terms::push_back(MTerm term_)  
  { 
    terms.push_back(term_); 
  }
  
  void
  Terms::insertNewTerms(Terms& t1)
  {
    for (std::list<MTerm>::iterator i = t1.terms.begin(); i != t1.terms.end(); ++i)
      {
	if (find(terms.begin(), terms.end(), *i) == terms.end())
	  {
	    terms.push_back(*i);
	  }
      }
  }
  
  std::string
  Terms::toString()  
  { 
    std::string tmp = ""; 
    
    if (terms.size() > 0)  
      { 
	std::list<MTerm>::iterator pos = terms.begin(); 
	tmp = terms.begin()->toString(); 
	pos = terms.begin(); 
	++pos; 
	for (; pos != terms.end(); pos++)  
	  { 
	    tmp += ", " + pos->toString(); 
	} 
      } 
    return tmp; 
  } 
  
} // namespace df 
} // namespace dlvhex
