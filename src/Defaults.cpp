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
 * @brief  A set of Defaults.
 * 
 * 
 */

#include "Defaults.h"

namespace dlvhex {
namespace df {

Defaults::Defaults() {
}

void Defaults::addDefault(Default& df) {
	df.setParent(this);
	dfs.push_back(df);	
}

Updates Defaults::getLambda() {
	Updates us;
	if (dfs.size() > 0) {
		std::vector<Default>::iterator pos;	
		for (pos = dfs.begin(); pos != dfs.end(); pos++) {
			us.addUpdate(pos->getUpdate4Lambda());
		}
	}
	return us;
}

Updates Defaults::getLambdaPrime() {
	Updates us;
	if (dfs.size() > 0) {
		std::vector<Default>::iterator pos;	
		for (pos = dfs.begin(); pos != dfs.end(); pos++) {
			us.addUpdate(pos->getUpdate4LambdaPrime());
		}
	}
	return us;
}

DLRules Defaults::getDLRules() {
	DLRules rules;
	//std::cout << "Getting rules from 1 default!!!" << std::endl;
	std::vector<Default>::iterator pos;	
	for (pos = dfs.begin(); pos != dfs.end(); pos++) {
		DLRules rs = pos->getDLRules();
		rules.insertDLRules(rs);
	}
	return rules;
}

}	// namespace df
}	// namespace dlvhex
