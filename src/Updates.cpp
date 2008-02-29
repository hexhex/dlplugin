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
 * @file   Updates.cpp
 * @author DAO Tran Minh
 * @date   Tue Dec 18 17:20:24 2007
 * 
 * @brief  A set of Updates.
 * 
 * 
 */

#include "Updates.h"

namespace dlvhex {
namespace df {

Updates::Updates() 
{ }

void
Updates::addUpdate(const Update& update_) 
{
	if (!gotThisUpdate(update_)) 
	{
		updates.push_back(update_);
	}
}

bool
Updates::gotThisUpdate(const Update& update_)
{
	std::vector<Update>::iterator pos;
	if (updates.size() == 0)
	{
		return false;
	}

	for (pos = updates.begin(); pos != updates.end(); pos++) 
	{
		if (*pos == update_)
		{
			return true;
		}
	}
	return false;
}

void 
Updates::insertUpdates(const Updates& updates_) 
{
	if (updates.size() > 0) 
	{
		std::vector<Update> us = updates_.updates;
		std::vector<Update>::iterator pos;
		for (pos = us.begin(); pos != us.end(); pos++)
		{
			if (!gotThisUpdate(*pos))
			{
				updates.push_back(*pos);
			}
		}
		//updates.insert(updates.begin(), updates_.updates.begin(), updates_.updates.end());
	} 
	else 
	{
		updates = updates_.updates;
	}
}

std::string 
Updates::toString() 
{
	std::string tmp = "";
	if (updates.size() > 0) 
		{
			std::vector<Update>::iterator pos;
			for (pos = updates.begin(); pos != updates.end(); pos++) 
			{
				tmp = tmp + pos->toString() + ", ";
			}
			tmp = tmp.erase(tmp.length()-2) + ";";
		}
	return tmp;
}

}	// namespace df
}	// namespace dlvhex
