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
 * @file   Predicate.h 
 * @author DAO Tran Minh 
 * @date   Tue Dec 18 17:20:24 2007 
 *  
 * @brief  An ausilary predicate. 
 *  
 *  
 */ 
 
#ifndef _DLVHEX_DF_PREDICATE_H 
#define _DLVHEX_DF_PREDICATE_H 
 
#include "Terms.h" 
 
namespace dlvhex { 
namespace df { 

  enum PredicateMode
  {
    mRule, mQuery, mDefault
  };
 
  /** 
   * @brief An auxilary predicate  
   */ 
  class Predicate  
    { 
    private: 
      bool isNull;
      bool isStrongNegated; 
      std::string predicate_name; 
      std::string prefix;
      Terms terms; 
      
    public: 
      Predicate();

      Predicate(const std::string&); 
      
      Predicate(const std::string&, const Terms&); 
      
      Predicate(const std::string&, const MTerm&); 
		
      Predicate(bool, const std::string&); 
      
      Predicate(bool, const std::string&, const Terms&); 

      Predicate(bool, const std::string&, const std::string&, const Terms&);

      bool
	isNULL();
      
      void
	setNULL(bool);

      bool  
	isStronglyNegated(); 

      bool
	gotThisTerm(MTerm&);
      
      std::string  
	getPredicateName();

      std::string
	getPrefix();

      std::string  
	getPredicateNameWithNS(); 
      
      /** 
       * @return predicate's name with prefix "not_" if it's strongly negated 
       *         predicate's name otw. 
       */ 
      std::string  
	getSignedPredicateName(); 
      
      /** 
       * @return predicate's name with its strong negation sign 
       *         i.e. -P if the predicate is strongly negated 
       *               P otw. 
       */ 
      std::string  
	getLiteralName(); 
      
      std::string  
	getLiteralNameWithNS();
      
      /** 
       * @return negated predicate's name with its strong negation sign 
       *         i.e. P if the predicate is strongly negated 
       *         -P otw. 
       */ 
      std::string  
	getNegatedLiteralName(); 

      std::string  
	getNegatedLiteralNameWithNS();

      Terms&  
	getTerms();  
      
      Predicate& 
	operator=(const Predicate&); 
      
      std::string  
	toString(); 

      std::string  
	toStringWithNS(); 
      
      std::string  
	toNegatedString(); 

      std::string  
	toNegatedStringWithNS(); 
    }; 
  
}	// namespace df 
}	// namespace dlvhex 

#endif /* _DLVHEX_DF_PREDICATE_H */
