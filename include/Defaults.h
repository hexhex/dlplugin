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
 * @file   Defaults.h
 * @author DAO Tran Minh 
 * @date   Tue Dec 18 17:20:24 2007 
 *  
 * @brief  A list of Default(s). 
 *  
 *  
 */

#ifndef _DLVHEX_DF_DEFAULTS_H_ 
#define _DLVHEX_DF_DEFAULTS_H_

#include <list>
#include "Default.h"
#include "DLRules.h"
#include "Updates.h"

namespace dlvhex {
namespace df {

  class Default;

  class Defaults
  {
  private:
    unsigned int count;
    std::list<Default> dfs;

  public:
    Defaults();

    int size();
    
    void
      push_back(Default&);

    Updates
      getLambda(std::string);

    DLRules
      getDLRules(bool cqmode, int dftrans);

    std::string
      toString();
  };

} // namespace df
} // namespace dlvhex

#endif /* _DLVHEX_DF_DEFAULTS_H */
