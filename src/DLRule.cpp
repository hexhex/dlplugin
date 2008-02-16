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
DLRule::addPositiveBody(const Predicate& positive_body) 
{
	positive_bodies.push_back(positive_body);
}

void 
DLRule::addNegativeBody(const Predicate& negative_body) 
{
	negative_bodies.push_back(negative_body);
}

void 
DLRule::addPositiveDLAtom(const DLAtom& positive_dl_atom) 
{
	positive_dl_atoms.push_back(positive_dl_atom);
}

void 
DLRule::addNegativeDLAtom(const DLAtom& negative_dl_atom) 
{
	negative_dl_atoms.push_back(negative_dl_atom);
}

std::string 
DLRule::toString() 
{
	std::vector<Predicate>::iterator ppos;
	std::vector<DLAtom>::iterator dpos;
	std::string tmp = head.toString();

	if (positive_bodies.size() > 0 || negative_bodies.size() > 0 || positive_dl_atoms.size() > 0 || negative_dl_atoms.size() > 0) 
		{
			tmp += " :- ";
			if (positive_bodies.size() > 0) 
				{
					for (ppos = positive_bodies.begin(); ppos != positive_bodies.end(); ppos++) 
					{
						tmp = tmp + ppos->toString() + ", ";
					}
				}
			if (positive_dl_atoms.size() > 0) 
				{
					for (dpos = positive_dl_atoms.begin(); dpos != positive_dl_atoms.end(); dpos++) 
					{
						tmp = tmp + dpos->toString() + ", ";
					}
				}
			if (negative_dl_atoms.size() > 0) 
				{
					for (dpos = negative_dl_atoms.begin(); dpos != negative_dl_atoms.end(); dpos++) 
					{
						tmp = tmp + "not " + dpos->toString() + ", ";
					}
				}
			if (negative_bodies.size() > 0) 
				{
					for (ppos = negative_bodies.begin(); ppos != negative_bodies.end(); ppos++) 
					{
						tmp = tmp + "not " + ppos->toString() + ", ";
					}
				}
			tmp = tmp.erase(tmp.length()-2) + ".\n";
		} 
	else 
		{
			tmp += ".\n";
		}
	return tmp;
}

}	// namespace df
}	// namespace dlvhex
