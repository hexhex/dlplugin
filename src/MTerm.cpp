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
 * @file   MTern.cpp
 * @author DAO Tran Minh
 * @date   Tue Dec 18 17:20:24 2007
 * 
 * @brief  A Term.
 * 
 * 
 */

#include "MTerm.h"

namespace dlvhex {
namespace df {

MTerm::MTerm(const std::string& term_representation_) {
	term_representation = term_representation_;
	isvar = ((term_representation[0] >= 'A')&&(term_representation[0] <= 'Z'));
}

MTerm::MTerm(const std::string& term_representation_, bool isvar_) : term_representation(term_representation_), isvar(isvar_) {
}

bool MTerm::isVar() {
	return isvar;
}

std::string MTerm::toString() {
	return term_representation;
}

}	// namespace df
}	// namespace dlvhex
