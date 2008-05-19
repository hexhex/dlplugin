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
 * @file   Predicate.cpp 
 * @author DAO Tran Minh 
 * @date   Tue Dec 18 17:20:24 2007 
 *  
 * @brief  A Predicate. 
 *  
 *  
 */ 
 
#include "Predicate.h" 
 
namespace dlvhex { 
namespace df { 
 
Predicate::Predicate() 
{ } 
 
Predicate::Predicate(const std::string& predicate_name_) 
  : isStrongNegated(false), predicate_name(predicate_name_), prefix("")
{ } 
 
Predicate::Predicate(const std::string& predicate_name_, const Terms& terms_) 
  : isStrongNegated(false), predicate_name(predicate_name_), terms(terms_), prefix("")
{ } 
 
Predicate::Predicate(const std::string& predicate_name_, const MTerm& term_) 
  : isStrongNegated(false), predicate_name(predicate_name_), prefix("")
{ 
  terms.addTerm(term_); 
} 
 
Predicate::Predicate(bool isStrongNegated_, const std::string& predicate_name_) 
  : isStrongNegated(isStrongNegated_), predicate_name(predicate_name_), prefix("") 
{ } 

Predicate::Predicate(bool isStrongNegated_, const std::string& predicate_name_, const Terms& terms_)
: isStrongNegated(isStrongNegated_), predicate_name(predicate_name_), prefix(""), terms(terms_) 
{ }

Predicate::Predicate(bool isStrongNegated_, const std::string& predicate_name_, const std::string& prefix_, const Terms& terms_)
: isStrongNegated(isStrongNegated_), predicate_name(predicate_name_), prefix(prefix_), terms(terms_) 
{ }
 
bool  
Predicate::isStronglyNegated()  
{ 
  return isStrongNegated; 
} 

std::string
Predicate::getPrefix()
{
  return prefix;
}
 
std::string  
Predicate::getPredicateName()  
{ 
  return predicate_name; 
} 

std::string
Predicate::getPredicateNameWithNS()
{
  if (prefix.compare("") == 0)
    {
      return predicate_name;
    }
  return prefix + '#' + predicate_name;
}
 
std::string  
Predicate::getSignedPredicateName()  
{ 
  std::string tmp = ""; 
  if (isStrongNegated)  
    { 
      tmp = "not_"; 
    } 
  return (tmp + predicate_name); 
} 

std::string  
Predicate::getLiteralName()  
{ 
  std::string tmp = ""; 
  if (isStrongNegated)  
    { 
      tmp = "-"; 
    }
  return (tmp + predicate_name); 
} 

std::string  
Predicate::getLiteralNameWithNS()  
{ 
  std::string tmp = ""; 
  if (isStrongNegated)  
    { 
      tmp = "-"; 
    } 
  if (prefix.compare("") == 0)
    {
      return (tmp + predicate_name); 
    }
  return tmp + prefix + '#' + predicate_name;
} 
 
std::string 
Predicate::getNegatedLiteralName()  
{ 
  std::string tmp = ""; 
  if (!isStrongNegated)  
    { 
      tmp = "-"; 
    } 
  return (tmp + predicate_name); 
} 

std::string 
Predicate::getNegatedLiteralNameWithNS()  
{ 
  std::string tmp = ""; 
  if (!isStrongNegated)  
    { 
      tmp = "-"; 
    } 
  if (prefix.compare("") == 0)
    {
      return (tmp + predicate_name); 
    }
  return tmp + prefix + '#' + predicate_name;
}

Terms&  
Predicate::getTerms()  
{ 
  return terms; 
} 
 
ComparisonResult 
Predicate::compareTo(Predicate& p2) 
{ 
  if ((isStrongNegated != p2.isStrongNegated) || (predicate_name.compare(p2.predicate_name) != 0)) 
    { 
      return NOT_COMPARABLE; 
    } 
  return (terms.compareTo(p2.terms)); 
} 

ComparisonResult 
Predicate::compareTo(std::vector<Predicate>& preds) 
{ 
  if (preds.size() == 0) 
    { 
      return MORE_GENERAL; 
    } 
  ComparisonResult finalCR = NOT_COMPARABLE;  
  std::vector<Predicate>::iterator p_pos; 
  for (p_pos = preds.begin(); p_pos != preds.end(); p_pos++) 
    {		 
      ComparisonResult cr = compareTo(*p_pos); 
      if (cr == LESS_GENERAL) 
	{ 
	  return LESS_GENERAL; 
	} 
      
      if (cr == MORE_GENERAL) 
	{ 
	  finalCR = MORE_GENERAL; 
	  if (p_pos == preds.begin()) 
	    { 
	      if (preds.size() == 1) 
		{ 
		  preds.clear(); 
		  return MORE_GENERAL; 
		} 
	      else 
		{ 
		  preds.erase(preds.begin()); 
		  p_pos = preds.begin(); 
		} 
	    } 
	  else 
	    { 
	      std::vector<Predicate>::iterator tmp_pos = p_pos; 
	      tmp_pos--; 
	      preds.erase(p_pos); 
	      p_pos = tmp_pos; 
	    } 
	} 
    } 
  return finalCR; 
} 
 
void 
Predicate::rename_terms(std::string& str_id) 
{ 
  terms.rename_terms(str_id);	 
} 
 
Predicate& 
Predicate::operator=(const Predicate& p2) 
{ 
  if (this != &p2) 
    { 
      this->predicate_name = p2.predicate_name; 
      this->terms = p2.terms; 
      this->isStrongNegated = p2.isStrongNegated; 
    } 
  return *this; 
} 
 
Unifier 
Predicate::isUnifiable(const Predicate& p2) 
{ 
  Unifier uni_pp; 
  if ((isStrongNegated == p2.isStrongNegated) && (predicate_name.compare(p2.predicate_name) == 0)) 
    { 
      Terms ts2 = p2.terms; 
      return terms.isUnifiable(ts2); 
    } 
  return uni_pp; 
} 
 
Unifier 
Predicate::isNegatedUnifiable(const Predicate& p2) 
{ 
  Unifier uni_pp; 
  if ((isStrongNegated != p2.isStrongNegated) && (predicate_name.compare(p2.predicate_name) == 0)) 
    { 
      Terms ts2 = p2.terms; 
      return terms.isUnifiable(ts2); 
    } 
  return uni_pp; 
} 
 
void  
Predicate::applyNegation()  
{ 
  isStrongNegated = !isStrongNegated; 
} 
 
std::string  
Predicate::toString()  
{ 
  std::string tmp = ""; 
  if (isStrongNegated)  
    { 
      tmp = "-"; 
    } 
  
  if (!terms.isEmpty())  
    { 
      tmp = tmp + predicate_name + "(" + terms.toString() + ")"; 
    }  
  else  
    { 
      tmp += predicate_name; 
    } 
  return tmp; 
} 

std::string  
Predicate::toStringWithNS()  
{ 
  std::string tmp = ""; 

  if (prefix.compare("") != 0)
    {
      tmp = prefix + '#';
    }

  if (isStrongNegated)  
    { 
      tmp = "-" + tmp; 
    } 
 
  if (!terms.isEmpty())  
    { 
      tmp = tmp + predicate_name + "(" + terms.toString() + ")"; 
    }  
  else  
    { 
      tmp += predicate_name; 
    } 

  return tmp; 
}
 
std::string  
Predicate::toNegatedString()  
{ 
  std::string tmp = ""; 
  if (!isStrongNegated)  
    { 
      tmp = "-"; 
    } 
 
  if (!terms.isEmpty())  
    { 
      tmp = tmp + predicate_name + "(" + terms.toString() + ")"; 
    }  
  else  
    { 
      tmp += predicate_name; 
    } 
  return tmp; 
}

std::string  
Predicate::toNegatedStringWithNS()
{ 
  std::string tmp = "";

  if (prefix.compare("") != 0)
    {
      tmp = prefix + '#';
    }

  if (!isStrongNegated)  
    { 
      tmp = "-" + tmp; 
    } 
 
  if (!terms.isEmpty())  
    { 
      tmp = tmp + predicate_name + "(" + terms.toString() + ")"; 
    }  
  else  
    { 
      tmp += predicate_name; 
    } 

  return tmp; 
} 
 
}	// namespace df 
}	// namespace dlvhex
