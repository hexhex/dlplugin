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
 * @file   DefaultParser.cpp
 * @author DAO Tran Minh
 * @date   Tue Dec 18 17:20:24 2007
 * 
 * @brief  A Parser for a Default of the form [A:B]/[C].
 * 
 * 
 */

#include "Registry.h"
#include "DefaultParser.h"

#include <dlvhex/Error.h>
#include <iostream>
#include <fstream>

namespace dlvhex { 
namespace df {

DefaultParser::DefaultParser() 
{ }

std::string 
DefaultParser::get_predicate_name(v_t_nvd::iterator pos) 
{
	std::string pn = "";
	v_t_nvd::iterator pos1;

	if (pos->children.size() == 0) 
	{
		pn = *pos->value.begin();
	} 
	else 
	{
		for (pos1 = pos->children.begin(); pos1 != pos->children.end(); ++pos1) 
		{
			pn += *pos1->value.begin();
		}
	}
	return pn;
}

std::string 
DefaultParser::get_term(v_t_nvd::iterator pos) 
{
	v_t_nvd::iterator pos1;
	std::string t;

	if (pos->children.size() == 0) 
		{
			// single name
			t = *pos->value.begin();
		} 
	else 
		{
			// not a single name
			t = "";
			for (pos1 = pos->children.begin(); pos1 != pos->children.end(); pos1++) 
			{
				t += *pos1->value.begin();
			}
		}
	return t;
}

Terms
DefaultParser::get_terms(v_t_nvd::iterator pos) 
{
	v_t_nvd::iterator pos1;
	std::string t_name;
	Terms ts;

	if (pos->children.size() == 0) 
		{
			// The simplest case ..x..
			t_name = *pos->value.begin();
			MTerm t(t_name);
			ts.addTerm(t);
		}
	else 
		{
			pos1 = pos->children.begin();
			pos1++;
			if (*pos1->value.begin() == ',') 
				{
					// The most complicated case ..XYZ, "ABC"...
					pos1--;
					do 
						{
							t_name = get_term(pos1);
							MTerm t(t_name);
							ts.addTerm(t);
							pos1++;
							if (pos1 == pos->children.end()) break;
							pos1++;
						} 
					while (true);
				}
			else 
				{
					// The second simplest case ..XYZ..			
					t_name = get_term(pos);
					MTerm t(t_name);
					ts.addTerm(t);
				}
		}
	return ts;
}

Predicate
DefaultParser::get_predicate(v_t_nvd::iterator pos) 
{
	v_t_nvd::iterator pos1;
	bool neg = false;
	std::string pn;
	Terms ts;
	
	if (pos->children.size() == 0) 
		{
			// Can be strong negation or a single name
			if (*pos->value.begin() == '-') 
				{
					neg = true;
					pos++;
					pos1 = pos->children.begin();
					pn = get_predicate_name(pos1);
					pos1++;
					pos1++;
					ts = get_terms(pos1);
				} 
			else 
				{
					pn = get_predicate_name(pos);
					pos++;
					pos++;
					ts = get_terms(pos);
				}
		} 
	else 
		{
			// Must be a plural name
			pn = get_predicate_name(pos);
			pos++;	// go to children[2] to get terms
			pos++;
			ts = get_terms(pos);
		}
	Predicate p(neg, pn, ts);
	return p;
}

Pred1Dim
DefaultParser::analyze_conjunction(v_t_nvd& children_) 
{
	Pred1Dim p_list;
	v_t_nvd::iterator pos;

	pos = children_.begin();
	while (pos->children.size() > 0) 
	{
		pos++;
	}
	if (*pos->value.begin() == '&' || *pos->value.begin() == ',') 
		{
			// conjunction or list of literals
			pos = children_.begin();
			do 
				{
					Predicate p = get_predicate(pos->children.begin());
					p_list.push_back(p);
					pos++;
					if (pos == children_.end()) break;
					pos++;
				} 
			while (true);
		} 
	else 
		{
			// single literal
			Predicate p = get_predicate(children_.begin());
			p_list.push_back(p);
		}
	return p_list;
}

Pred2Dim
DefaultParser::analyze_justification(v_t_nvd& children_) 
{
	Pred2Dim c_list;
	v_t_nvd::iterator pos;

	pos = children_.begin();
	while (pos->children.size() > 0) 
	{
		pos++;
	}		
	if (*pos->value.begin() == ',') 
		{
			//std::cout << "list of conjuction" << std::endl;
			pos = children_.begin();
			do 
				{
					Pred1Dim c = analyze_conjunction(pos->children);				
					c_list.push_back(c);
					pos++;
					if (pos == children_.end()) break;
					pos++;
				} 
			while (true);
		} 
	else 
		{
			//std::cout << "only one conjunction" << std::endl;
			Pred1Dim c = analyze_conjunction(children_);
			c_list.push_back(c);
		}
	return c_list;
}

Default
DefaultParser::getDefault(v_t_nvd& branch) 
{
	Pred1Dim premise;
	Pred2Dim justification;
	Pred1Dim conclusion;	

	switch (branch.size())
		{
		case 9:	
			// [A1 & ... & An : B1,...,Bk ]/[C].
			premise = analyze_conjunction(branch[1].children);
			justification = analyze_justification(branch[3].children);
			conclusion = analyze_conjunction(branch[7].children);
			break;
		case 8:
			// [: B1,...,Bk ]/[C].
			if (std::string(branch[1].value.begin(), branch[1].value.end()) == ":") 
				{
					justification = analyze_justification(branch[2].children);
					conclusion = analyze_conjunction(branch[6].children);
				} 
			else 
				{
					// or
					// [A1 & ... & An:]/[C].			
					premise = analyze_conjunction(branch[1].children);
					conclusion = analyze_conjunction(branch[6].children);
				}			
			break;
		case 7:
			// [:]/[C].
			conclusion = analyze_conjunction(branch[5].children);
			break;		
		}
	Default df(premise, justification, conclusion);	
	return df;
}

void
DefaultParser::evaluateDefaults(const iter_t& root, Defaults& dfs)
{
	int n = root->children.size();
	for (int i = 0; i < n; i+= 2)
	{
		Default df = getDefault(root->children[i].children);
		dfs.addDefault(df);		
	}
}

void
DefaultParser::parseInputStream(std::string& filename, std::string& program)
{
  std::ifstream iss;
  iss.open(filename.c_str());
	
  if (!iss.is_open()) 
    {
      throw PluginError("Could not open " + filename);
    }

  parseInputStream(iss, program);
}

void
DefaultParser::parseInputStream(std::istream& iss, std::string& program)
{
	std::string dfcontent = "";
	std::string line;
	default_p df_p;

	while (!iss.eof()) 
	{
		getline(iss, line);
		dfcontent = dfcontent + line + "\n";
	}

	if (dlvhex::dl::Registry::getVerbose() > 1) 
	{
		std::cerr << "Input defaults:" << std::endl;
		std::cerr << dfcontent << std::endl;
	}
	
	boost::spirit::tree_parse_info<> info = boost::spirit::ast_parse(dfcontent.c_str(), df_p, boost::spirit::space_p);
	if (info.full) 
	{
		if (dlvhex::dl::Registry::getVerbose() > 1)
		{
			std::cerr << "Parsing succeeded\n";
		}
		Defaults dfs;
		evaluateDefaults(info.trees.begin(), dfs);
		DLRules dlrs = dfs.getDLRules();
		program = dlrs.toString();
		//std::string program = dlrs.toString();
		//oss << program;
	}
	else 
	{
		std::cout << "Parsing FAILED\n";
	}
}

} // namespace df
} // namespace dlvhex
