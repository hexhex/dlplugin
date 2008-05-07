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
 * @file   Defaults.cpp 
 * @author DAO Tran Minh 
 * @date   Tue Dec 18 17:20:24 2007 
 *  
 * @brief  A set of Defaults. 
 *  
 *  
 */ 
 
#include "Defaults.h" 
#include <sstream> 
 
namespace dlvhex { 
namespace df { 
 
Defaults::Defaults()  
{ 
  count = 0; 
} 
 
void  
Defaults::addDefault(Default& df)  
{ 
  df.setParent(this); 
  df.setID(++count); 
  dfs.push_back(df);	 
} 
 
Updates  
Defaults::getLambda()  
{ 
  Updates us; 
  if (dfs.size() > 0)  
    { 
      std::vector<Default>::iterator pos;	 
      for (pos = dfs.begin(); pos != dfs.end(); pos++)  
	{ 
	  us.insertUpdates(pos->getUpdates4Lambda()); 
	} 
    } 
  return us; 
} 
 
Updates  
Defaults::getLambdaPrime() 
{ 
  Updates us; 
  if (dfs.size() > 0)  
    { 
      std::vector<Default>::iterator pos;	 
      for (pos = dfs.begin(); pos != dfs.end(); pos++)  
	{ 
	  us.insertUpdates(pos->getUpdates4LambdaPrime()); 
	} 
    } 
  return us; 
} 
 
void 
Defaults::rename_terms(Pred1Dim& conclusion, Terms& terms, std::string& str_id) 
{ 
  Pred1Dim::iterator c_pos; 
  for (c_pos = conclusion.begin(); c_pos != conclusion.end(); c_pos++) 
    { 
      c_pos->rename_terms(str_id); 
    } 
  terms.rename_terms(str_id); 
} 

Unifier1Dim 
Defaults::check_unifiable(Predicate& p, Pred1Dim& conclusion) 
{ 
  Unifier1Dim uniset_p; 
  Pred1Dim::iterator c_pos; 
  for (c_pos = conclusion.begin(); c_pos != conclusion.end(); c_pos++) 
    { 
      Unifier uni_pp = c_pos->isUnifiable(p); 
      if (!uni_pp.getUnifier().empty()) 
	{ 
	  uniset_p.push_back(uni_pp); 
	} 
    } 
  return uniset_p; 
} 
 
Unifier2Dim 
Defaults::check_forcing_in(Pred1Dim& conclusion1, Pred1Dim& conclusion2) 
{ 
  Unifier2Dim uniset; 
  Pred1Dim::iterator c2_pos; 
  for (c2_pos = conclusion2.begin(); c2_pos != conclusion2.end(); c2_pos++) 
    { 
      Unifier1Dim uniset_p = check_unifiable(*c2_pos, conclusion1); 
      if (uniset_p.empty()) 
	{ 
	  uniset.clear(); 
	  return uniset; 
	} 
      else 
	{ 
	  uniset.push_back(uniset_p); 
	} 
    } 
  return uniset; 
} 
 
void 
Defaults::gen_force_in_rules(DLRules& rules, Unifier2Dim& uniset,  
			     Unifier2Dim::iterator uniset_pos, 
			     Unifier unifier, 
			     std::string pname1, Terms& ts1, 
			     std::string pname2, Terms& ts2) 
{ 
	Unifier1Dim uni_candidate = *uniset_pos; 
	Unifier1Dim::iterator m_pos; 
	for (m_pos = uni_candidate.begin(); m_pos != uni_candidate.end(); m_pos++) 
	{		 
		if (unifier.isConsistent(*m_pos)) 
		{ 
			Unifier new_uni = unifier; 
			new_uni.getUnifier().insert(m_pos->getUnifier().begin(), m_pos->getUnifier().end()); 
			Unifier2Dim::iterator uniset_pos_next = uniset_pos; 
			uniset_pos_next++; 
			if (uniset_pos_next == uniset.end()) 
			{ 
				// generating rules here 
				Terms ts_h = ts2.unify(new_uni, false); 
				Terms ts_b = ts1.unify(new_uni, false); 
				Predicate p_h(pname2, ts_h); 
				Predicate p_b(pname1, ts_b); 
				DLRule ri(p_h); 
				ri.addPositiveBody(p_b); 
				rules.addDLRule(ri); 
			} 
			else 
			{ 
				// continue 
				gen_force_in_rules(rules, uniset,  
						   uniset_pos_next, new_uni,  
						   pname1, ts1,  
						   pname2, ts2); 
			} 
		} 
	} 
} 
 
DLRules 
Defaults::getForcingDLRules() 
{ 
  DLRules rules; 
  std::vector<Default>::iterator d_pos; 
  std::vector<Default>::iterator d_pos_end_limit = dfs.end(); 
  d_pos_end_limit--;	 
  for (d_pos = dfs.begin(); d_pos != d_pos_end_limit; d_pos++) 
    { 
      std::vector<Default>::iterator d_pos_next = d_pos;		 
      d_pos_next++; 
      for (std::vector<Default>::iterator d_pos2 = d_pos_next; d_pos2 != dfs.end(); d_pos2++) 
	{	 
	  // Preparation, rename all the variables 
	  std::string str_id; 
	  std::string str_id2; 
	  std::stringstream out; 
	  std::stringstream out2; 
	  out << d_pos->getID(); 
	  out2 << d_pos2->getID(); 
	  str_id = out.str(); 
	  str_id2 = out2.str(); 
	  Pred1Dim d_pos_conclusion = d_pos->getConclusion(); 
	  Pred1Dim d_pos2_conclusion = d_pos2->getConclusion(); 
	  Terms d_pos_all_disc_terms = d_pos->getAllInPred().getTerms(); 
	  Terms d_pos2_all_disc_terms = d_pos2->getAllInPred().getTerms(); 
	  
	  rename_terms(d_pos_conclusion, d_pos_all_disc_terms, str_id); 
	  rename_terms(d_pos2_conclusion, d_pos2_all_disc_terms, str_id2); 
	  
	  // Check if any pair of predicates will force OUT			 
	  /*Pred1Dim::iterator dc_pos; 
	    Pred1Dim::iterator dc_pos2; 
	    for (dc_pos = d_pos_conclusion.begin(); dc_pos != d_pos_conclusion.end(); dc_pos++) 
	      { 
		for (dc_pos2 = d_pos2_conclusion.begin(); dc_pos2 != d_pos2_conclusion.end(); dc_pos2++) 
		  { 
		Unifier pp_uni = dc_pos->isNegatedUnifiable(*dc_pos2); 
		if (!pp_uni.getUnifier().empty()) 
		  { 
		    Terms ts = d_pos_all_disc_terms.unify(pp_uni, true); 
		    Terms ts2 = d_pos2_all_disc_terms.unify(pp_uni, true); 
		    Terms tp = dc_pos->getTerms().unify(pp_uni, false);						 
		    Terms tp2 = dc_pos2->getTerms().unify(pp_uni, false);				 
		    Predicate p_r1_h(d_pos->getOutPred().getPredicateName(), ts); 
		    Predicate p_r1_b(PREFIX_IN + dc_pos2->getSignedPredicateName(), tp2); 
		    DLRule r1(p_r1_h); 
		    r1.addPositiveBody(p_r1_b); 
		    std::vector<MTerm> r1_h_terms = ts.getMTerms(); 
		    std::vector<MTerm>::iterator t_pos; 
		    for (t_pos = r1_h_terms.begin(); t_pos != r1_h_terms.end(); t_pos++) 
		      { 
			if (!tp2.gotThisTerm(*t_pos)) 
			  { 
			    Predicate p_dom("dom", *t_pos); 
			    r1.addPositiveBody(p_dom); 
			  } 
		      } 
		    rules.addDLRule(r1); 
		    
		    Predicate p_r2_h(d_pos2->getOutPred().getPredicateName(), ts2); 
		    Predicate p_r2_b(PREFIX_IN + dc_pos->getSignedPredicateName(), tp); 
		    DLRule r2(p_r2_h); 
		    r2.addPositiveBody(p_r2_b); 
		    std::vector<MTerm> r2_h_terms = ts2.getMTerms();						 
		    for (t_pos = r2_h_terms.begin(); t_pos != r2_h_terms.end(); t_pos++) 
		      { 
			if (!tp.gotThisTerm(*t_pos)) 
			  { 
			    Predicate p_dom("dom", *t_pos); 
			    r2.addPositiveBody(p_dom); 
			  } 
		      } 
		    rules.addDLRule(r2); 
		  } 
		  } 
	      }*/ 
	  // Checking for forcing in rules 
	  // d_pos ~~Force IN~~> d_pos2 ??? 
	  Unifier2Dim uniset = check_forcing_in(d_pos_conclusion, d_pos2_conclusion); 
	  if (!uniset.empty()) 
	    // Now we have unification(s) to consider 
	    { 
	      Unifier unifier; 
	      gen_force_in_rules(rules, uniset, uniset.begin(), unifier, 
				 d_pos->getAllInPred().getPredicateName(), d_pos_all_disc_terms, 
				 d_pos2->getAllInPred().getPredicateName(), d_pos2_all_disc_terms); 
	    } 
	  
	  // d_pos2 ~~Force IN~~> d_pos ??? 
	  uniset = check_forcing_in(d_pos2_conclusion, d_pos_conclusion); 
	  if (!uniset.empty()) 
	    // Now we have unification(s) to consider 
	    { 
	      Unifier unifier; 
	      gen_force_in_rules(rules, uniset, uniset.begin(), unifier, 
				 d_pos2->getAllInPred().getPredicateName(), d_pos2_all_disc_terms, 
				 d_pos->getAllInPred().getPredicateName(), d_pos_all_disc_terms); 
	    } 
	} 
    } 
  return rules; 
} 
 
DLRules 
Defaults::getDirectRules() 
{ 
  DLRules rules; 
  Pred1Dim tmp_p; 
  std::vector<Default>::iterator ds_pos; 
  Pred1Dim::iterator c_pos; 
  Pred1Dim concl; 
  for (ds_pos = dfs.begin(); ds_pos != dfs.end(); ds_pos++) 
    { 
      concl = ds_pos->getConclusion(); 
      for (c_pos = concl.begin(); c_pos != concl.end(); c_pos++) 
	{ 
	  if ((!c_pos->isStronglyNegated()) && (c_pos->compareTo(tmp_p) != LESS_GENERAL)) 
	    { 
	      tmp_p.push_back(*c_pos); 
	    } 
	} 
    } 
  for (c_pos = tmp_p.begin(); c_pos != tmp_p.end(); c_pos++) 
    { 
      Predicate my_p(PREFIX_MY + c_pos->getPredicateName(), c_pos->getTerms()); 
      Predicate in_p(PREFIX_IN + c_pos->getPredicateName(), c_pos->getTerms()); 
      Predicate p_p(PREFIX_PRED + c_pos->getPredicateName(), c_pos->getTerms()); 
      Pred1Dim cq; 
      cq.push_back(*c_pos); 
      Pred2Dim ucq; 
      ucq.push_back(cq); 
      DLAtom dl_p(ucq, c_pos->getTerms()); 
      DLRule r1(my_p); 
      r1.addPositiveDLAtom(dl_p); 
      DLRule r2(in_p); 
      r2.addPositiveBody(my_p); 
      DLRule r3(p_p); 
      r3.addPositiveBody(my_p); 
      rules.addDLRule(r1); 
      rules.addDLRule(r2); 
      rules.addDLRule(r3); 
    } 
  return rules; 
} 
 
DLRules  
Defaults::getDLRules(bool cqmode, int trans)
{ 
  DLRules rules; 
  std::vector<Default>::iterator pos;	 
  for (pos = dfs.begin(); pos != dfs.end(); pos++)  
    { 
      DLRules rs;
      switch (trans)
	{
	case 1:
	  rs = pos->getDLRules(cqmode);
	  break;
	case 2:
	  rs = pos->getDLRules1(cqmode);
	  break;
	case 3:
	  rs = pos->getDLRules2(cqmode);
	  break;
	}
      rules.insertDLRules(rs); 
    } 
  //rules.insertDLRules(getForcingDLRules()); 
  //rules.insertDLRules(getDirectRules()); 
  return rules; 
} 

}	// namespace df 
}	// namespace dlvhex
