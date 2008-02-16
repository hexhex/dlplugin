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
 * @file   Unifier.cpp
 * @author DAO Tran Minh
 * @date   Wed Feb 13 16:35:00 2008
 * 
 * @brief  Unification of predicates/predicates, no concern of function symbols.
 * 
 * 
 */

#include "Unifier.h"

namespace dlvhex {
namespace df {

Unifier&
Unifier::operator=(const Unifier& u2)
{
	if (this != &u2)
	{
		unifier = u2.unifier;
		equivalent_classes = u2.equivalent_classes;
	}
	return *this;
}

bool
Unifier::gotThisPair(std::string& first_, std::string& second_)
{
	if (unifier.size() == 0)
	{
		return false;
	}
	MultimapStr::iterator mm_pos;
	for (mm_pos = unifier.begin(); mm_pos != unifier.end(); mm_pos++)
	{
		if (((mm_pos->first.compare(first_) == 0)
				&& (mm_pos->second.compare(second_) == 0))
				|| 
				((mm_pos->first.compare(second_) == 0) 
				&& (mm_pos->first.compare(first_) == 0))
			)
		{
			return true;
		}
	}
	return false;
}

MultimapStr&
Unifier::getUnifier()
{
	return unifier;
}

bool
Unifier::isConsistent(std::string var, std::string cons)
{

	MultimapStr::iterator pos;
	if (unifier.size() > 0)
	{
		for (pos = unifier.begin(); pos != unifier.end(); pos++)
		{
			if ((pos->first.compare(var) == 0) && (pos->second.compare(cons) != 0))
			{
				return false;
			}
		}
	}
	return true;
}

bool
Unifier::isConsistent(Unifier& u2)
{
	MultimapStr::iterator u_pos;
	MultimapStr u2_unifier = u2.unifier;
	for (u_pos = u2_unifier.begin(); u_pos != u2_unifier.end(); u_pos++)
	{
		if ((strVar(u_pos->first) && (!strVar(u_pos->second))) && (!isConsistent(u_pos->first, u_pos->second)))
		{
			return false;
		}					
	}
	return true;
}

std::string
Unifier::firstEquivalentTerm(std::string var)
{
	if (equivalent_classes.size() > 0)
	{
		String2Dim::iterator e_pos;
		for (e_pos = equivalent_classes.begin(); e_pos != equivalent_classes.end(); e_pos++)
		{
			String1Dim equivalent_class = *e_pos;
			String1Dim::iterator s_pos;
			for (s_pos = equivalent_class.begin(); s_pos != equivalent_class.end(); s_pos++)
			{
				if (var.compare(*s_pos) == 0)
				{
					break;
				}
			}
			if (s_pos != equivalent_class.end()) 
			{				
				return *equivalent_class.begin();
			}			
		}
	}
	return "";
}

bool
Unifier::strVar(const std::string var)
{
	return ((var[0] >= 'A') && (var[0] <= 'Z'));
}

bool
Unifier::gotThisVar(String1Dim& eq_class, std::string& varname)
{
	String1Dim::iterator e_pos;
	for (e_pos = eq_class.begin(); e_pos != eq_class.end(); e_pos++)
	{
		if (varname.compare(*e_pos) == 0)
		{
			return true;
		}
	}
	return false;
}

std::string
Unifier::createEquivalentClass(std::string var)
{
	bool appeared = false;
	MultimapStr::iterator m_pos;
	String1Dim new_eq_class;
	int e_pos;
	new_eq_class.push_back(var);
	e_pos = 0;
	

	while (e_pos < new_eq_class.size())
	{
		std::string varname = new_eq_class[e_pos];
		for (m_pos = unifier.begin(); m_pos != unifier.end(); m_pos++)
		{
			std::string fst = m_pos->first;
			std::string scd = m_pos->second;

			if ((varname.compare(fst) == 0) && (!gotThisVar(new_eq_class, scd)))
			{
				appeared = true;
				if (!strVar(scd))
				{
					new_eq_class.insert(new_eq_class.begin(), scd);
				}
				else
				{
					new_eq_class.push_back(scd);
				}
			}
			
			if ((varname.compare(scd) == 0) && (!gotThisVar(new_eq_class, fst)))
			{
				appeared = true;
				if (!strVar(fst))
				{
					new_eq_class.insert(new_eq_class.begin(), fst);
				}
				else
				{
					new_eq_class.push_back(fst);
				}
			}
		}
		e_pos++;
	}
	
	if (appeared)
	{			
		equivalent_classes.push_back(new_eq_class);
		return (*new_eq_class.begin());
	}
	return "_";
}

std::string
Unifier::unifiedValue(std::string var, bool keep_old_name)
{
	std::string u_var;
	u_var = firstEquivalentTerm(var);
	if (u_var.compare("") == 0)
	{
		u_var = createEquivalentClass(var);
	}
	if ((keep_old_name) && (u_var.compare("_") == 0))
	{
		u_var = var;
	}
	return u_var;
}

}	// namespace df
}	// namespace dlvhex
