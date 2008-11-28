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
 * @brief  A Default of the form [A;B]/[C]. 
 *  
 *  
 */ 

#include <iostream>
#include "Default.h"

namespace dlvhex {
namespace df {

  Default::Default(Pred1Dim prerequisite_, Pred2Dim justifications_, Pred1Dim conclusion_)
    : prerequisite(prerequisite_), justifications(justifications_), conclusion(conclusion_)
  { 
    parent = NULL;
  }
  
  Default::Default(Pred1Dim prerequisite_, Pred2Dim justifications_, Pred1Dim conclusion_, Predicate argument_)
    : prerequisite(prerequisite_), justifications(justifications_), conclusion(conclusion_), argument(argument_)
  { 
    parent = NULL;
  }
  
  unsigned int
  Default::getID()
  {
    return id;
  }
  
  void 
  Default::setID(unsigned int id_) 
  {
    id = id_;
  }
  
  void
  Default::setParent(Defaults* parent_)
  {
    parent = parent_;
  }

  Updates 
  Default::getUpdates(std::string prefix) 
  {
    return conclusion.getUpdates(prefix);
  }

  DLRules
  Default::getFacts_Pi()
  {
    DLRules rules;
    for (std::list<Predicate>::iterator i = conclusion.begin(); i != conclusion.end(); ++i)
      {
	std::string pred_name = i->getPredicateName();
	Terms ts = i->getTerms();
	Predicate pin(PREFIX_IN + pred_name, ts);
	DLRule rin(pin);
	Predicate pp(PREFIX_PRED + pred_name, ts);
	DLRule rp(pp);
	rules.push_back(rin);
	rules.push_back(rp);
      }
    return rules;
  }

  DLRules
  Default::getFacts_Omega_Upsilon()
  {
    DLRules rules;
    for (std::list<Predicate>::iterator i = conclusion.begin(); i != conclusion.end(); ++i)
      {
	std::string pred_name = i->getPredicateName();
	Terms ts = i->getTerms();
	Predicate pin(PREFIX_IN + pred_name, ts);
	DLRule rin(pin);
	rules.push_back(rin);
      }
    return rules;
  }

  DLRules
  Default::additionalDLRules4Conclusion(Pred1Dim& conclusion, Predicate& p_all, std::string prefix)
  {
    DLRules rules;
    for (std::list<Predicate>::iterator i = conclusion.begin(); i != conclusion.end(); ++i)
      {
	Predicate p_ri_h(prefix + i->getSignedPredicateName(), i->getTerms());
	DLRule ri(p_ri_h);
	ri.addPositiveBody(p_all);
	rules.push_back(ri);
	// !!! may be we need to add domain predicates here !!!
      }
    return rules;
  }

  DLAtoms
  Default::getDLBodies(Updates& lambda, Pred1Dim& pred1dim, bool isNAF)
  {
    DLAtoms dlas;
    for (std::list<Predicate>::iterator i = pred1dim.begin(); i != pred1dim.end(); ++i)
      {
	// in mode --cq=off, hence we split each predicate into a single DL atom
	if (isNAF)
	  {
	    DLAtom dla(true, lambda, i->getLiteralName(), i->getTerms());
	    dlas.push_back(dla);
	  }
	else
	  {
	    DLAtom dla(false, lambda, i->getLiteralName(), i->getTerms());
	    dlas.push_back(dla);
	  }
      }
    return dlas;
  }

