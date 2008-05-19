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

#include <dlvhex/Error.h>

namespace dlvhex {
namespace df {

Default Default::null_default(true);

Default::Default(bool is_null_) 
: is_null(is_null_) 
{ }

Default::Default(Pred1Dim premise_, Pred2Dim justification_, Pred1Dim conclusion_) 
  : premise(premise_), justification(justification_), conclusion(conclusion_) 
{
 is_null = false;
 parent = NULL;
}

unsigned int
Default::getID()
{
 return id;
}

bool 
Default::isNULL() 
{
 return is_null;
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

Pred1Dim 
Default::getConclusion() 
{
  return conclusion;
}

Pred2Dim
Default::getJustifications()
{
  return justification;
}

Updates 
Default::getUpdates4Lambda() 
{
  Pred1Dim::iterator pos;
  Operator op;
  Updates us;
  
  for (pos = conclusion.begin(); pos != conclusion.end(); pos++) 
    {
      if (pos->isStronglyNegated()) 
	{
	  op = UMINUS;
	} 
      else 
	{
	  op = UPLUS;
	}			
      Update u(pos->getPredicateNameWithNS(), op, PREFIX_PRED + pos->getSignedPredicateName());
      us.addUpdate(u);
    }
  return us;
}

Updates 
Default::getUpdates4LambdaPrime() 
{
  Pred1Dim::iterator pos;
  Operator op;
  Updates us;
  
  for (pos = conclusion.begin(); pos != conclusion.end(); pos++) 
    {
      if (pos->isStronglyNegated()) 
	{
	  op = UMINUS;
	} 
      else 
	{
	  op = UPLUS;
	}			
      Update u(pos->getPredicateNameWithNS(), op, PREFIX_IN + pos->getSignedPredicateName());
      us.addUpdate(u);
    }
  return us;
}

// Get all variables of a conjunction
// which is necessary in generating rules 
// for a conjunction.
Terms 
Default::getAllDistinctTerms(Pred1Dim c) 
{
  Terms ts;
  Pred1Dim::iterator c_pos;
  for (c_pos = c.begin(); c_pos != c.end(); c_pos++) 
    {
      Terms t1 = c_pos->getTerms();
      ts.insertNewTerms(t1);
    }
  return ts;
}

Terms 
Default::getAllDistinctTerms(Pred2Dim j) 
{
  Terms ts;
  Pred2Dim::iterator j_pos;
  for (j_pos = j.begin(); j_pos != j.end(); ++j_pos)
    {
      Pred1Dim c = *j_pos;    
      Pred1Dim::iterator c_pos;
      for (c_pos = c.begin(); c_pos != c.end(); ++c_pos) 
	{
	  Terms t1 = c_pos->getTerms();
	  ts.insertNewTerms(t1);
	}
    }
  return ts;
}

std::string 
Default::toString() 
{
  std::string tmp = "[";
  Pred1Dim::iterator pos1;
  Pred2Dim::iterator pos2;
  
  if (premise.size() > 0) 
    {
      for (pos1 = premise.begin(); pos1 != premise.end(); pos1++) 
	{
	  tmp = tmp + pos1->toString() + " & ";
	}
      tmp = tmp.erase(tmp.length()-3);
    }
  tmp += ":";
  
  if (justification.size() > 0) 
    {
      for (pos2 = justification.begin(); pos2 != justification.end(); pos2++) 
	{
	  Pred1Dim justif = *pos2;
	  for (pos1 = justif.begin(); pos1 != justif.end(); pos1++) 
	    {
	      tmp = tmp + pos1->toString() + " & ";
	    }
	  tmp = tmp.erase(tmp.length()-3);
	  tmp += ", ";
	}
      tmp = tmp.erase(tmp.length()-2);
    }
  tmp += "]/[";
  
  for (pos1 = conclusion.begin(); pos1 != conclusion.end(); pos1++) 
    {
      tmp = tmp + pos1->toString() + " & ";
    }
  tmp = tmp.erase(tmp.length()-3);
  tmp += "]\n";
  
  return tmp;
}

Predicate&
Default::getAllInPred()
{
  return all_in_p;
}

DLRules Default::getFacts()
{
  DLRules rules;
  Pred1Dim::iterator c_pos;

  for (c_pos = conclusion.begin(); c_pos != conclusion.end(); c_pos++)
    {
      std::string pred_name = c_pos->getPredicateName();
      Terms ts = c_pos->getTerms();
      Predicate pin(PREFIX_IN + pred_name, ts);
      DLRule rin(pin);
      Predicate pp(PREFIX_PRED + pred_name, ts);
      DLRule rp(pp);
      rules.addDLRule(rin);
      rules.addDLRule(rp);
    }

  return rules;
}

DLRules Default::getDLRules(bool cqmode) 
{
  //
  //		\premise[0] \land\ldots\land \premise[k] : ..., \justification[i,0] \land\ldots\land \justification[i,m],...
  //		----------------------------------------------------------------------------------------------------------
  //								conclusion[0] \land\ldots\land \conclusion[n]
  //
  DLRules rules;
  
  if (premise.size() == 0 && justification.size() == 0)
    {
      return getFacts();
    }
  else 
    {
      Updates lambda = parent->getLambda();
      Updates lambda_prime = parent->getLambdaPrime();
      std::string aux;
      
      Terms all_distinct_terms_conclusion = getAllDistinctTerms(conclusion);
      Terms all_distinct_terms_justifications = getAllDistinctTerms(justification);
      Terms all_distinct_terms_premise = getAllDistinctTerms(premise);
      std::stringstream strid;
      strid << id;
      // all_in_def_id(allTerms_conclusion) :- not out_def_id(allTerms_conclusion), dom(eachTerm).
      Predicate p_r1_h(PREFIX_ALL_IN + strid.str(), all_distinct_terms_conclusion);
      Predicate p_r1_b(PREFIX_OUT_DEF + strid.str(), all_distinct_terms_conclusion);
      DLRule r1(p_r1_h);	
      r1.addNegativeBody(p_r1_b);
      
      // cache the all_in_def_id and out_def_id predicates
      // for building forcing rules
      all_in_p = p_r1_h;
      
      // out_def_id(allTerms_conclusion) :- not all_in_def_id(allTerms_conclusion), dom(eachTerm).
      DLRule r2(p_r1_b);
      r2.addNegativeBody(p_r1_h);

      std::vector<MTerm> terms;
      std::vector<MTerm>::iterator t_pos;	
      terms = all_distinct_terms_conclusion.getMTerms();
      for (t_pos = terms.begin(); t_pos != terms.end(); t_pos++) 
	{
	  if (t_pos->isVar()) 
	    {
	      Predicate p_dom("dom", *t_pos);
	      r1.addPositiveBody(p_dom);
	      r2.addPositiveBody(p_dom);		
	    }
	}
      rules.addDLRule(r1);
      rules.addDLRule(r2);
      
      // ...
      // in_conclusion_i(Y_i) :- all_in_def_id(Y).
      // project Y to Y_i and let all other become '_'
      // ...
      Pred1Dim::iterator c_pos;
      for (c_pos = conclusion.begin(); c_pos != conclusion.end(); c_pos++) 
	{
	  Terms projected_terms = all_distinct_terms_conclusion.projectTo(c_pos->getTerms());
	  Predicate p_ri_h(PREFIX_IN + c_pos->getSignedPredicateName(), c_pos->getTerms());
	  Predicate p_ri_b(PREFIX_ALL_IN + strid.str(), projected_terms);
	  DLRule ri(p_ri_h);
	  ri.addPositiveBody(p_ri_b);
	  rules.addDLRule(ri);
	}
      
      // fail :- DL[\lambda';conclusion[0](Terms[0]),...,conclusion[n](Terms[n])](allTerms_conclusion), out_def_id(allTerms_conclusion), not fail.
      Predicate p_fail("fail");
      DLRule r3(p_fail);
      Pred2Dim ucq_conclusion;
      ucq_conclusion.push_back(conclusion);
      if (cqmode)
	{	  
	  DLAtom d_r3_b1(lambda_prime, ucq_conclusion, all_distinct_terms_conclusion);
	  r3.addPositiveDLAtom(d_r3_b1);
	}
      else
	{
	  // Have to split all conclusion's literal to single DLAtoms
	  Pred1Dim::iterator c_pos;
	  for (c_pos = conclusion.begin(); c_pos != conclusion.end(); ++c_pos)
	    {
	      DLAtom d_r3_b1i(lambda_prime, c_pos->getLiteralName(), c_pos->getTerms());
	      r3.addPositiveDLAtom(d_r3_b1i);
	    }
	}    
      Predicate p_r3_b2(PREFIX_OUT_DEF + strid.str(), all_distinct_terms_conclusion);
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
      Predicate p_r4_h(PREFIX_ALL_PRED + strid.str(), all_distinct_terms_conclusion);
      DLRule r4(p_r4_h);
//       if (premise.size() > 0) 
// 	{
// 	  Pred2Dim ucq_premise;
// 	  ucq_premise.push_back(premise);
// 	  Terms all_distinct_terms_premise = getAllDistinctTerms(premise);
// 	  DLAtom d_r4_b1(lambda, ucq_premise, all_distinct_terms_premise);			
// 	  r4.addPositiveDLAtom(d_r4_b1);
// 	}
      // If we don't use ucq here:
      if (premise.size() > 0)
	{
	  Pred1Dim::iterator p_pos;
	  for (p_pos = premise.begin(); p_pos != premise.end(); ++p_pos)
	    {
	      if (cqmode)
		{
		  DLAtom d_r4_bi(lambda_prime, *p_pos, p_pos->getTerms());
		  r4.addPositiveDLAtom(d_r4_bi);
		}
	      else
		{
		  DLAtom d_r4_bi(lambda_prime, p_pos->getLiteralName(), p_pos->getTerms());
		  r4.addPositiveDLAtom(d_r4_bi);
		}
	    }
	}

      if (justification.size() > 0)
	{
	  Pred2Dim::iterator j_pos;
	  for (j_pos = justification.begin(); j_pos != justification.end(); j_pos++) 
	    {
	      Pred1Dim one_justification = *j_pos;
	      if (cqmode)
		{
		  Pred2Dim ucq_justification;
		  Pred1Dim::iterator pos;
		  for (pos = one_justification.begin(); pos != one_justification.end(); pos++) 
		    {
		      Predicate p_justification(!pos->isStronglyNegated(), pos->getPredicateName(), pos->getTerms());
		      Pred1Dim cq_justification;
		      cq_justification.push_back(p_justification);
		      ucq_justification.push_back(cq_justification);
		    }
		  Terms all_distinct_terms_justification = getAllDistinctTerms(one_justification);
		  DLAtom d_r4_bi(lambda_prime, ucq_justification, all_distinct_terms_justification);
		  r4.addNegativeDLAtom(d_r4_bi);
		}
	      else
		{
		  if (one_justification.size() > 1)
		    {
		      throw PluginError("Conjunctive justification encountered. --cq must be ON!");
		    }
		  else
		    {
		      DLAtom d_r4_bi(lambda_prime, one_justification.begin()->getNegatedLiteralName(), one_justification.begin()->getTerms());
		      r4.addNegativeDLAtom(d_r4_bi);
		    }
		}
	    }			
	}
      
      // adding dom() predicates for safety conditions
      Terms all_distinct_terms = all_distinct_terms_conclusion;
      all_distinct_terms.insertNewTerms(all_distinct_terms_justifications);
      all_distinct_terms.insertNewTerms(all_distinct_terms_premise);
      terms = all_distinct_terms.getMTerms();
      for (t_pos = terms.begin(); t_pos != terms.end(); t_pos++) 
	{
	  //	  if (t_pos->isVar())
	  //{
	      Predicate p_dom("dom", *t_pos);
	      r4.addPositiveBody(p_dom);			
	      //}
	}
      rules.addDLRule(r4);
      
      // conclusion_p[0](Terms_conclusion[0]) :- all_p_def_id(allTerms_conclusion).
      // ...
      // conclusion_p[n](Terms_conclusion[n]) :- all_p_def_id(allTerms_conclusion).
      for (c_pos = conclusion.begin(); c_pos != conclusion.end(); c_pos++) 
	{
	  Terms projected_terms = all_distinct_terms_conclusion.projectTo(c_pos->getTerms());
	  //Predicate p_ri_h(PREFIX_PRED + c_pos->getPredicateName(), c_pos->getTerms());
	  Predicate p_ri_h(PREFIX_PRED + c_pos->getSignedPredicateName(), c_pos->getTerms());
	  Predicate p_ri_b(PREFIX_ALL_PRED + strid.str(), projected_terms);
	  DLRule ri(p_ri_h);
	  ri.addPositiveBody(p_ri_b);
	  rules.addDLRule(ri);
	}
      
      // fail :- not DL[\lambda; conclusion[0](Terms[0]),...,conclusion[n](Terms[n])](allTerms_conclusion), all_in_def_id(allTerms_conclusion), not fail.
      DLRule r5(p_fail);
      DLRule r6(p_fail);
      if (cqmode)
	{
	  DLAtom d_r5_b(lambda, ucq_conclusion, all_distinct_terms_conclusion);
	  r5.addNegativeDLAtom(d_r5_b);
	  r6.addPositiveDLAtom(d_r5_b);
	}
      else
	  {
	    if (conclusion.size() == 1)
	      {
		DLAtom d_r5_b(lambda, conclusion.begin()->getLiteralName(), conclusion.begin()->getTerms());
		r5.addNegativeDLAtom(d_r5_b);
		r6.addPositiveDLAtom(d_r5_b);
	      }
	    else
	      {
		throw PluginError("Conjuctive conclusion encountered. --cq must be ON!");
	      }
	  }
	r5.addPositiveBody(p_r1_h);
	r5.addNegativeBody(p_fail);
	rules.addDLRule(r5);

	// fail :- not DL[\lambda; conclusion[0](Terms[0]),...,conclusion[n](Terms[n])](allTerms_conclusion), out_def_id(allTerms_conclusion), not fail.
	r6.addPositiveBody(p_r1_b);
	r6.addNegativeBody(p_fail);
	rules.addDLRule(r6);
    }
  return rules;	
}

DLRules 
Default::getDLRules1(bool cqmode) // Testing new transformation
{
  DLRules rules;
  
  if (premise.size() == 0 && justification.size() == 0)
    {
      return getFacts();
    }
  else 
    {
      Updates lambda_prime = parent->getLambdaPrime();
      Terms all_distinct_terms_conclusion = getAllDistinctTerms(conclusion);
      Terms all_distinct_terms_justifications = getAllDistinctTerms(justification);
      Terms all_distinct_terms_premise = getAllDistinctTerms(premise);
      std::stringstream strid;
      strid << id;
      // all_p_def_id(allTerms_conslusion) :-	
      //		DL[\lambda; premise[0](Terms_premise[0]),...,premise[k](Terms_premise[k])](allTerms_premise),
      //		not DL[\lambda'; -justification[0]](allTerms_justification_0),
      //			...
      //		not DL[\lambda'; -justification[i,0](Terms_justification[i,0]) v ... v -justification[i,m](Terms_justification[i,m])]
      //			...
      //		not DL[\lambda'; -justification[m]](allTerms_justification_m).
      Predicate p_r_h(PREFIX_ALL_IN + strid.str(), all_distinct_terms_conclusion);
      all_in_p = p_r_h;
      DLRule r(p_r_h);
//       if (premise.size() > 0) 
// 	{
// 	  Pred2Dim ucq_premise;
// 	  ucq_premise.push_back(premise);
// 	  Terms all_distinct_terms_premise = getAllDistinctTerms(premise);
// 	  DLAtom d_r_b1(lambda_prime, ucq_premise, all_distinct_terms_premise);			
// 	  r.addPositiveDLAtom(d_r_b1);
// 	}
      // If we don't use ucq here:
      // all_p_def_id(allTerms_conslusion) :-	
      //		DL[\lambda'; premise[0]](Terms_premise[0]),...,DL[\lambda';premise[k]](Terms_premise[k]),
      //		cons_\beta_{i,1}(Y_{i,1}),...,cons_\beta_{i,m_i}(Y_{i,m_i}).
      if (premise.size() > 0)
	{
	  Pred1Dim::iterator p_pos;
	  for (p_pos = premise.begin(); p_pos != premise.end(); ++p_pos)
	    {
	      if (cqmode)
		{
		  DLAtom d_r_bi(lambda_prime, *p_pos, p_pos->getTerms());
		  r.addPositiveDLAtom(d_r_bi);
		}
	      else
		{
		  DLAtom d_r_bi(lambda_prime, p_pos->getLiteralNameWithNS(), p_pos->getTerms());
		  r.addPositiveDLAtom(d_r_bi);
		}
	    }
	}

      if (justification.size() > 0)
	{
	  Pred2Dim::iterator j_pos;
	  for (j_pos = justification.begin(); j_pos != justification.end(); j_pos++) 
	    {
	      Pred1Dim one_justification = *j_pos;
	      if (cqmode)
		{
		  Pred2Dim ucq_justification;
		  Pred1Dim::iterator pos;
		  for (pos = one_justification.begin(); pos != one_justification.end(); pos++) 
		    {
		      Predicate p_justification(!pos->isStronglyNegated(), pos->getPredicateNameWithNS(), pos->getTerms());
		      Pred1Dim cq_justification;
		      cq_justification.push_back(p_justification);
		      ucq_justification.push_back(cq_justification);
		    }
		  Terms all_distinct_terms_justification = getAllDistinctTerms(one_justification);
		  DLAtom d_r_bi(lambda_prime, ucq_justification, all_distinct_terms_justification);
		  r.addNegativeDLAtom(d_r_bi);
		}
	      else
		{
		  if (one_justification.size() > 1)
		    {
		      throw PluginError("Conjuctive justification encountered. --cq must be ON!");
		    }
		  else
		    {
		      Pred1Dim::iterator p_pos = one_justification.begin();
		      DLAtom d_r_bi(lambda_prime, p_pos->getLiteralNameWithNS(), p_pos->getTerms());
		      r.addNegativeDLAtom(d_r_bi);
		    }
		}
	    }			
	}
      
      // adding dom() predicates for safety conditions
      std::vector<MTerm> terms;
      std::vector<MTerm>::iterator t_pos;	

      Terms all_distinct_terms = all_distinct_terms_conclusion;
      all_distinct_terms.insertNewTerms(all_distinct_terms_justifications);
      all_distinct_terms.insertNewTerms(all_distinct_terms_premise);
      terms = all_distinct_terms.getMTerms();
      for (t_pos = terms.begin(); t_pos != terms.end(); t_pos++) 
	{
	  //	  if (t_pos->isVar())
	  //{
	      Predicate p_dom("dom", *t_pos);
	      r.addPositiveBody(p_dom);			
	      //}
	}
      rules.addDLRule(r);
      
      // ...
      // in_conclusion_i(Y_i) :- all_in_def_id(Y).
      // project Y to Y_i and let all other become '_'
      // ...
      Pred1Dim::iterator c_pos;
      for (c_pos = conclusion.begin(); c_pos != conclusion.end(); c_pos++) 
	{
	  Terms projected_terms = all_distinct_terms_conclusion.projectTo(c_pos->getTerms());
	  Predicate p_ri_h(PREFIX_IN + c_pos->getSignedPredicateName(), c_pos->getTerms());
	  Predicate p_ri_b(PREFIX_ALL_IN + strid.str(), projected_terms);
	  DLRule ri(p_ri_h);
	  ri.addPositiveBody(p_ri_b);
	  rules.addDLRule(ri);
	}
    }
  return rules;
}

DLRules
Default::getDLRules2(bool cqmode)
{
DLRules rules;
  
  if (premise.size() == 0 && justification.size() == 0)
    {
      return getFacts();
    }
  else 
    {
      std::stringstream strid;
      strid << id;
      // Guess the consistency of \beta_{i,j}
      // auxc_\beta_{i,j}(Y_{i,j}) :- not out_auxc_\beta_{i,j}(Y_{i,j}).
      // out_auxc_\beta_{i,j}(Y_{i,j}) :- out_auxc_\beta_{i,j}(Y_{i,j}).
      Pred2Dim::iterator j_pos;
      Pred1Dim one_justification;
      Pred1Dim::iterator j_pos1;
      if (justification.size() > 0)
	{
	  int i = 0;
	  for (j_pos = justification.begin(); j_pos != justification.end(); ++j_pos)
	    {
	      ++i;
	      one_justification = *j_pos;
	      if (one_justification.size() == 1) // not a conjuction (simple case)
		{
		  Predicate pj = *one_justification.begin();
		  Predicate pi(PREFIX_CONS + pj.getSignedPredicateName(), pj.getTerms());
		  Predicate po(PREFIX_OUT_CONS + pj.getSignedPredicateName(), pj.getTerms());
		  DLRule r1(pi);
		  r1.addNegativeBody(po);		  
		  DLRule r2(po);
		  r2.addNegativeBody(pi);
		  std::vector<MTerm> terms = pj.getTerms().getMTerms();
		  std::vector<MTerm>::iterator t_pos;
		  for (t_pos = terms.begin(); t_pos != terms.end(); ++t_pos)
		    {
		      if (t_pos->isVar())
			{
			  Predicate p_dom("dom", *t_pos);
			  r1.addPositiveBody(p_dom);
			  r2.addPositiveBody(p_dom);
			}
		    }
		  rules.addDLRule(r1);
		  rules.addDLRule(r2);
		}
	      else // a conjuction (complicated case)
		{
		  Terms all_distinct_terms_justification = getAllDistinctTerms(one_justification);
		  std::stringstream just_i;
		  just_i << i;
		  std::string just_id;
		  just_id = strid.str() + "_" + just_i.str();
		  Predicate p_cons(PREFIX_CONS + just_id, all_distinct_terms_justification);
		  Predicate p_out_cons(PREFIX_OUT_CONS + just_id, all_distinct_terms_justification);
		  DLRule r1(p_cons);
		  r1.addNegativeBody(p_out_cons);
		  DLRule r2(p_out_cons);
		  r2.addNegativeBody(p_cons);
		  std::vector<MTerm> terms = all_distinct_terms_justification.getMTerms();
		  std::vector<MTerm>::iterator t_pos;
		  for (t_pos = terms.begin(); t_pos != terms.end(); ++t_pos)
		    {
		      if (t_pos->isVar())
			{
			  Predicate p_dom("dom", *t_pos);
			  r1.addPositiveBody(p_dom);
			  r2.addPositiveBody(p_dom);
			}
		    }
		  rules.addDLRule(r1);
		  rules.addDLRule(r2);
		}
	    }
	}
	
      // Rules that evaluate extensions
      // all_p_def_id(allTerms_conslusion) :-	
      //		DL[\lambda; premise[0](Terms_premise[0]),...,premise[k](Terms_premise[k])](allTerms_premise),
      //		cons_\beta_{i,1}(Y_{i,1}),...,cons_\beta_{i,m_i}(Y_{i,m_i}).
      Updates lambda_prime = parent->getLambdaPrime();
      Terms all_distinct_terms_conclusion = getAllDistinctTerms(conclusion);
      Predicate p_r3_h(PREFIX_ALL_IN + strid.str(), all_distinct_terms_conclusion);
      all_in_p = p_r3_h;
      DLRule r3(p_r3_h);      

//    if (premise.size() > 0) 
// 	{
// 	  Pred2Dim ucq_premise;
// 	  ucq_premise.push_back(premise);
// 	  Terms all_distinct_terms_premise = getAllDistinctTerms(premise);
// 	  DLAtom d_r3_b1(lambda_prime, ucq_premise, all_distinct_terms_premise);		
// 	  r3.addPositiveDLAtom(d_r3_b1);
// 	}

      // If we don't use ucq here:
      // all_p_def_id(allTerms_conslusion) :-	
      //		DL[\lambda'; premise[0]](Terms_premise[0]),...,DL[\lambda';premise[k]](Terms_premise[k]),
      //		cons_\beta_{i,1}(Y_{i,1}),...,cons_\beta_{i,m_i}(Y_{i,m_i}).
      Pred1Dim::iterator p_pos;
      for (p_pos = premise.begin(); p_pos != premise.end(); ++p_pos)
	{
	  if (cqmode)
	    {
	      DLAtom d_r3_bi(lambda_prime, *p_pos, p_pos->getTerms());
	      r3.addPositiveDLAtom(d_r3_bi);
	    }
	  else
	    {
	      DLAtom d_r3_bi(lambda_prime, p_pos->getLiteralName(), p_pos->getTerms());
	      r3.addPositiveDLAtom(d_r3_bi);
	    }
	}

      if (justification.size() > 0)
	{
	  int i = 0;
	  for (j_pos = justification.begin(); j_pos != justification.end(); ++j_pos)
	    {
	      ++i;
	      one_justification = *j_pos;
	      if (one_justification.size() == 1)
		{
		  Predicate p_cons(PREFIX_CONS + one_justification.begin()->getSignedPredicateName(), one_justification.begin()->getTerms());
		  r3.addPositiveBody(p_cons);
		}
	      else
		{
		  std::stringstream just_i;
		  just_i << i;
		  std::string just_id = strid.str() + "_" + just_i.str();
		  Terms all_distinct_terms_justification = getAllDistinctTerms(one_justification);
		  Predicate p_cons(PREFIX_CONS + just_id, all_distinct_terms_justification);
		  r3.addPositiveBody(p_cons);
		}
	    }
	}
      
      rules.addDLRule(r3);

      // ...
      // in_conclusion_i(Y_i) :- all_in_def_id(Y).
      // project Y to Y_i and let all other become '_'
      // ...
      Pred1Dim::iterator c_pos;
      for (c_pos = conclusion.begin(); c_pos != conclusion.end(); c_pos++) 
	{
	  Terms projected_terms = all_distinct_terms_conclusion.projectTo(c_pos->getTerms());
	  Predicate p_ri_h(PREFIX_IN + c_pos->getSignedPredicateName(), c_pos->getTerms());
	  Predicate p_ri_b(PREFIX_ALL_IN + strid.str(), projected_terms);
	  DLRule ri(p_ri_h);
	  ri.addPositiveBody(p_ri_b);
	  rules.addDLRule(ri);
	}
      
      // Rules that check the compliance of our guess with extension E
      Predicate p_fail("fail");     
      if (justification.size() > 0)
	{
	  int i = 0;
	  for (j_pos = justification.begin(); j_pos != justification.end(); j_pos++) 
	    {
	      ++i;
	      DLRule r4(p_fail);
	      r4.addNegativeBody(p_fail);	      
	      DLRule r5(p_fail);
	      r5.addNegativeBody(p_fail);
	      Pred1Dim one_justification = *j_pos;

	      if (one_justification.size() == 1)
		{
		  if (cqmode)
		    {
		      Pred2Dim ucq_justification;
		      Predicate p_justification(!one_justification.begin()->isStronglyNegated(), one_justification.begin()->getPredicateName(), one_justification.begin()->getTerms());
		      Pred1Dim cq_justification;
		      cq_justification.push_back(p_justification);
		      ucq_justification.push_back(cq_justification);
		      DLAtom d_r_bi(lambda_prime, ucq_justification, one_justification.begin()->getTerms());
		      r4.addPositiveDLAtom(d_r_bi);
		      r5.addNegativeDLAtom(d_r_bi);
		    }
		  else
		    {
		      DLAtom d_r_bi(lambda_prime, one_justification.begin()->getNegatedLiteralName(), one_justification.begin()->getTerms());
		      r4.addPositiveDLAtom(d_r_bi);
		      r5.addNegativeDLAtom(d_r_bi);
		    }
		  Predicate p_cons(PREFIX_CONS + j_pos->begin()->getSignedPredicateName(), j_pos->begin()->getTerms());
		  r4.addPositiveBody(p_cons);
		  Predicate p_out_cons(PREFIX_OUT_CONS + j_pos->begin()->getSignedPredicateName(), j_pos->begin()->getTerms());
		  r5.addPositiveBody(p_out_cons);
		}
	      else
		{
		  if (!cqmode)
		    {
		      throw PluginError("Conjunctive justification encountered. --cq must be ON!");
		    }
		  Pred2Dim ucq_justification;
		  Pred1Dim::iterator pos;	      
		  for (pos = one_justification.begin(); pos != one_justification.end(); pos++) 
		    {
		      Predicate p_justification(!pos->isStronglyNegated(), pos->getPredicateName(), pos->getTerms());
		      Pred1Dim cq_justification;
		      cq_justification.push_back(p_justification);
		      ucq_justification.push_back(cq_justification);
		    }
		  Terms all_distinct_terms_justification = getAllDistinctTerms(one_justification);
		  DLAtom d_r_bi(lambda_prime, ucq_justification, all_distinct_terms_justification);
		  r4.addPositiveDLAtom(d_r_bi);
		  r5.addNegativeDLAtom(d_r_bi);
		  
		  std::stringstream just_i;
		  just_i << i;
		  std::string just_id;
		  just_id = strid.str() + "_" + just_i.str();
		  Predicate p_cons(PREFIX_CONS + just_id, all_distinct_terms_justification);
		  r4.addPositiveBody(p_cons);
		  Predicate p_out_cons(PREFIX_OUT_CONS + just_id, all_distinct_terms_justification);
		  r5.addNegativeBody(p_out_cons);
		}
	     
	      		
	      rules.addDLRule(r4);
	      rules.addDLRule(r5);
	    }			
	}
      
    }
  return rules;
}

} // namespace df
} // namespace dlvhex
