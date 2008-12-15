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
 * @file   Defaults.cpp 
 * @author DAO Tran Minh 
 * @date   Tue Dec 18 17:20:24 2007 
 *  
 * @brief  A list of Default(s). 
 *  
 *  
 */ 

#include <iostream>
#include <sstream>
#include "Defaults.h"

namespace dlvhex {
namespace df {

  Defaults::Defaults()
  {
    count = 0;
  }

  int
  Defaults::size()
  {
    return dfs.size();
  }

  void
  Defaults::push_back(Default& df)
  {
    df.setParent(this); 
    df.setID(++count); 
    dfs.push_back(df);
  }

  Updates
  Defaults::getLambda(std::string prefix)
  {
   Updates us; 
    if (dfs.size() > 0)  
      { 
	for (std::list<Default>::iterator i = dfs.begin(); i != dfs.end(); ++i)
	  { 
	    us.insert(i->getUpdates(prefix));
	  } 
      } 
    return us; 
  }

  DLRules
  Defaults::getDLRules(bool cqmode, int dftrans)
  {
    DLRules rules; 
    for (std::list<Default>::iterator i = dfs.begin(); i != dfs.end(); ++i)  
      { 
	DLRules rs;
	switch (dftrans)
	  {
	  case 1:
	    rs = i->getDLRules_Pi(cqmode);
	    break;
	  case 2:
	    rs = i->getDLRules_Omega(cqmode);
	    break;
	  case 3:
	    rs = i->getDLRules_Upsilon(cqmode);
	    break;
	  }
	rules.insert(rs); 
      }
    return rules;
  }

  std::list<std::string>
  Defaults::getConclusionsNames()
  {
    std::list<std::string> tmp;
    for (std::list<Default>::iterator i = dfs.begin(); i != dfs.end(); ++i)
      {
	std::list<std::string> tmp_ = i->getConclusionNames();
	for (std::list<std::string>::iterator j = tmp_.begin(); j != tmp_.end(); ++j)
	  {
	    if (find(tmp.begin(), tmp.end(), *j) == tmp.end())
	      {
		tmp.push_back(*j);
	      }
	  }
      }

    // testing
    //std::cout << "Conclusions' Names:"<< std::endl;
    //for (std::list<std::string>::iterator k = tmp.begin(); k != tmp.end(); ++k)
    //  {
    //std::cout << *k << std::endl;
    //  }
    return tmp;
  }

  std::string
  Defaults::toString()
  {
    std::ostringstream o;
    int i = 0;
    for (std::list<Default>::iterator ci = dfs.begin(); ci != dfs.end(); ++ci)
      {
	o << "Default (" << ++i << "): " << ci->toString() << "\n";
      }
    return o.str();
  }

} // namespace df
} // namespace dlvhex
