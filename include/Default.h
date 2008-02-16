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
 * @file   Default.h
 * @author DAO Tran Minh
 * @date   Tue Dec 18 17:20:24 2007
 * 
 * @brief  A Default of the form [A:B]/[C].
 * 
 * 
 */

#ifndef _DLVHEX_DF_DEFAULT_H
#define _DLVHEX_DF_DEFAULT_H

#include <vector>
#include <sstream>
#include "DLRules.h"
#include "Defaults.h"
#include "Predicate.h"
#include "Update.h"

#define PREFIX_IN 			"in_"
#define	PREFIX_OUT 			"out_def_"
#define	PREFIX_PRED			"p_"
#define PREFIX_ALL_IN 	"all_in_def_"
#define PREFIX_ALL_PRED	"all_p_def_"
#define PREFIX_MY 			"my_"

namespace dlvhex {
namespace df {

	class Defaults;

	/**
	 * @breif A Default of the form [A:B]/[C].
	 */
	class Default 
	{
	private:
		/// Each default is assigned with an an id
		/// This id is used in the all_in_def_id and out_def_id names 
		/// for guessing the in/out status of the whole conjunctive conclusion
		unsigned int id;

		/// A boolean variable used to mark NULL defaults
		/// A NULL default is generated when the parser fails to parse an input
		bool is_null;

		/// The pointer to the set of all defaults
		Defaults* parent;

		/// The premise can be a conjunction
		Pred1Dim premise;

		/// The justification can be a set of conjunctions
		Pred2Dim justification;

		/// The conclusion can be a conjunction
		Pred1Dim conclusion;

		// Cache these special predicates (all_in_ and out_) for building forcing rules
		Predicate all_in_p;	
		Predicate out_p;

	public:
		Default(bool);
	
		Default(Pred1Dim, Pred2Dim, Pred1Dim);

		static 
		Default null_default;
	
		unsigned int
		getID();

		bool 
		isNULL();

		void 
		setID(unsigned int);
	
		void 
		setParent(Defaults*);
	
		Pred1Dim 
		getConclusion();
	
		/**
		 * @return An Updates to build \lambda
		 *         \union(concept/role +=/-= p_concept/p_role)
		 *         for all concepts/roles in the conclusion
		 */
		Updates
		getUpdates4Lambda();
	
		/**
		 * @return An Updates to build \lambda'
		 *         \union(concept/role +=/-= in_concept/in_role)
		 *         for all concepts/roles in the conclusion
		 */
		Updates 
		getUpdates4LambdaPrime();
	
		/**
		 * @param ps A set of predicates
		 *
		 * @return A Terms including all ps's MTerm(s) but no overlap
		 *         e.g. ps = P(X,Y), Q(Y,Z)
		 *         then Terms = <X,Y,Z>
		 */
		Terms 
		getAllDistinctTerms(Pred1Dim ps);

		/**
		  * @return The all_in_def_id predicate corresponding to this default
		  */
		Predicate&
		getAllInPred();

		/**
		  * @return The out_def_id predicate corresponding to this default
		  */
		Predicate&
		getOutPred();
	
		/**
		  * @return All the transformed DL rules corresponding to this default
		  */
		DLRules 
		getDLRules();
	
		std::string 
		toString();
};

}	// namespace df
}	// namespace dlvhex

#endif /* _DLVHEX_DF__H */
