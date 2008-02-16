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

DLAtom::DLAtom(Pred2Dim& ucq_, Terms& terms_)
	: ucq(ucq_), terms(terms_) 
{ }

DLAtom::DLAtom(Updates& updates_, Pred2Dim& ucq_, Terms& terms_)
	: updates(updates_), ucq(ucq_), terms(terms_) 
{ }

std::string 
DLAtom::toString() 
{
	std::string query = "";
	Pred2Dim::iterator u_pos;
	Pred1Dim::iterator cq_pos;

	for (u_pos = ucq.begin(); u_pos != ucq.end(); u_pos++) 
		{
			for (cq_pos = u_pos->begin(); cq_pos != u_pos->end(); cq_pos++) 
				{
					query = query + cq_pos->toString() + ",";
				}
			query = query.erase(query.length()-1) + " v ";
		}
	query = query.erase(query.length()-3);
	return ("DL[" + updates.toString() + query + "](" + terms.toString() + ")");
}

}	// namespace df
}	// namespace dlvhex
