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
 
#include <vector> 
#include "MTerm.h" 
#include "Unifier.h" 
 
typedef std::vector<dlvhex::df::Unifier>	Unifier1Dim; 
typedef std::vector<Unifier1Dim>	Unifier2Dim; 
 
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
      std::vector<MTerm> terms; 
      
    public: 
      Terms(); 
      
      bool  
	isEmpty(); 
      
      /** 
       * Check if this Terms has got a MTerm or not 
       * 
       * @param t the term to be checked 
       * 
       * @return true if the MTerm t is already in this Terms 
       */ 
      bool 
	gotThisTerm(MTerm& t); 

      bool
	containsTerms(Terms& ts2);
      
      /** 
       * Compare the generality to another Terms 
       * 
       * @param ts2 
       * 
       * @return MORE_GENERAL if this Terms is more general than ts2 
       *         LESS_GENERAL if this Terms is less general than ts2 
       *         NOT_COMPARABLE if this Terms and ts2 are incomparable 
       * 
       */ 
      ComparisonResult 
	compareTo(Terms& ts2); 
      
      std::vector<MTerm>  
	getMTerms(); 
      
      Terms& 
	operator=(const Terms&); 
      
      void  
	addTerm(MTerm term_); 
      
      /** 
       * Append a Terms (a set of MTerm) into this Terms 
       * 
       * @param ts2 the Terms to be appended 
       */ 
      void  
	insertNewTerms(Terms& ts2); 
      
      /** 
       * Project one Terms to another one 
       * 
       * @param ts2 
       * 
       * @return a projected Terms, where all MTerm not in ts2 will become anonymous represented by "_" 
       */ 
      Terms  
	projectTo(Terms& ts2); 
      
      /** 
       * Unify a Terms 
       * 
       * @param u The unifier 
       * @param keep_old_name Mark if anonymous variables will be set as '_' 
       *                      or their old name 
       * 
       * @return The unified Terms 
       */ 
      Terms 
	unify(Unifier& u, bool keep_old_name); 
      
      /** 
       * Check if 2 Terms are unifiable or not 
       *  
       * @param ts2 the Tersm to be checked 
       * 
       * @return an empty unifier if the two terms are not unifiable 
       *         the unifer otw. 
       */ 
      Unifier 
	isUnifiable(Terms& ts2); 
 
      /** 
       * Rename all variables in preparing for unification 
       * 
       * @param std_id The id from a Default, to be added to each variable's name 
       */ 
      void 
	rename_terms(std::string& str_id); 
      
      std::string  
	toString();	 
    }; 
  
}	// df 
}	// dlvhex 
 
#endif /* _DLVHEX_DF_TERMS_H */
