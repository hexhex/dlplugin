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
 * @file   Terms.cpp
 * @author DAO Tran Minh
 * @date   Tue Dec 18 17:20:24 2007
 * 
 * @brief  A set of Terms.
 * 
 * 
 */

#include "Terms.h"

namespace dlvhex {
namespace df {

Terms::Terms() {
}

void Terms::addTerm(MTerm term_) {
	terms.push_back(term_);
}

void Terms::removeLastTerm() {
	if (terms.size() > 0) {
		terms.pop_back();
	}
}

bool Terms::isEmpty() {
	return (terms.size() == 0);
}

std::string Terms::toString() {
	std::string tmp = "";

	if (terms.size() > 0) {
		std::vector<MTerm>::iterator pos = terms.begin();
		tmp = terms.begin()->toString();
		pos = terms.begin();
		++pos;
		for (; pos != terms.end(); pos++) {
			tmp += ", " + pos->toString();
		}
	}
	return tmp;
}

std::vector<MTerm> Terms::getMTerms() {
	return terms;
}

}	// namespace df
}	// namespace dlvhex
