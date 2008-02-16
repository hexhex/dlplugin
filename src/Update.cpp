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
 * @file   Update.cpp
 * @author DAO Tran Minh
 * @date   Tue Dec 18 17:20:24 2007
 * 
 * @brief  An update of the form: Concept/Role +=/-= Predicate.
 * 
 * 
 */

#include "Update.h"

namespace dlvhex {
namespace df {

Update::Update(const std::string& cr_name_, Operator op_, const std::string& predicate_name_)
	: cr_name(cr_name_), op(op_), predicate_name(predicate_name_) 
{ }

std::string 
Update::toString() 
{
	std::string tmp;
	switch (op) 
		{
			case UPLUS:	tmp = "+=";
				break;
			case UMINUS: tmp = "-=";
				break;
		}	
	return cr_name + tmp + predicate_name;
}

}	// namespace df
}	// namespace dlvhex
