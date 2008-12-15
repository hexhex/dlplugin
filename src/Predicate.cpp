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

#include <iostream> 
#include "Predicate.h" 
 
namespace dlvhex { 
namespace df { 
 
  Predicate::Predicate() 
  { isNull = true; } 
  
  Predicate::Predicate(const std::string& predicate_name_) 
    : isNull(false), isStrongNegated(false), predicate_name(predicate_name_), prefix("")
  { } 
  
  Predicate::Predicate(const std::string& predicate_name_, const Terms& terms_) 
    : isNull(false), isStrongNegated(false), predicate_name(predicate_name_), terms(terms_), prefix("")
  { } 
  
  Predicate::Predicate(const std::string& predicate_name_, const MTerm& term_) 
    : isNull(false), isStrongNegated(false), predicate_name(predicate_name_), prefix("")
  { 
    terms.push_back(term_); 
  } 
  
  Predicate::Predicate(bool isStrongNegated_, const std::string& predicate_name_) 
    : isNull(false), isStrongNegated(isStrongNegated_), predicate_name(predicate_name_), prefix("") 
  { } 
  
  Predicate::Predicate(bool isStrongNegated_, const std::string& predicate_name_, const Terms& terms_)
  : isNull(false), isStrongNegated(isStrongNegated_), predicate_name(predicate_name_), prefix(""), terms(terms_) 
  { }
  
  Predicate::Predicate(bool isStrongNegated_, const std::string& predicate_name_, const std::string& prefix_, const Terms& terms_)
  : isNull(false), isStrongNegated(isStrongNegated_), predicate_name(predicate_name_), prefix(prefix_), terms(terms_) 
  { }
  
  bool
  Predicate::isNULL()
  {
    return isNull;
  }

  void
  Predicate::setNULL(bool isNull_)
  {
    isNull = isNull_;
  }
  
  bool  
  Predicate::isStronglyNegated()  
  { 
    return isStrongNegated; 
  } 

  bool
  Predicate::gotThisTerm(MTerm& term_)
  {
    return terms.gotThisTerm(term_);
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
	tmp = PREFIX_NEG; 
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
  
} // namespace df 
} // namespace dlvhex
