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
 * @file   MTern.h
 * @author DAO Tran Minh
 * @date   Wed Dec 19 11:18:24 2007
 * 
 * @brief  A Term. Have to used MTerm to avoid name clash with dlvhex::Term (right now dlvhex doesn't have any namespace)
 * 
 * 
 */

#ifndef _DLVHEX_DF_TERM_H
#define _DLVHEX_DF_TERM_H

#include <string>
#include <map>

namespace dlvhex {
namespace df {

  enum UnificationResult
  {
    BOTH_VARS,
    VAR1CONST2,
    VAR2CONST1,
    BOTH_CONSTS,
    NOT_UNIFIABLE
  };
  
  /**
   * @brief A Term. Have to use the name MTerm in order to avoid a naming clash with dlvhex's Term.
   *
   */
  class MTerm 
  {
  private:
    std::string term_representation;
    bool isvar;
    
  public:
    MTerm(const std::string&);
    
    MTerm(const std::string&, bool);
    
    bool 
      isVar();
	
    bool 
      operator==(const MTerm&) const;
    
    MTerm&
      operator=(const MTerm&);

    /**
     * Check if 2 terms are unifiable or not
     *
     * @param t2 the 2nd term
     * 
     * @return BOTH_VARS if two terms are both variables
     *         VAR1CONST2 if 1st term is a variable and 2nd term is a const
     *         VAR2CONST1 if 1st term is a const and 2nd is a variable
     *         BOTH_CONSTS if both terms are a same const
     *         NOT_UNIFIABLE if both terms are different constants
     */
    UnificationResult
      isUnifiable(const MTerm& t2);
    
    void
      setName(std::string);
    
    std::string 
      toString();
  };
  
}	// namespace df
}	// namespace dlvhex

#endif /* _DLVHEX_DF_TERM_H */