  DLRules
  Default::getDLRules_Pi(bool cqmode)
  {
    DLRules rules;

    if (prerequisite.size() == 0 && justifications.size() == 0)
      {
	return getFacts_Pi();
      }
    else 
      {
	Updates lambda = parent->getLambda(PREFIX_PRED);
	Updates lambda_prime = parent->getLambda(PREFIX_IN);
	
	Terms adt_prerequisite = prerequisite.getAllDistinctTerms();
	Terms adt_justifications = justifications.getAllDistinctTerms();
	Terms adt_conclusion = conclusion.getAllDistinctTerms();
	Terms all_distinct_terms = adt_prerequisite;
	all_distinct_terms.insertNewTerms(adt_justifications);
	all_distinct_terms.insertNewTerms(adt_conclusion);
	Predicate p_fail("fail");

	// in_conclusion(Terms_conclusion) :- not out_conclusion(Terms_conclusion), dom(eachTerm).
	// out_conclusion(Terms_conclusion) :- not in_conclusion(Terms_conclusion), dom(eachTerm).
	
	Predicate p_in(conclusion.behalf_name(PREFIX_IN, id), adt_conclusion);
	Predicate p_out(conclusion.behalf_name(PREFIX_OUT, id), adt_conclusion);
	DLRule r1(p_in);
	r1.addNegativeBody(p_out);
	DLRule r2(p_out);
	r2.addNegativeBody(p_in);
	
	for (std::list<MTerm>::iterator i = adt_conclusion.begin(); i != adt_conclusion.end(); ++i)
	  {
	    if ((i->isVar()) && (!argument.gotThisTerm(*i)))
	      {
		Predicate p_dom("dom", *i);
		r1.addPositiveBody(p_dom);
		r2.addPositiveBody(p_dom);
	      }
	  }
	if (!argument.getTerms().isEmpty())
	  {
	    r1.addPositiveBody(argument);
	    r2.addPositiveBody(argument);
	  }

	rules.push_back(r1);
	rules.push_back(r2);
	
	// fail :- DL[\lambda';conclusion(Terms_conclusion)](Terms_conclusion), 
	//         out_conclusion(Terms_conclusion), not fail.
	DLRule r3(p_fail);
	if (cqmode)
	  {
	    DLAtom d_lambda_prime(false, lambda_prime, conclusion, adt_conclusion);
	    r3.addDLAtom(d_lambda_prime);
	  }
	else
	  {
	    DLAtoms d_lambda_prime = getDLBodies(lambda_prime, conclusion, false);
	    r3.insertDLAtoms(d_lambda_prime);
	  }

	r3.addPositiveBody(p_out);
	r3.addNegativeBody(p_fail);
	rules.push_back(r3);
	
	// p_conclusion(Terms_conslusion) :-	
	//		DL[\lambda; prerequisite[0](Terms_prerequisite[0]),...,prerequisite[k](Terms_prerequisite[k])](allTerms_prerequisite),
	//		not DL[\lambda'; -justification[0]](allTerms_justification_0),
	//			...
	//		not DL[\lambda'; -justification[i,0](Terms_justification[i,0]) v ... v -justification[i,m](Terms_justification[i,m])]
	//			...
	//		not DL[\lambda'; -justification[m]](allTerms_justification_m),
	//              dom(eachTerm).

	Predicate p_p(conclusion.behalf_name(PREFIX_PRED, id), adt_conclusion);
	DLRule r4(p_p);

	if (cqmode)
	  {
	    DLAtom d_prerequisite(false, lambda, prerequisite, adt_prerequisite);
	    r4.addDLAtom(d_prerequisite);
	  }
	else
	  {
	    DLAtoms d_prerequisites = getDLBodies(lambda, prerequisite, false);
	    r4.insertDLAtoms(d_prerequisites);
	  }

	for (std::list<Pred1Dim>::iterator i = justifications.begin(); i != justifications.end(); ++i)
	  {
	    if (cqmode)
	      {
		Terms adt_justification =  i->getAllDistinctTerms();
		Pred2Dim neg_justification = i->deMorgan();
		DLAtom d_r4_bi(true, lambda_prime, neg_justification, adt_justification);
		r4.addDLAtom(d_r4_bi);
	      }
	    else
	      {
		if (i->size() > 1)
		  {
		    throw PluginError("Meet a conjunctive justification. CQ mode must be turnt ON!!!");
		  }
		else
		  {
		    DLAtom d_justification_n(true, lambda_prime,
					     i->begin()->getNegatedLiteralName(), 
					     i->begin()->getTerms());
		    r4.addDLAtom(d_justification_n);
		  }
	      }
	  }

	for (std::list<MTerm>::iterator i = all_distinct_terms.begin(); i != all_distinct_terms.end(); ++i)
	  {
	    if ((i->isVar()) && (!argument.gotThisTerm(*i)))
	      {
		Predicate p_dom("dom", *i);
		r4.addPositiveBody(p_dom);
	      }	    
	  }
	if (!argument.getTerms().isEmpty())
	{
	  r4.addPositiveBody(argument);
	}
	rules.push_back(r4);

	// in_conclusion_i(Terms_conclusion_i) :- in_conclusion(Terms_conclusion).
	// p_conclusion_i(Terms_conclusion_i)  :- p_conclusion(Terms_conclusion).
	if (conclusion.size() > 1)
	  {
	    DLRules ri = additionalDLRules4Conclusion(conclusion, p_in, PREFIX_IN);
	    DLRules rp = additionalDLRules4Conclusion(conclusion, p_p, PREFIX_PRED);
	    rules.insert(ri);
	    rules.insert(rp);
	  }
	
	// fail :- not DL[\lambda; conclusion[0](Terms[0]),...,conclusion[n](Terms[n])](allTerms_conclusion), 
	//         in_conclusion(allTerms_conclusion), not fail.

	if (cqmode)
	  {
	    DLRule r5(p_fail);
	    DLRule r6(p_fail);
	    DLAtom d_conclusion_p(false, lambda, conclusion, adt_conclusion);
	    DLAtom d_conclusion_n(true, lambda, conclusion, adt_conclusion);
	    r5.addDLAtom(d_conclusion_n);
	    r6.addDLAtom(d_conclusion_p);
	    r5.addPositiveBody(p_in);
	    r5.addNegativeBody(p_fail);
	    r6.addPositiveBody(p_out);
	    r6.addNegativeBody(p_fail);
	    rules.push_back(r5);
	    rules.push_back(r6);
	  }
	else
	  {
	    for (std::list<Predicate>::iterator i = conclusion.begin(); i != conclusion.end(); ++i)
	      {
		DLRule r5(p_fail);
		DLRule r6(p_fail);
		DLAtom d_conclusion_p(false, lambda, i->getLiteralName(), i->getTerms());
		DLAtom d_conclusion_n(true, lambda, i->getLiteralName(), i->getTerms());
		Predicate p_i(PREFIX_IN + i->getSignedPredicateName(), i->getTerms());
		Predicate p_o(PREFIX_OUT + i->getSignedPredicateName(), i->getTerms());
		r5.addDLAtom(d_conclusion_n);
		r6.addDLAtom(d_conclusion_p);
		r5.addPositiveBody(p_i);
		r5.addNegativeBody(p_fail);
		r6.addPositiveBody(p_o);
		r6.addNegativeBody(p_fail);
		rules.push_back(r5);
		rules.push_back(r6);
	      }
	  }
      }	
    return rules;
  }

