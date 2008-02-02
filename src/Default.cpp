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
 * @file   Default.cpp
 * @author DAO Tran Minh
 * @date   Tue Dec 18 17:20:24 2007
 * 
 * @brief  A Default of the form [A:B]/[C].
 * 
 * 
 */

#include "Default.h"

namespace dlvhex {
namespace df {

Default Default::null_default(true);

Default::Default(bool is_null_) : is_null(is_null_) {
}

Default::Default(Pred1Dim premise_, Pred2Dim justification_, Pred1Dim conclusion_) : premise(premise_), justification(justification_), conclusion(conclusion_) {
	is_null = false;
	parent = NULL;
}

bool Default::isNULL() {
	return is_null;
}

void Default::setID(unsigned int id_) {
	id = id_;
}

void Default::setParent(Defaults* parent_) {
	parent = parent_;
}

Pred1Dim Default::getConclusion() {
	return conclusion;
}

Updates Default::getUpdates4Lambda() {
	Pred1Dim::iterator pos;
	Operator op;
	Updates us;
	for (pos = conclusion.begin(); pos != conclusion.end(); pos++) {
		if (pos->isStronglyNegated()) {
			op = UMINUS;
		} else {
			op = UPLUS;
		}
		std::string conclusion_name = pos->getPredicateName();
		Update u(conclusion_name, op, PREFIX_PRED + conclusion_name);
		us.addUpdate(u);
	}
	return us;
}

Updates Default::getUpdates4LambdaPrime() {
	Pred1Dim::iterator pos;
	Operator op;
	Updates us;

	for (pos = conclusion.begin(); pos != conclusion.end(); pos++) {
		if (pos->isStronglyNegated()) {
			op = UMINUS;
		} else {
			op = UPLUS;
		}
		std::string conclusion_name = pos->getPredicateName();
		Update u(conclusion_name, op, PREFIX_IN + conclusion_name);
		us.addUpdate(u);
	}
	return us;
}

// Get all variables of a conjunction
// which is necessary in generating rules 
// for a conjunction.
Terms Default::getAllDistinctTerms(Pred1Dim& c) {
	Terms ts;
	Pred1Dim::iterator c_pos;
	for (c_pos = c.begin(); c_pos != c.end(); c_pos++) {
		Terms t1 = c_pos->getTerms();
		ts.insertNewTerms(t1);
	}
	return ts;
}

std::string Default::toString() {
	std::string tmp = "[";
	Pred1Dim::iterator pos1;
	Pred2Dim::iterator pos2;

	if (premise.size() > 0) {
		for (pos1 = premise.begin(); pos1 != premise.end(); pos1++) {
			tmp = tmp + pos1->toString() + " & ";
		}
		tmp = tmp.erase(tmp.length()-3);
	}
	tmp += ":";

	if (justification.size() > 0) {
		for (pos2 = justification.begin(); pos2 != justification.end(); pos2++) {
			Pred1Dim justif = *pos2;
			for (pos1 = justif.begin(); pos1 != justif.end(); pos1++) {
				tmp = tmp + pos1->toString() + " & ";
			}
			tmp = tmp.erase(tmp.length()-3);
			tmp += ", ";
		}
		tmp = tmp.erase(tmp.length()-2);
	}
	tmp += "]/[";

	for (pos1 = conclusion.begin(); pos1 != conclusion.end(); pos1++) {
		tmp = tmp + pos1->toString() + " & ";
	}
	tmp = tmp.erase(tmp.length()-3);
	tmp += "]\n";

	return tmp;
}

DLRules Default::getDLRules() {
	//
	//		\premise[0] \land\ldots\land \premise[k] : ..., \justification[i,0] \land\ldots\land \justification[i,m],...
	//		----------------------------------------------------------------------------------------------------------
	//								conclusion[0] \land\ldots\land \conclusion[n]
	//
	DLRules rules;
	Updates lambda = parent->getLambda();
	Updates lambda_prime = parent->getLambdaPrime();
	std::string aux;

	Terms all_distinct_terms_conclusion = getAllDistinctTerms(conclusion);
	std::stringstream strid;
	strid << id;
	// all_in_def_id(allTerms_conclusion) :- not out_def_id(allTerms_conclusion), dom(eachTerm).
	Predicate p_r1_h("all_in_def_" + strid.str(), all_distinct_terms_conclusion);
	Predicate p_r1_b("out_def_" + strid.str(), all_distinct_terms_conclusion);
	DLRule r1(p_r1_h);	
	r1.addNegativeBody(p_r1_b);

	// out_def_id(allTerms_conclusion) :- not all_in_def_id(allTerms_conclusion), dom(eachTerm).
	DLRule r2(p_r1_b);
	r2.addNegativeBody(p_r1_h);

	std::vector<MTerm> terms;
	std::vector<MTerm>::iterator t_pos;
	terms = all_distinct_terms_conclusion.getMTerms();
	for (t_pos = terms.begin(); t_pos != terms.end(); t_pos++) {
		Predicate p_dom("dom", *t_pos);
		r1.addPositiveBody(p_dom);
		r2.addPositiveBody(p_dom);
	}

	rules.addDLRule(r1);
	rules.addDLRule(r2);

	// ...
	// in_conclusion_i(Y_i) :- all_in_def_id(Y).
	// project Y to Y_i and let all other become '_'
	// ...
	Pred1Dim::iterator c_pos;
	for (c_pos = conclusion.begin(); c_pos != conclusion.end(); c_pos++) {
		Terms projected_terms = all_distinct_terms_conclusion.projectTo(c_pos->getTerms());
		Predicate p_ri_h(c_pos->isStronglyNegated(), "in_" + c_pos->getPredicateName(), c_pos->getTerms());
		Predicate p_ri_b(c_pos->isStronglyNegated(), "all_in_def_" + strid.str(), projected_terms);
		DLRule ri(p_ri_h);
		ri.addPositiveBody(p_ri_b);
		rules.addDLRule(ri);
	}

	// fail :- DL[\lambda';conclusion[0](Terms[0]),...,conclusion[n](Terms[n])](allTerms_conclusion), out_def_id(allTerms_conclusion), not fail.
	Predicate p_fail("fail");
	Pred2Dim ucq_conclusion;
	ucq_conclusion.push_back(conclusion);
	DLAtom d_r3_b1(lambda_prime, ucq_conclusion, all_distinct_terms_conclusion);
	Predicate p_r3_b2("out_def_" + strid.str(), all_distinct_terms_conclusion);
	DLRule r3(p_fail);
	r3.addPositiveDLAtom(d_r3_b1);
	r3.addPositiveBody(p_r3_b2);
	r3.addNegativeBody(p_fail);
	rules.addDLRule(r3);

	// all_p_def_id(allTerms_conslusion) :-	
	//		DL[\lambda; premise[0](Terms_premise[0]),...,premise[k](Terms_premise[k])](allTerms_premise),
	//		not DL[\lambda'; -justification[0]](allTerms_justification_0),
	//			...
	//		not DL[\lambda'; -justification[i,0](Terms_justification[i,0]) v ... v -justification[i,m](Terms_justification[i,m])]
	//			...
	//		not DL[\lambda'; -justification[m]](allTerms_justification_m).
	Predicate p_r4_h("all_p_def_" + strid.str(), all_distinct_terms_conclusion);
	Pred2Dim ucq_premise;
	ucq_premise.push_back(premise);
	Terms all_distinct_terms_premise = getAllDistinctTerms(premise);
	DLAtom d_r4_b1(lambda, ucq_premise, all_distinct_terms_premise);
	DLRule r4(p_r4_h);
	r4.addPositiveDLAtom(d_r4_b1);
	Pred2Dim::iterator j_pos;
	for (j_pos = justification.begin(); j_pos != justification.end(); j_pos++) {
		Pred1Dim one_justification = *j_pos;
		Pred2Dim ucq_justification;
		Pred1Dim::iterator pos;
		for (pos = one_justification.begin(); pos != one_justification.end(); pos++) {
			Predicate p_justification(!pos->isStronglyNegated(), pos->getPredicateName(), pos->getTerms());
			Pred1Dim cq_justification;
			cq_justification.push_back(p_justification);
			ucq_justification.push_back(cq_justification);
		}
		Terms all_distinct_terms_justification = getAllDistinctTerms(one_justification);
		DLAtom d_r4_bi(lambda_prime, ucq_justification, all_distinct_terms_justification);
		r4.addNegativeDLAtom(d_r4_bi);
	}
	rules.addDLRule(r4);
	
	// conclusion_p[0](Terms_conclusion[0]) :- all_p_def_id(allTerms_conclusion).
	// ...
	// conclusion_p[n](Terms_conclusion[n]) :- all_p_def_id(allTerms_conclusion).
	for (c_pos = conclusion.begin(); c_pos != conclusion.end(); c_pos++) {
		Terms projected_terms = all_distinct_terms_conclusion.projectTo(c_pos->getTerms());
		Predicate p_ri_h(c_pos->isStronglyNegated(), PREFIX_PRED + c_pos->getPredicateName(), c_pos->getTerms());
		Predicate p_ri_b(c_pos->isStronglyNegated(), "all_p_def_" + strid.str(), projected_terms);
		DLRule ri(p_ri_h);
		ri.addPositiveBody(p_ri_b);
		rules.addDLRule(ri);
	}

	// fail :- not DL[\lambda; conclusion[0](Terms[0]),...,conclusion[n](Terms[n])](allTerms_conclusion), all_in_def_id(allTerms_conclusion), not fail.
	DLAtom d_r5_b(lambda, ucq_conclusion, all_distinct_terms_conclusion);
	DLRule r5(p_fail);
	r5.addNegativeDLAtom(d_r5_b);
	r5.addPositiveBody(p_r1_h);
	r5.addNegativeBody(p_fail);
	rules.addDLRule(r5);

	// fail :- not DL[\lambda; conclusion[0](Terms[0]),...,conclusion[n](Terms[n])](allTerms_conclusion), out_def_id(allTerms_conclusion), not fail.
	DLRule r6(p_fail);
	r6.addPositiveDLAtom(d_r5_b);
	r6.addPositiveBody(p_r1_b);
	r6.addNegativeBody(p_fail);
	rules.addDLRule(r6);

	return rules;
}

}	// namespace df
}	// namespace dlvhex
