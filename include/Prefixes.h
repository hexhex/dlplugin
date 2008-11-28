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
 * @file   Prefixes.h
 * @author DAO Tran Minh
 * @date   Sat May 17 09:48:24 2008
 * 
 * @brief  A vector of prefixes.
 * 
 * 
 */

#ifndef _DLVHEX_DF_PREFIXES_H_
#define _DLVHEX_DF_PREFIXES_H_

#include <list>
#include "Prefix.h"

namespace dlvhex {
namespace df {

  class Prefixes
    {
    private:
      std::list<Prefix> prefixes;
      
    public:		
      Prefixes();

      void
	push_back(Prefix&);
      
      std::string
	getPath(std::string&);
    };
  
} // namespace df
} // namespace dlvhex

#endif /* _DLVHEX_DF_PREFIXES_H_ */
