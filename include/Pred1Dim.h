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
 * @file   Pred1Dim.h
 * @author DAO Tran Minh 
 * @date   Tue Dec 18 17:20:24 2007 
 *  
 * @brief  A list of Predicate(s). 
 *  
 *  
 */

#ifndef _DLVHEX_DF_PRED1DIM_H_
#define _DLVHEX_DF_PRED1DIM_H_

#include <list>
#include "Predicate.h"
#include "Pred2Dim.h"
#include "Updates.h"

namespace dlvhex {
namespace df {

  class Pred2Dim;

  class Pred1Dim
  {
  protected:
    std::list<Predicate> pred1dim;
    
  public:
    Pred1Dim() { };
    Pred1Dim(Predicate&);
    Pred1Dim(std::list<Predicate>&);
    
    Pred1Dim&
      operator=(const Pred1Dim&);
    
    Terms
      getAllDistinctTerms();
    
    void
      setContent(std::list<Predicate>&);
    
    void
      push_back(Predicate&);
    
    int
      size();
    
    std::list<Predicate>::iterator
      begin();
    
    std::list<Predicate>::iterator
      end();
    
    Updates
      getUpdates(std::string);

    Pred2Dim
      deMorgan();

    std::string
      behalf_name(std::string prefix = "", int id1 = 0, int id2 = 0);
    
    std::string 
      toString(PredicateMode, bool naf = false);
  };
  
} // namespace df
} // namespace dlvhex

#endif /* _DLVHEX_DF_PRED1DIM_H_*/
