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
 * @file   DefaultParser.h
 * @author DAO Tran Minh
 * @date   Tue Dec 18 17:20:24 2007
 * 
 * @brief  A Parser for a Default of the form [A:B]/[C].
 * 
 * 
 */

#ifndef _DLVHEX_DF_DEFAULT_PARSER_H
#define _DLVHEX_DF_DEFAULT_PARSER_H

#include <boost/spirit/core.hpp>
#include <boost/spirit/tree/ast.hpp>
#include <boost/spirit/tree/tree_to_xml.hpp>
#include <boost/spirit/tree/parse_tree.hpp>
#include "Default.h"
#include "Registry.h"

typedef char const*																											iterator_t;
typedef boost::spirit::tree_match<iterator_t>													parse_tree_match_t;
typedef parse_tree_match_t::tree_iterator															iter_t;
typedef boost::spirit::node_val_data<iterator_t, boost::spirit::nil_t>	nvd;
typedef boost::spirit::tree_node<nvd >																	t_nvd;
typedef std::vector<t_nvd , std::allocator<t_nvd > >										v_t_nvd;

namespace dlvhex { namespace df {

struct default_p : boost::spirit::grammar<default_p> {
	template <typename ScannerT>
	struct definition {
		definition(default_p const& self) {
			predicate_name = ( boost::spirit::range_p('a', 'z') | boost::spirit::range_p('A', 'Z') )
										>> *( boost::spirit::range_p('a', 'z') 
											| boost::spirit::range_p('A', 'Z') 
											| boost::spirit::range_p('0', '9') 
											| boost::spirit::ch_p('_'));

			constant		=	boost::spirit::range_p('a', 'z') 
									>> *( boost::spirit::range_p('a', 'z') 
										| boost::spirit::range_p('A', 'Z') 
										| boost::spirit::range_p('0', '9') 
										| boost::spirit::ch_p('_'))
										|	boost::spirit::ch_p('\"')
									>> +( boost::spirit::range_p('a', 'z') 
										| boost::spirit::range_p('A', 'Z')
										| boost::spirit::range_p('0', '9')
										| boost::spirit::ch_p('_'))
									>> 	boost::spirit::ch_p('\"')
										|	boost::spirit::int_p;

			variable		=	boost::spirit::range_p('A', 'Z') 
									>> *( boost::spirit::range_p('a', 'z') 
										| boost::spirit::range_p('A', 'Z') 
										| boost::spirit::range_p('0', '9') 
										| boost::spirit::ch_p('_'));

			term				=	boost::spirit::ch_p('_')
									|	constant
									|	variable;

			terms			=	term
									>> *(boost::spirit::ch_p(',') >> term);

			predicate		=	predicate_name
									>> boost::spirit::ch_p('(')
									>> terms
									>> boost::spirit::ch_p(')');

			lit					=	boost::spirit::ch_p('-')
									>> predicate								
									|	predicate;

			conjunction	=	lit
									>> *(boost::spirit::ch_p('&') >> lit);

			justifications	=	conjunction
									>> *(boost::spirit::ch_p(',') >> conjunction);

			default_		=	boost::spirit::ch_p('[')
									>> conjunction
									>> boost::spirit::ch_p(':')
									>> justifications
									>> boost::spirit::str_p("]/[")
									>> conjunction
									>> boost::spirit::str_p("].")
									|	boost::spirit::str_p("[:")
									>> justifications
									>> boost::spirit::str_p("]/[")
									>> conjunction
									>> boost::spirit::str_p("].")
									|	boost::spirit::ch_p('[')
									>> conjunction
									>> boost::spirit::str_p(":]/[")
									>> conjunction
									>> boost::spirit::str_p("].")
									|	boost::spirit::str_p("[:]/[")
									>> conjunction
									>> boost::spirit::str_p("].");
	}

	boost::spirit::rule<ScannerT> predicate_name, constant, variable, term, terms, predicate, lit, conjunction, justifications, default_;

	boost::spirit::rule<ScannerT> const& start() const {
		return default_;
	}
};
};

class DefaultParser {
private:
	std::string get_predicate_name(v_t_nvd::iterator);
	std::string get_term(v_t_nvd::iterator);
	Terms get_terms(v_t_nvd::iterator);
	Predicate get_predicate(v_t_nvd::iterator);
	Pred1Dim analyze_conjunction(v_t_nvd&);
	Pred2Dim analyze_justification(v_t_nvd&);
	Default extract_ast(const iter_t&);
public:
	DefaultParser();
	Default getParsedDefault(const std::string&);
};

}} // namespace dlvhex::df

#endif // _DLVHEX_DF_DEFAULT_PARSER_H
