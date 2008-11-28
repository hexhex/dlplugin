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
 * @file   Terms.h 
 * @author DAO Tran Minh 
 * @date   Tue Dec 18 17:20:24 2007 
 *  
 * @brief  A set of Terms. 
 *  
 *  
 */ 
 
#ifndef _DLVHEX_DF_TERMS_H 
#define _DLVHEX_DF_TERMS_H 
 
#include <list>
#include "MTerm.h" 
 
namespace dlvhex { 
namespace df { 
 
  enum ComparisonResult 
    { 
      MORE_GENERAL, 
      LESS_GENERAL, 
      NOT_COMPARABLE 
    }; 
  
  /** 
   * @brief A set of MTerm(s) 
   */ 
  class Terms  
    { 
    private: 
      std::list<MTerm> terms; 
      
    public: 
      Terms();
      Terms(std::list<MTerm>&); 
      
      bool  
	isEmpty();

      bool
	gotThisTerm(MTerm&);

      std::list<MTerm>::iterator
	begin();

      std::list<MTerm>::iterator
	end();

      void  
	push_back(MTerm);

      void
	insertNewTerms(Terms&);

      std::string  
	toString();	 
    }; 
  
} // df 
} // dlvhex 
 
#endif /* _DLVHEX_DF_TERMS_H */
