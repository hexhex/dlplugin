/* dlvhex-dlplugin -- Integration of Answer-Set Programming and Description Logics.
 *
 * Copyright (C) 2008 DAO Tran Minh
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
 * @file   Unifier.h
 * @author DAO Tran Minh
 * @date   Wed Feb 13 16:33:00 2008
 * 
 * @brief  Unifier of predicates/predicates, no concern of function symbols.
 * 
 * 
 */

#ifndef _DLVHEX_DF_UNIFIER_H
#define _DLVHEX_DF_UNIFIER_H

#include <string>
#include <map>
#include <set>
#include <vector>

typedef std::multimap<std::string, std::string> MultimapStr;
typedef std::vector<std::string> String1Dim;
typedef std::vector<String1Dim> String2Dim;

namespace dlvhex {
namespace df {

/**
 * @brief Unifier of predicates/predicates, no concern of function symbols.
 */
class Unifier
{
	private:
		/// The set of substitutions 
		MultimapStr unifier;

		/// The set of all equivalent classes 
		/// For example if the unifier is: [X1/Y1, Y1/Y2, X2/"mmsc"]
		/// then we have 2 equivalent classes, namely:
		/// {X1, Y1, Y2} and {X2, "mmsc"}
		String2Dim equivalent_classes;

		/**
		 * Create the equivalent class for a var term
		 *
		 * @param var Name of the var term
		 *
		 * @return The first string value in the equivalent class created for this term
		 */
		std::string
		createEquivalentClass(std::string var);

		/**
		 * Get the first string representation in the equivalent class (if any)
		 * corresponding to this var 
		 *
		 * @param var The variable to be concerned
		 *
		 * @return An empty string if there is no equivalent class for this var
		 *         Or the first string representation in the equivalent class for this var
		 */
		std::string
		firstEquivalentTerm(std::string var);

		/**
		 * Check if a term_representation represents a variable
		 *
		 * @param term_representation The term to be checked
		 *
		 * @return true if this term_representation represents a variable
		 *         false otw.
		 */
		bool
		strVar(const std::string term_representation);

		/**
		 * Check if an equivalent class contains a term representation or not
		 *
		 * @param eq_class The equivalent class to be checked
		 * @param var The term representation to be checked
		 *
		 * @return true if eq_class contains var
		 *         false otw.
		 */
		bool
		gotThisVar(String1Dim& eq_class, std::string& var);

	public:
		/**
		 * Check if this unifier contains a pair of term representation or not (a substitution)
		 *
		 * @param first_ The first term of the pair
		 * @param second_ The second term of the pair
		 *
		 * @return true if this unifier contains <first_, second_>
		 *         false otw.
		 */
		bool
		gotThisPair(std::string& first_, std::string& second_);

		Unifier&
		operator=(const Unifier&);

		MultimapStr&
		getUnifier();

		/**
		 * Check if this unifier is consistent with a pair of term representation (a substitution)
		 *
		 * @param var The variable of the substitution
		 * @param cons The constant of the substition
		 *
		 * @return true if this unifier is consistent with [var/cons]
		 *         false otw.
		 */		
		bool
		isConsistent(std::string var, std::string cons);

		/**
		 * Check  if this unifer is consistent with another unifier or not
		 *
		 * @param u2 The other unifier to be checked
		 *
		 * @return true if this unifier is consistent with every pair of u2
		 *         false if there is a pair of u2 such that this unifier is not consistent with
		 */
		bool
		isConsistent(Unifier& u2);

		/**
		 * Find the unified value for a variable
		 *
		 * @param var The variable
		 * @param keep_old_name Mark if the old representation of the variable will be kept 
		 *        in case no unified value found
		 */
		std::string
		unifiedValue(std::string var, bool keep_old_name);
};

}	// df
}	// dlvhex

#endif /* _DLVHEX_DF_UNIFIER_H */
