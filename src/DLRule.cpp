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
 * @file   DLRule.cpp 
 * @author DAO Tran Minh 
 * @date   Tue Dec 18 17:20:24 2007 
 *  
 * @brief  A DL Rule. 
 *  
 *  
 */ 

#include "DLRule.h"

namespace dlvhex {
namespace df {

  DLRule::DLRule(const Predicate& head_)
    : head(head_) 
  { }
  
  void
  DLRule::addPositiveBody(Predicate& positive_body) 
  {
    positive_bodies.push_back(positive_body);
  }
  
  void 
  DLRule::addNegativeBody(Predicate& negative_body) 
  {
    negative_bodies.push_back(negative_body);
  }
  
  void 
  DLRule::addDLAtom(DLAtom& dl_atom) 
  {
    dl_atoms.push_back(dl_atom);
  }

  void
  DLRule::insertDLAtoms(DLAtoms& dl_atoms_)
  {
    dl_atoms.insert(dl_atoms_);    
  }
  
  std::string 
  DLRule::toString() 
  {
    std::string tmp = head.toString();
    
    if (positive_bodies.size() > 0 || negative_bodies.size() > 0 || dl_atoms.size() > 0) 
      {
	tmp += " :- ";
	if (positive_bodies.size() > 0) tmp = tmp + positive_bodies.toString(mRule) + ", ";
	if (dl_atoms.size() > 0) tmp = tmp + dl_atoms.toString() + ", ";
	if (negative_bodies.size() > 0) tmp = tmp + negative_bodies.toString(mRule, true) + ", ";
	tmp = tmp.substr(0, tmp.length() - 2) + ".\n";
      } 
    else 
      {
	tmp += ".\n";
      }
    return tmp;
  }
  
} // namespace df
} // namespace dlvhex
