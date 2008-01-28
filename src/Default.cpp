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

Default::Default(std::vector<Predicate> premise_, std::vector<Predicate> justification_, Predicate conclusion_) : premise(premise_), justification(justification_), conclusion(conclusion_) {
	is_null = false;
	parent = NULL;
}

bool Default::isNULL() {
	return is_null;
}

void Default::setParent(Defaults* parent_) {
	parent = parent_;
}

Predicate Default::getConclusion() {
	return conclusion;
}

Update Default::getUpdate4Lambda() {
	Operator op;
	if (conclusion.isStronglyNegated()) {
		op = UMINUS;
	} else {
		op = UPLUS;
	}
	std::string conclusion_name = conclusion.getPredicateName();
	Update u(conclusion_name, op, PREFIX_PRED + conclusion_name);
	return u;
}

Update Default::getUpdate4LambdaPrime() {
	Operator op;
	if (conclusion.isStronglyNegated()) {
		op = UMINUS;
	} else {
		op = UPLUS;
	}
	std::string conclusion_name = conclusion.getPredicateName();
	Update u(conclusion_name, op, PREFIX_IN + conclusion_name);
	return u;
}

std::string Default::toString() {
	std::string tmp = "[";
	std::vector<Predicate>::iterator pos;

	if (premise.size() > 0) {
		for (pos = premise.begin(); pos != premise.end(); pos++) {
			tmp = tmp + pos->toString() + " & ";
		}
		tmp = tmp.erase(tmp.length()-3);
	}
	tmp += ":";

	if (justification.size() > 0) {
		for (pos = justification.begin(); pos != justification.end(); pos++) {
			tmp = tmp + pos->toString() + ", ";
		}
		tmp = tmp.erase(tmp.length()-2);
	}
	tmp = tmp +  "]/[" + conclusion.toString() + "].\n";

	return tmp;
}

void 
Default::gen_ground_rules(DLRules& rules, 
													const std::string& name_in, const std::string& name_out, 
													Terms& grounded_terms, std::vector<MTerm>& terms, 
													std::vector<std::string>& individuals, 
													std::vector<MTerm>::iterator& pos_t) 
{
	if (pos_t != terms.end()) {
		if (pos_t->isVar()) {
			std::vector<std::string>::iterator pos_i;
			for (pos_i = individuals.begin(); pos_i != individuals.end(); pos_i++) {
				MTerm t(*pos_i);
				grounded_terms.addTerm(t);
				pos_t++;
				gen_ground_rules(rules, name_in, name_out, grounded_terms, terms, individuals, pos_t);
				pos_t--;
				grounded_terms.removeLastTerm();
			}
		}	else 	{
			grounded_terms.addTerm(*pos_t);
			pos_t++;
			gen_ground_rules(rules, name_in, name_out, grounded_terms, terms, individuals, pos_t);
			pos_t--;
			grounded_terms.removeLastTerm();
		}
	} else {
		// Got a grounded rule here, add to "rules"
		Predicate p_in(name_in, grounded_terms);
		Predicate p_out(name_out, grounded_terms);
		DLRule r1(p_in);
		r1.addNegativeBody(p_out);
		rules.addDLRule(r1);
		DLRule r2(p_out);
		r2.addNegativeBody(p_in);
		rules.addDLRule(r2);
	}
}

