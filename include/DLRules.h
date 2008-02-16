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
 * @file   DLRules.h
 * @author DAO Tran Minh
 * @date   Tue Dec 18 17:20:24 2007
 * 
 * @brief  A set of DL Rules.
 * 
 * 
 */

#ifndef _DLVHEX_DF_DLRULES_H
#define _DLVHEX_DF_DLRULES_H

#include <vector>
#include "DLRule.h"

namespace dlvhex {
namespace df {

	class DLRules 
	{
	private:
		std::vector<DLRule> dlrules;

	public:
		DLRules();
	
		void 
		addDLRule(const DLRule&);
	
		/**
		 * Append a DLRules (a set of DLRule) into this DLRules
		 *
		 * @param dlrs2 the DLRules to be appended
		 */
		void 
		insertDLRules(const DLRules& dlrs2);
	
		std::string 
		toString();
};

}	// namespace df
}	// namespace dlvhex

#endif /* _DLVHEX_DF_DLRULES_H */
