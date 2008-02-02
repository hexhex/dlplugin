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

#include "DefaultParser.h"

namespace dlvhex { namespace df {

DefaultParser::DefaultParser() {
}

std::string DefaultParser::get_predicate_name(v_t_nvd::iterator pos) {
	std::string pn = "";
	v_t_nvd::iterator pos1;

	if (pos->children.size() == 0) {
		pn = *pos->value.begin();
	} else {
		for (pos1 = pos->children.begin(); pos1 != pos->children.end(); ++pos1) {
			pn += *pos1->value.begin();
		}
	}
	return pn;
}

std::string DefaultParser::get_term(v_t_nvd::iterator pos) {
	v_t_nvd::iterator pos1;
	std::string t;

	if (pos->children.size() == 0) {
		// single name
		t = *pos->value.begin();
	} else {
		// not a single name
		t = "";
		for (pos1 = pos->children.begin(); pos1 != pos->children.end(); pos1++) {
			t += *pos1->value.begin();
		}
	}
	return t;
}

Terms DefaultParser::get_terms(v_t_nvd::iterator pos) {
	v_t_nvd::iterator pos1;
	std::string t_name;
	Terms ts;

	if (pos->children.size() == 0) {
		// The simplest case ..x..
		t_name = *pos->value.begin();
		MTerm t(t_name);
		ts.addTerm(t);
	} else {
		pos1 = pos->children.begin();
		pos1++;
		if (*pos1->value.begin() == ',') {
			// The most complicated case ..XYZ, "ABC"...
			pos1--;
			do {
				t_name = get_term(pos1);
				MTerm t(t_name);
				ts.addTerm(t);
				pos1++;
				if (pos1 == pos->children.end()) break;
				pos1++;
			} while (true);
		} else {
			// The second simplest case ..XYZ..			
			t_name = get_term(pos);
			MTerm t(t_name);
			ts.addTerm(t);
		}
	}
	return ts;
}

Predicate DefaultParser::get_predicate(v_t_nvd::iterator pos) {
	v_t_nvd::iterator pos1;
	bool neg = false;
	std::string pn;
	Terms ts;
	
	if (pos->children.size() == 0) {
		// Can be strong negation or a single name
		if (*pos->value.begin() == '-') {
			neg = true;
			pos++;
			pos1 = pos->children.begin();
			pn = get_predicate_name(pos1);
			pos1++;
			pos1++;
			ts = get_terms(pos1);
		} else {
			pn = get_predicate_name(pos);
			pos++;
			pos++;
			ts = get_terms(pos);
		}
	} else {
		// Must be a plural name
		pn = get_predicate_name(pos);
		pos++;	// go to children[2] to get terms
		pos++;
		ts = get_terms(pos);
	}
	Predicate p(neg, pn, ts);
	return p;
}

Pred1Dim DefaultParser::analyze_conjunction(v_t_nvd& children_) {
	Pred1Dim p_list;
	v_t_nvd::iterator pos;

	pos = children_.begin();
	while (pos->children.size() > 0) {
		pos++;
	}	if (*pos->value.begin() == '&' || *pos->value.begin() == ',') {
		// conjunction or list of literals
		pos = children_.begin();
		do {
			Predicate p = get_predicate(pos->children.begin());
			p_list.push_back(p);
			pos++;
			if (pos == children_.end()) break;
			pos++;
		} while (true);
	} else {
		// single literal
		Predicate p = get_predicate(children_.begin());
		p_list.push_back(p);
	}
	return p_list;
}

Pred2Dim DefaultParser::analyze_justification(v_t_nvd& children_) {
	Pred2Dim c_list;
	v_t_nvd::iterator pos;

	pos = children_.begin();
	while (pos->children.size() > 0) {
		pos++;
	}	
	
	if (*pos->value.begin() == ',') {
		//std::cerr << "list of conjuction" << std::endl;
		pos = children_.begin();
		do {
			Pred1Dim c = analyze_conjunction(pos->children);				
			c_list.push_back(c);
			pos++;
			if (pos == children_.end()) break;
			pos++;
		} while (true);
	} else {
		//std::cerr << "only one conjunction" << std::endl;
		Pred1Dim c = analyze_conjunction(children_);
		c_list.push_back(c);
	}	

	return c_list;
}

Default DefaultParser::extract_ast(const iter_t& i) {
	Pred1Dim premise;
	Pred2Dim justification;
	Pred1Dim conclusion;	

	switch (i->children.size()) {
		case 7:	
			// [A1 & ... & An : B1,...,Bk ]/[C].
			premise = analyze_conjunction(i->children[1].children);
			justification = analyze_justification(i->children[3].children);
			conclusion = analyze_conjunction(i->children[5].children);
			break;
		case 5:
			// [: B1,...,Bk ]/[C].
			if (std::string(i->children[0].value.begin(), i->children[0].value.end()) == "[:") {
				justification = analyze_justification(i->children[1].children);
				conclusion = analyze_conjunction(i->children[3].children);
			} else {
			// or
			// [A1 & ... & An:]/[C].			
				premise = analyze_conjunction(i->children[1].children);
				conclusion = analyze_conjunction(i->children[3].children);
			}			
			break;
		case 3:
			// [:]/[C].
			conclusion = analyze_conjunction(i->children[1].children);			
			break;		
	}
	Default df(premise, justification, conclusion);
	//std::cerr << df.toString();
	return df;
}

Default DefaultParser::getParsedDefault(const std::string& str_df) {
	default_p df_p;
	if (dlvhex::dl::Registry::getVerbose() > 1) {
		std::cerr << "-------------------------\n";
		std::cerr << "Parsing: " << str_df << std::endl;
	}
	boost::spirit::tree_parse_info<> info = boost::spirit::ast_parse(str_df.c_str(), df_p, boost::spirit::space_p);

	if (info.full) {
		if (dlvhex::dl::Registry::getVerbose() > 1) {
			std::cerr << "-------------------------\n";
			std::cerr << "Parsing succeeded\n";
			std::cerr << "-------------------------\n";
		}
		Default df = extract_ast(info.trees.begin());
		return df;
	} else {
		if (dlvhex::dl::Registry::getVerbose() > 1) {
			std::cerr << "-------------------------\n";
			std::cerr << "Parsing failed\n";
			std::cerr << "stopped at: \": " << info.stop << "\"\n";
			std::cerr << "-------------------------\n";
		}
	}
	return Default::null_default;
}

}} // namespace dlvhex::df