DLRules Default::getDLRules() {
	DLRules rules;
	
	Updates lambda = parent->getLambda();
	Updates lambda_prime = parent->getLambdaPrime();
	std::string aux;
	std::vector<Predicate>::iterator pos;

	// ----------------transformation for the defaults of the form:----------------
	//
	//	\alpha_i1(X_i1) & ... & \alpha_ik(X_ik) : \beta_i1(Z_i1),...,\beta_in(Z_in)
	//	---------------------------------------------------------------------------
	//																	\gamma_i(Y_i)
	//

	// in_\gamma_i(Y_i) :- not out_\gamma_i(Y_i), dom(Y_i).
	// out_\gamma_i(Y_i) :- not in_\gamma_i(Y_i), dom(Y_i).
	// all rules are grounded wrt individuals
	// This ground step is required by the DL-Safe condition
	std::string name_in = PREFIX_IN + conclusion.getPredicateName();
	std::string name_out = PREFIX_OUT + conclusion.getPredicateName();
	Terms grounded_terms;
	std::vector<MTerm> terms = conclusion.getTerms().getMTerms();
	std::vector<MTerm>::iterator pos_t = terms.begin();
	//std::vector<std::string>::iterator pos_i;
	//std::cout << "Start generate grounded rules" << std::endl;
	//gen_ground_rules(rules, name_in, name_out, grounded_terms, terms, individuals, pos_t);
	Predicate p_in(PREFIX_IN + conclusion.getPredicateName(), conclusion.getTerms());
	Predicate p_out(PREFIX_OUT + conclusion.getPredicateName(), conclusion.getTerms());
	DLRule r1(p_in);
	r1.addNegativeBody(p_out);
	DLRule r2(p_out);
	r2.addNegativeBody(p_in);

	for (pos_t = terms.begin(); pos_t != terms.end(); pos_t++) {
		Predicate p("dom", *pos_t);
		r1.addPositiveBody(p);
		r2.addPositiveBody(p);
	}

	rules.addDLRule(r1);
	rules.addDLRule(r2);

	//Predicate p_r2_h(PREFIX_OUT + conclusion.getPredicateName(), conclusion.getTerms());
	//Predicate p_r2_b(PREFIX_IN + conclusion.getPredicateName(), conclusion.getTerms());
	//DLRule r2(p_r2_h);
	//r2.addNegativeBody(p_r2_b);
	//rules.addDLRule(r2);

	// fail :- DL[\lambda';\gamma_i](Y_i), out_\gamma_i(Y_i), not fail.
	Predicate p_r3_h("fail");
	DLAtom d_r3_b1(lambda_prime, conclusion.getLiteralName(), conclusion.getTerms());
	Predicate p_r3_b2(PREFIX_OUT + conclusion.getPredicateName(), conclusion.getTerms());
	Predicate p_r3_b3("fail");
	DLRule r3(p_r3_h);
	r3.addPositiveDLAtom(d_r3_b1);
	r3.addPositiveBody(p_r3_b2);
	r3.addNegativeBody(p_r3_b3);
	rules.addDLRule(r3);

	// gamma_i^+(Y_i) :-  DL[\lambda;\alpa_i1](X_i1),..., DL[\lambda;\alpa_ik](X_ik), 
	//										not DL[\lambda';-beta_i1](Z_i1),..., not DL[\lambda';-beta_in](Z_in)
	Predicate p_r4_h(PREFIX_PRED + conclusion.getPredicateName(), conclusion.getTerms());
	DLRule r4(p_r4_h);
	for (pos = premise.begin(); pos != premise.end(); pos++) {
		DLAtom d_r4_bp(lambda, pos->getLiteralName(), pos->getTerms());
		r4.addPositiveDLAtom(d_r4_bp);
	}
	for (pos = justification.begin(); pos != justification.end(); pos++) {
		DLAtom d_r4_bn(lambda_prime, pos->getNegatedLiteralName(), pos->getTerms());
		r4.addNegativeDLAtom(d_r4_bn);
	}
	rules.addDLRule(r4);

	// fail :- not DL[\lambda; \gamma_i](Y_i), in_\gamma_i(Y_i), not fail.
	Predicate p_r5_h("fail");
	DLAtom d_r5_b1(lambda, conclusion.getLiteralName(), conclusion.getTerms());
	Predicate p_r5_b2(PREFIX_IN + conclusion.getPredicateName(), conclusion.getTerms());
	Predicate p_r5_b3("fail");
	DLRule r5(p_r5_h);
	r5.addNegativeDLAtom(d_r5_b1);
	r5.addPositiveBody(p_r5_b2);
	r5.addNegativeBody(p_r5_b3);
	rules.addDLRule(r5);

	// fail :- DL[\lambda; \gamma_i](Y_i), out_\gamma_i(Y_i), not fail.
	Predicate p_r6_h("fail");
	DLAtom d_r6_b1(lambda, conclusion.getLiteralName(), conclusion.getTerms());
	Predicate p_r6_b2(PREFIX_OUT + conclusion.getPredicateName(), conclusion.getTerms());
	Predicate p_r6_b3("fail");
	DLRule r6(p_r6_h);
	r6.addPositiveDLAtom(d_r6_b1);
	r6.addPositiveBody(p_r6_b2);
	r6.addNegativeBody(p_r6_b3);
	rules.addDLRule(r6);

	return rules;
}

}	// namespace df
}	// namespace dlvhex