  DLRules
  Default::getDLRules_Omega(bool cqmode)
  {
    DLRules rules;
    
    if (prerequisite.size() == 0 && justifications.size() == 0)
      {
	return getFacts_Omega_Upsilon();
      }
    else 
      {
	Updates lambda = parent->getLambda(PREFIX_IN);
	Terms adt_prerequisite = prerequisite.getAllDistinctTerms();
	Terms adt_justifications = justifications.getAllDistinctTerms();
	Terms adt_conclusion = conclusion.getAllDistinctTerms();
	Terms all_distinct_terms = adt_prerequisite;
	all_distinct_terms.insertNewTerms(adt_justifications);
	all_distinct_terms.insertNewTerms(adt_conclusion);

	Predicate p_in(conclusion.behalf_name(PREFIX_IN, id), adt_conclusion);
	DLRule r(p_in);
	if (cqmode)
	  {
	    DLAtom d_prerequisite(false, lambda, prerequisite, adt_prerequisite);
	    r.addDLAtom(d_prerequisite);
	  }
	else
	  {
	    DLAtoms d_prerequisites = getDLBodies(lambda, prerequisite, false);
	    r.insertDLAtoms(d_prerequisites);
	  }

	for (std::list<Pred1Dim>::iterator i = justifications.begin(); i != justifications.end(); ++i)
	  {
	    if (cqmode)
	      {
		Terms adt_justification =  i->getAllDistinctTerms();
		Pred2Dim neg_justification = i->deMorgan();
		DLAtom d_bi(true, lambda, neg_justification, adt_justification);
		r.addDLAtom(d_bi);
	      }
	    else
	      {
		if (i->size() > 1)
		  {
		    throw PluginError("Meet a conjunctive justification. CQ mode must be turnt ON!!!");
		  }
		else
		  {
		    DLAtom d_justification_n(true, lambda,
					     i->begin()->getNegatedLiteralName(), 
					     i->begin()->getTerms());
		    r.addDLAtom(d_justification_n);
		  }
	      }
	  }

	for (std::list<MTerm>::iterator i = all_distinct_terms.begin(); i != all_distinct_terms.end(); ++i)
	  {
	    if ((i->isVar()) && (!argument.gotThisTerm(*i)))
	      {
		Predicate p_dom("dom", *i);
		r.addPositiveBody(p_dom);
	      }	    
	  }
	if (!argument.getTerms().isEmpty())
	{
	  r.addPositiveBody(argument);
	}
	rules.push_back(r);

	// in_conclusion_i(Terms_conclusion_i) :- in_conclusion(Terms_conclusion).
	if (conclusion.size() > 1)
	  {
	    DLRules ri = additionalDLRules4Conclusion(conclusion, p_in);
	    rules.insert(ri);
	  }
      }

    return rules;
  }

