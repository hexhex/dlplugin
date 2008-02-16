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
 * @file   Update.h
 * @author DAO Tran Minh
 * @date   Tue Dec 18 17:20:24 2007
 * 
 * @brief  An update of the form: Concept/Role +=/-= Predicate.
 * 
 * 
 */

#ifndef _DLVHEX_DF_UPDATE_H
#define _DLVHEX_DF_UPDATE_H

#include <string>

namespace dlvhex {
namespace df {

	enum Operator 
		{ 
			UPLUS, 
			UMINUS 
		};

	/**
	 * @brief  An update of the form: Concept/Role +=/-= Predicate
	 */
	class Update 
	{
	private:
		/// Concept or Role name
		std::string cr_name;

		/// operator += or -=
		Operator op;

		/// predicate used for updating the concept or role
		std::string predicate_name;

	public:
		Update(const std::string&, Operator, const std::string&);

		std::string 
		toString();
	};

}	// namespace df
}	// namespace dlvhex

#endif	/* _UPDATE_H */
