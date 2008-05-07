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
 * @file   DLAtom.h 
 * @author DAO Tran Minh 
 * @date   Tue Dec 18 17:20:24 2007 
 *  
 * @brief  A DLAtom of the form DL[\lambda;\query](Terms). 
 *  
 *  
 */ 
 
#ifndef _DLVHEX_DF_DLATOM_H 
#define _DLVHEX_DF_DLATOM_H 
 
#include <string> 
#include <vector> 
#include "Predicate.h" 
#include "Terms.h" 
#include "Updates.h" 
 
typedef std::vector<dlvhex::df::Predicate> Pred1Dim; 
typedef std::vector<Pred1Dim>		   Pred2Dim; 
 
namespace dlvhex { 
namespace df { 
 
  /** 
   * @brief A DLAtom of the form DL[\lambda;\query](Terms). 
   */ 
  class DLAtom  
    { 
    private: 
      /// A set of updates to build up \lambda 
      Updates updates;	 
      
      /// A query to the KB in the form of UCQ (Union of Conjuctive Queries) 
      Pred2Dim ucq; 

      /// Simple query, with concept/role name only.
      /// Used when --cq=no
      std::string query;
      
      /// A set of Terms to get the instantiated result of the query 
      Terms terms; 
 
    public: 
      DLAtom(Updates&, Predicate&, Terms&);
      DLAtom(Pred2Dim&, Terms&); 
      DLAtom(Updates&, Pred2Dim&, Terms&); 
      DLAtom(Updates&, std::string, Terms&);
      
      std::string 
	toString(); 
    }; 
  
}	// namespace df 
}	// namespace dlvhex 

#endif /* _DLVHEX_DF_DLATOM_H */
