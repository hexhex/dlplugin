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
 * @file   DLAtom.cpp 
 * @author DAO Tran Minh 
 * @date   Tue Dec 18 17:20:24 2007 
 *  
 * @brief  A DLAtom of the form DL[\lambda;\ucq](Terms). 
 *  
 *  
 */ 

#include "DLAtom.h"

namespace dlvhex {
namespace df {

  DLAtom::DLAtom(bool naf_, Updates& us, Predicate& p, Terms& ts)
    : naf(naf_), updates(us), ucq(p), terms(ts), query("")
  { }

  DLAtom::DLAtom(bool naf_, Pred1Dim& ps, Terms& ts)
    : naf(naf_), ucq(ps), terms(ts), query("")
  { }

  DLAtom::DLAtom(bool naf_, Updates& us, Pred1Dim& ps, Terms& ts)
    : naf(naf_), updates(us), ucq(ps), terms(ts), query("")
  { }

  DLAtom::DLAtom(bool naf_, Updates& us, Pred2Dim& ucq_, Terms& ts)
    : naf(naf_), updates(us), ucq(ucq_), terms(ts), query("")
  { }

  DLAtom::DLAtom(bool naf_, Updates& us, std::string query_, Terms& ts)
    : naf(naf_), updates(us), query(query_), terms(ts)
  { }

  std::string
  DLAtom::toString()
  {
    std::string snot = "";
    if (naf) snot = "not ";
    if (query == "") // cqmode is ON
    {
      return (snot + "DL[" + updates.toString() + ucq.toString(mQuery) + "](" + terms.toString() + ")"); 
    }    
    return (snot + "DL[" + updates.toString() + query + "](" + terms.toString() + ")"); 
  }
  
} // namespace df
} // namespace dlvhex
