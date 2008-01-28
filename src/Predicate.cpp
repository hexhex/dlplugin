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
 * @file   Predicate.cpp
 * @author DAO Tran Minh
 * @date   Tue Dec 18 17:20:24 2007
 * 
 * @brief  A Predicate.
 * 
 * 
 */

#include "Predicate.h"

namespace dlvhex {
namespace df {

Predicate::Predicate() {
}

Predicate::Predicate(const std::string& predicate_name_) : isStrongNegated(false), predicate_name(predicate_name_) {
}

Predicate::Predicate(const std::string& predicate_name_, const Terms& terms_) : isStrongNegated(false), predicate_name(predicate_name_), terms(terms_) {
}

Predicate::Predicate(const std::string& predicate_name_, const MTerm& term_) : isStrongNegated(false), predicate_name(predicate_name_) {
	terms.addTerm(term_);
}


Predicate::Predicate(bool isStrongNegated_, const std::string& predicate_name_) : isStrongNegated(isStrongNegated_), predicate_name(predicate_name_) {
}

Predicate::Predicate(bool isStrongNegated_, const std::string& predicate_name_, const Terms& terms_) : isStrongNegated(isStrongNegated_), predicate_name(predicate_name_), terms(terms_) {
}

bool Predicate::isStronglyNegated() {
	return isStrongNegated;
}

std::string Predicate::getPredicateName() {
	return predicate_name;
}

std::string Predicate::getLiteralName() {
	std::string tmp = "";
	if (isStrongNegated) {
		tmp = "-";
	}
	return (tmp + predicate_name);
}

std::string Predicate::getNegatedLiteralName() {
	std::string tmp = "";
	if (!isStrongNegated) {
		tmp = "-";
	}
	return (tmp + predicate_name);
}

Terms Predicate::getTerms() {
	return terms;
}

void Predicate::applyNegation() {
	isStrongNegated = !isStrongNegated;
}

std::string Predicate::toString() {
	std::string tmp = "";
	if (isStrongNegated) {
		tmp = "-";
	}
	if (!terms.isEmpty()) {
		tmp = tmp + predicate_name + "(" + terms.toString() + ")";
	} else {
		tmp += predicate_name;
	}
	return tmp;
}

std::string Predicate::toNegatedString() {
	std::string tmp = "";
	if (!isStrongNegated) {
		tmp = "-";
	}
	if (!terms.isEmpty()) {
		tmp = tmp + predicate_name + "(" + terms.toString() + ")";
	} else {
		tmp += predicate_name;
	}
	return tmp;
}

}	// namespace df
}	// namespace dlvhex
