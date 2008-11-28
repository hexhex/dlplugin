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
 * @file   Pred2Dim.h
 * @author DAO Tran Minh 
 * @date   Tue Dec 18 17:20:24 2007 
 *  
 * @brief  A list of Pred1Dim(s), i.e., a 2-dimensional list of Predicate(s).
 *  
 *  
 */

#ifndef _DLVHEX_DF_PRED2DIM_H_
#define _DLVHEX_DF_PRED2DIM_H_

#include "Pred1Dim.h"

namespace dlvhex {
namespace df {

  class Pred1Dim;

  class Pred2Dim
  {
  private:
    std::list<Pred1Dim> pred2dim;
  public:
    Pred2Dim() { };
    Pred2Dim(Predicate&);
    Pred2Dim(Pred1Dim&);
    Pred2Dim(std::list<Pred1Dim>&);

    void
      setContent(std::list<Pred1Dim>&);

    Terms
      getAllDistinctTerms();

    int
      size();

    std::list<Pred1Dim>::iterator
      begin();

    std::list<Pred1Dim>::iterator
      end();

    void
      push_back(Pred1Dim&);

    std::string
      toString(PredicateMode);
  };
  
} // namespace df
} // namespace dlvhex

#endif /* _DLVHEX_DF_PRED2DIM_H_*/