  DLRules
  Default::getDLRules_Upsilon(bool cqmode)
  {
    DLRules rules;
    if (prerequisite.size() == 0 && justifications.size() == 0)
      {
	return getFacts_Omega_Upsilon();
      }
    else 
      {
	Updates lambda = parent->getLambda(PREFIX_IN);
	Terms adt_prerequisite = prerequisite.getAllDistinctTerms();
	Terms adt_justifications = justifications.getAllDistinctTerms();
	Terms adt_conclusion = conclusion.getAllDistinctTerms();
	Terms all_distinct_terms = adt_prerequisite;
	all_distinct_terms.insertNewTerms(adt_justifications);
	all_distinct_terms.insertNewTerms(adt_conclusion);

	Predicate p_in(conclusion.behalf_name(PREFIX_IN, id), adt_conclusion);
	Predicate p_fail("fail");
	DLRule r_gamma(p_in);
	if (cqmode)
	  {
	    DLAtom d_prerequisite(false, lambda, prerequisite, adt_prerequisite);
	    r_gamma.addDLAtom(d_prerequisite);
	  }
	else
	  {
	    DLAtoms d_prerequisites = getDLBodies(lambda, prerequisite, false);
	    r_gamma.insertDLAtoms(d_prerequisites);
	  }
	
	// Guess the consistency of \beta_{i,j}
	// auxc_\beta_{i,j}(Y_{i,j}) :- not out_auxc_\beta_{i,j}(Y_{i,j}).
	// out_auxc_\beta_{i,j}(Y_{i,j}) :- out_auxc_\beta_{i,j}(Y_{i,j}).
	int j = 0;
	for (std::list<Pred1Dim>::iterator i = justifications.begin(); i != justifications.end(); ++i)
	  {
	    ++j;
	    Terms adt_justification = i->getAllDistinctTerms();
	    Predicate p_cons(i->behalf_name(PREFIX_CONS, id, j), adt_justification);
	    Predicate p_incons(i->behalf_name(PREFIX_INCONS, id, j), adt_justification);
	    r_gamma.addPositiveBody(p_cons);
	    DLRule r1(p_cons);
	    DLRule r2(p_incons);
	    r1.addNegativeBody(p_incons);
	    r2.addNegativeBody(p_cons);

	    // dom predicates for safety conditions
	    for (std::list<MTerm>::iterator t = adt_justification.begin(); t != adt_justification.end(); ++t)
	      {
		if ((t->isVar()) && (!argument.gotThisTerm(*t)))
		  {
		    Predicate p_dom("dom", *t);
		    r1.addPositiveBody(p_dom);
		    r2.addPositiveBody(p_dom);
		  }
	      }
	    if (!argument.getTerms().isEmpty())
	      {
		r1.addPositiveBody(argument);
		r2.addPositiveBody(argument);
	      }

	    DLRule r3(p_fail);
	    DLRule r4(p_fail);
	    r3.addPositiveBody(p_cons);
	    r3.addNegativeBody(p_fail);
	    r4.addPositiveBody(p_incons);
	    r4.addNegativeBody(p_fail);

	    if (cqmode)
	      {
		Pred2Dim neg_justification = i->deMorgan();
		DLAtom d_justification_p(false, lambda, neg_justification, adt_justification);
		DLAtom d_justification_n(true, lambda, neg_justification, adt_justification);
		r3.addDLAtom(d_justification_p);
		r4.addDLAtom(d_justification_n);
	      }
	    else
	      {
		if (i->size() > 1)
		  {
		    throw PluginError("Meet a conjunctive justification. CQ mode must be turnt ON!!!");		
		  }
		else
		  {
		    DLAtom d_justification_p(false, lambda, i->begin()->getNegatedLiteralName(), i->begin()->getTerms());
		    DLAtom d_justification_n(true, lambda, i->begin()->getNegatedLiteralName(), i->begin()->getTerms());
		    r3.addDLAtom(d_justification_p);
		    r4.addDLAtom(d_justification_n);
		  }
	      }

	    rules.push_back(r1);
	    rules.push_back(r2);
	    rules.push_back(r3);
	    rules.push_back(r4);


	  }

// 	for (std::list<MTerm>::iterator i = all_distinct_terms.begin(); i != all_distinct_terms.end(); ++i)
// 	  {
// 	    if ((i->isVar()) && (!argument.gotThisTerm(*i)))
// 	      {
// 		Predicate p_dom("dom", *i);
// 		r_gamma.addPositiveBody(p_dom);
// 	      }	    
// 	  }
// 	if (!argument.getTerms().isEmpty())
// 	{
// 	  r_gamma.addPositiveBody(argument);
// 	}
	
	rules.push_back(r_gamma);
	if (conclusion.size() > 1)
	  {
	    DLRules ri = additionalDLRules4Conclusion(conclusion, p_in);
	    rules.insert(ri);
	  }
      }    

    return rules;
  }
  
  std::string
  Default::toString()
  {
    std::string tmp = "";
    tmp = "[" + prerequisite.toString(mDefault) + ";" + justifications.toString(mDefault) + "]/[" + conclusion.toString(mDefault) + "]";
    if (!argument.getTerms().isEmpty())
      {
	tmp = tmp + "<" + argument.toString() + ">";
      }
    return tmp;
  }

} // namespace df
} // namespace dlvhex
