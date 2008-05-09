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
#include <iostream>
 
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
Defaults::rename_terms(Pred2Dim& justification, std::string& str_id)
{
  Pred2Dim::iterator j_pos;
  for (j_pos = justification.begin(); j_pos != justification.end(); ++j_pos)
    {
      rename_terms(*j_pos, str_id);
    }
}

void 
Defaults::rename_terms(Pred1Dim& conclusion, std::string& str_id) 
{ 
  Pred1Dim::iterator c_pos; 
  for (c_pos = conclusion.begin(); c_pos != conclusion.end(); c_pos++) 
    { 
      c_pos->rename_terms(str_id); 
    } 
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

DLRule
Defaults::forceOutRule(Default& d_pos, Default& d_pos2, Terms& d_pos_all_disc_terms, Terms& d_pos2_all_disc_terms, Unifier& pp_uni)
{
  Terms ts = d_pos_all_disc_terms.unify(pp_uni, true); 
  Terms ts2 = d_pos2_all_disc_terms.unify(pp_uni, true); 
  Predicate p_fail("fail");
  DLRule r_force_out(p_fail);
  r_force_out.addNegativeBody(p_fail);
  Predicate d_pos_all_in(d_pos.getAllInPred().getPredicateName(), ts);
  Predicate d_pos2_all_in(d_pos2.getAllInPred().getPredicateName(), ts2);
  r_force_out.addPositiveBody(d_pos_all_in);
  r_force_out.addPositiveBody(d_pos2_all_in);

  return r_force_out;
}

DLRules 
Defaults::getForcingDLRules() 
{ 
  bool forced_out;
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
	  Pred2Dim d_pos_justifications = d_pos->getJustifications();
	  Pred2Dim d_pos2_justifications = d_pos2->getJustifications();

	  d_pos_all_disc_terms.rename_terms(str_id);
	  d_pos2_all_disc_terms.rename_terms(str_id2);
	  rename_terms(d_pos_conclusion, str_id); 
	  rename_terms(d_pos2_conclusion, str_id2); 
	  rename_terms(d_pos_justifications, str_id); 
	  rename_terms(d_pos2_justifications, str_id2);
	  
	  // Check if any pair of predicates will force OUT
	  Pred1Dim::iterator dc_pos;
	  Pred1Dim::iterator dc_pos2; 
	  forced_out = false;
	  for (dc_pos = d_pos_conclusion.begin(); dc_pos != d_pos_conclusion.end(); ++dc_pos) 
	    { 
	      for (dc_pos2 = d_pos2_conclusion.begin(); dc_pos2 != d_pos2_conclusion.end(); ++dc_pos2) 
		{ 
		  Unifier pp_uni = dc_pos->isNegatedUnifiable(*dc_pos2); 
		  if (!pp_uni.getUnifier().empty()) 
		    { 
		      rules.addDLRule(forceOutRule(*d_pos, *d_pos2, d_pos_all_disc_terms,
					     d_pos2_all_disc_terms, pp_uni));
		      forced_out = true;
		      break;
		    }
		}
	      if (forced_out)
		break;
	      Pred2Dim::iterator djs_pos2;
	      for (djs_pos2 = d_pos2_justifications.begin(); djs_pos2 != d_pos2_justifications.end(); ++djs_pos2)
		{
		  Pred1Dim one_justification = *djs_pos2;
		  Pred1Dim::iterator dj_pos2;
		  for (dj_pos2 = one_justification.begin(); dj_pos2 != one_justification.end(); ++dj_pos2)
		    {
		      Unifier pp_uni = dc_pos->isNegatedUnifiable(*dj_pos2);
		      if (!pp_uni.getUnifier().empty())
			{
			  rules.addDLRule(forceOutRule(*d_pos, *d_pos2, d_pos_all_disc_terms,
						 d_pos2_all_disc_terms, pp_uni));
			  forced_out = true;
			  break;
			}
		    }
		  if (forced_out)
		    break;
		}
	      if (forced_out)
		break;
	    }

	  // One more chance for forcing out
	  // i.e., d_pos' justification and d_pos2's conclusion
	  if (!forced_out)
	    {
	      for (dc_pos2 = d_pos2_conclusion.begin(); dc_pos2 != d_pos2_conclusion.end(); ++dc_pos2)
		{
		  Pred2Dim::iterator djs_pos;
		  for (djs_pos = d_pos_justifications.begin(); djs_pos != d_pos_justifications.end(); ++djs_pos)
		    {
		      Pred1Dim one_justification = *djs_pos;
		      Pred1Dim::iterator dj_pos;
		      for (dj_pos = one_justification.begin(); dj_pos != one_justification.end(); ++dj_pos)
			{
			  Unifier pp_uni = dj_pos->isNegatedUnifiable(*dc_pos2);
			  if (!pp_uni.getUnifier().empty())
			    {
			      rules.addDLRule(forceOutRule(*d_pos, *d_pos2, d_pos_all_disc_terms,
							   d_pos2_all_disc_terms, pp_uni));
			      forced_out = true;
			      break;			      
			    }
			}
		      if (forced_out)
			break;
		    }
		  if (forced_out)
		    break;
		}
	    }
	  
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
Defaults::getDLRules(bool cqmode, int trans, bool pruning)
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
  if (pruning)
    {
      rules.insertDLRules(getForcingDLRules()); 
      //rules.insertDLRules(getDirectRules()); 
    }
  return rules; 
} 

}	// namespace df 
}	// namespace dlvhex
