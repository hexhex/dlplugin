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

Terms::Terms() 
{ }

bool 
Terms::isEmpty() 
{
	return (terms.size() == 0);
}

bool 
Terms::gotThisTerm(MTerm& t) 
{
	std::vector<MTerm>::iterator pos;
	for (pos = terms.begin(); pos != terms.end(); pos++) 
		{
			if (*pos == t)
			{
				return true;
			}
		}
	return false;
}

ComparisonResult
Terms::compareTo(Terms& ts2)
{
	if (terms.size() != ts2.terms.size())
	{
		return NOT_COMPARABLE;
	}
	int count1 = 0;
	int count2 = 0;
	std::vector<MTerm>::iterator t_pos;
	std::vector<MTerm>::iterator t_pos2 = ts2.terms.begin();
	for (t_pos = terms.begin(); t_pos != terms.end(); t_pos++)
	{
		switch (t_pos->isUnifiable(*t_pos2))
		{
			case VAR1CONST2:				
				if (count2 > 0)
				{
					return NOT_COMPARABLE;
				}
				count1++;
				break;
			case VAR2CONST1:
				if (count1 > 0)
				{
					return NOT_COMPARABLE;
				}
				count2++;
				break;
			case NOT_UNIFIABLE:
				return NOT_COMPARABLE;				
		}
		t_pos2++;
	}
	if (count1 > 0)
	{
		return MORE_GENERAL;
	}
	return LESS_GENERAL;
}

std::vector<MTerm> 
Terms::getMTerms() 
{
	return terms;
}

Terms&
Terms::operator=(const Terms& ts2)
{
	if (this != &ts2)
	{
		std::vector<MTerm> ts2terms = ts2.terms;
		std::vector<MTerm>::iterator pos;
		terms.clear();
		for (pos = ts2terms.begin(); pos != ts2terms.end(); pos++)
		{
			terms.push_back(*pos);
		}
	}
	return *this;
}

void Terms::addTerm(MTerm term_) 
{
	terms.push_back(term_);
}

// insert all the terms which are in t1 and not in (*this*) to (*this*)
void 
Terms::insertNewTerms(Terms& t1) 
{
	std::vector<MTerm>::iterator pos;
	for (pos = t1.terms.begin(); pos != t1.terms.end(); pos++)
		{
			if (!gotThisTerm(*pos)) 
			{
				addTerm(*pos);
			}
		}
}

// Project this set of terms (*this*) to another set of term (t1)
// All terms which are not in t1 will be set to "_"
Terms 
Terms::projectTo(Terms& t1) 
{
	Terms ts;
	MTerm anonymous("_");
	std::vector<MTerm>::iterator pos;
	for (pos = terms.begin(); pos != terms.end(); pos++) 
	{
		if (!t1.gotThisTerm(*pos)) 
		{
			ts.addTerm(anonymous);
		} 
		else 
		{
			ts.addTerm(*pos);
		}
	}
	return ts;
}

Unifier
Terms::isUnifiable(Terms& ts2)
{
	Unifier uni_ts;
	if (terms.size() != ts2.terms.size())
	{		
		return uni_ts;
	}
	std::vector<MTerm> ts2terms = ts2.terms;
	std::vector<MTerm>::iterator t_pos;
	std::vector<MTerm>::iterator t_pos2 = ts2terms.begin();
	for (t_pos = terms.begin(); t_pos != terms.end(); t_pos++)
	{
		UnificationResult ur = t_pos->isUnifiable(*t_pos2);
		if (ur != NOT_UNIFIABLE)
		{
			std::string t1 = t_pos->toString();
			std::string t2 = t_pos2->toString();
			if (!uni_ts.gotThisPair(t1, t2))
			{
				// Check for consistency first
				// The unification is inconsistent iff there exist 2 pairs <X,a> and <X,b>
				switch (ur)
				{
					case BOTH_VARS:
						uni_ts.getUnifier().insert(std::make_pair(t1, t2));
						break;
					case VAR1CONST2:
						if (uni_ts.isConsistent(t1, t2)) 
						{
							uni_ts.getUnifier().insert(std::make_pair(t1, t2));
						}
						break;
					case VAR2CONST1:
						if (uni_ts.isConsistent(t2, t1)) 
						{
							uni_ts.getUnifier().insert(std::make_pair(t2, t1));		
						}
						break;
					case BOTH_CONSTS:
						uni_ts.getUnifier().insert(std::make_pair(t1, t2));
						break;
				}				
			}
			t_pos2++;
		}
		else
		{
			uni_ts.getUnifier().clear();
			return uni_ts;
		}
	}
	return uni_ts;
}

Terms
Terms::unify(Unifier& unifier, bool keep_old_name)
{
	Terms ts;
	std::vector<MTerm>::iterator t_pos;
	for (t_pos = terms.begin(); t_pos != terms.end(); t_pos++)
	{
		if (t_pos->isVar())
		{
			std::string str_new = unifier.unifiedValue(t_pos->toString(), keep_old_name);
			MTerm t_new(str_new);
			ts.addTerm(t_new);
		}
		else
		{
			ts.addTerm(*t_pos);
		}
	}
	return ts;
}

void
Terms::rename_terms(std::string& str_id)
{
	std::vector<MTerm>::iterator t_pos;
	for (t_pos = terms.begin(); t_pos != terms.end(); t_pos++)
	{
		if (t_pos->isVar())
		{
			t_pos->setName(t_pos->toString() + str_id);
		}
	}
}

std::string 
Terms::toString() 
{
	std::string tmp = "";

	if (terms.size() > 0) 
		{
			std::vector<MTerm>::iterator pos = terms.begin();
			tmp = terms.begin()->toString();
			pos = terms.begin();
			++pos;
			for (; pos != terms.end(); pos++) 
				{
					tmp += ", " + pos->toString();
				}
		}
	return tmp;
}

}	// namespace df
}	// namespace dlvhex
