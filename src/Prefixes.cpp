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
 * @file   Prefixes.cpp
 * @author DAO Tran Minh
 * @date   Sat May 17 09:48:24 2008
 * 
 * @brief  A vector of prefixes.
 * 
 * 
 */

#include "Prefixes.h"
#include <iostream>

namespace dlvhex { 
namespace df {

Prefixes::Prefixes()
{
}

void
Prefixes::addPrefix(Prefix& p)
{
  prefixes.push_back(p);
}
  
std::string
Prefixes::getPath(std::string& name_)
{
  std::string path_ = "";
  std::vector<Prefix>::iterator pos;
  
  if (prefixes.size() > 0)
    {
      for (pos = prefixes.begin(); pos != prefixes.end(); ++pos)
	{
	  if (name_.compare(pos->getName()) == 0)
	    {
	      path_ = pos->getPath();
	      break;
	    }
	}
    }
  
  return path_;
}

} // namespace df
} // namespace dlvhex
