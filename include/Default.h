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

#define PREFIX_IN 	"in_"
#define	PREFIX_OUT 	"out_"
#define	PREFIX_PRED "p_"

namespace dlvhex {
namespace df {

class Defaults;

class Default {
private:
	unsigned int	id;
	bool is_null;
	Defaults* parent;
	Pred1Dim premise;
	Pred2Dim justification;
	Pred1Dim conclusion;

public:
	Default(bool);
	Default(Pred1Dim, Pred2Dim, Pred1Dim);

	static Default null_default;
	bool isNULL();
	void setID(unsigned int);
	void setParent(Defaults*);
	Pred1Dim getConclusion();
	Updates getUpdates4Lambda();
	Updates getUpdates4LambdaPrime();
	Terms getAllDistinctTerms(Pred1Dim);
	DLRules getDLRules();
	std::string toString();
};

}	// namespace df
}	// namespace dlvhex

#endif /* _DLVHEX_DF__H */
