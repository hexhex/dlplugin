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

std::vector<MTerm> Terms::getMTerms() {
	return terms;
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

// insert all the terms which are in t1 and not in (*this*) to (*this*)
void Terms::insertNewTerms(Terms& t1) {
	std::vector<MTerm>::iterator pos;
	for (pos = t1.terms.begin(); pos != t1.terms.end(); pos++) {
		if (!gotThisTerm(*pos)) {
			addTerm(*pos);
		}
	}
}

// Project this set of terms (*this*) to another set of term (t1)
// All terms which are not in t1 will be set to "_"
Terms Terms::projectTo(Terms& t1) {
	Terms ts;
	MTerm anonymous("_");
	std::vector<MTerm>::iterator pos;
	for (pos = terms.begin(); pos != terms.end(); pos++) {
		if (!t1.gotThisTerm(*pos)) {
			ts.addTerm(anonymous);
		} else {
			ts.addTerm(*pos);
		}
	}
	return ts;
}

bool Terms::gotThisTerm(MTerm& t) {
	std::vector<MTerm>::iterator pos;
	for (pos = terms.begin(); pos != terms.end(); pos++) {
		if (*pos == t) {
			return true;
		}
	}
	return false;
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

}	// namespace df
}	// namespace dlvhex
