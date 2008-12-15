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
 * @file   DFProcessor.cpp 
 * @author DAO Tran Minh 
 * @date   Tue Dec 18 17:20:24 2007 
 *  
 * @brief  The processor which contains the parser for default rules. 
 *  
 *  
 */ 

#include <algorithm>
#include <iostream>

#include "DFProcessor.h"

namespace dlvhex {
namespace df {

  std::list<MTerm> DFProcessor::lst_term;
  std::list<Predicate> DFProcessor::lst_predicate;
  std::list<Pred1Dim> DFProcessor::lst_justification;
  Pred1Dim DFProcessor::prerequisite;
  Pred1Dim DFProcessor::conclusion;
  Pred2Dim DFProcessor::justifications;
  Predicate DFProcessor::argument;
  Defaults DFProcessor::defaults;
  std::ostringstream DFProcessor::others;
  Prefixes DFProcessor::namespaces;

  bool DFProcessor::hasDefaults()
  {
    return (defaults.size() > 0);
  }

  void DFProcessor::push_namespace(const char* first, const char* last)
  {
    std::string s(first, last);
    std::size_t i = s.find("(");
    std::size_t j = s.find(")");
    s = s.substr(++i, j-i-1);
    //std::cout << s << std::endl;
    i = s.find(",");
    j = s.find_last_of("\"");
    std::string t = s.substr(i+2, j-i-2);
    s = s.substr(1, i-2);
    //std::cout << s << std::endl;
    //std::cout << t << std::endl;
    Prefix ns(s, t);
    namespaces.push_back(ns);
    push_others(first, last);
  }

  void DFProcessor::push_others(const char* first, const char* last)
  {
    std::string str(first, last);
    others << str;
    //std::cout << "passing through: " << str << std::endl;    
  }

  void DFProcessor::push_lst_term(const char* first, const char* last)
  {
    std::string str(first, last);
    //std::cout << str << std::endl;
    MTerm t(str);
    lst_term.push_back(t);
  }
  
  void DFProcessor::push_lst_literal(const char* first, const char* last)
  {
    bool isStrongNegated = false;

    Terms ts(lst_term);
    lst_term.clear();
    std::string s(first, last);
    std::string prefix = "";
    std::size_t i = s.find('(');
    if (*first == '-')
      {
	isStrongNegated = true;	
	s = s.substr(1, --i);
      }
    else
      {
	s = s.substr(0, i);
      }
    i = s.find(":");
    if (i != std::string::npos)
      {
	prefix = s.substr(0, i);
	//prefix = namespaces.getPath(prefix);
	s = s.substr(i+1);
      }
    
    //std::cout << s << std::endl;
    //std::cout << prefix << std::endl;
    //std::cout << ts.toString() << std::endl;
    Predicate p(isStrongNegated, s, prefix, ts);
    //std::cout << p.toStringWithNS() << std::endl;
    lst_predicate.push_back(p);
  }

  void DFProcessor::push_lst_justification(const char* first, const char* last)
  {
    Pred1Dim j(lst_predicate);
    lst_predicate.clear();
    lst_justification.push_back(j);
  }

  void DFProcessor::set_prerequisite(const char* first, const char* last)
  {
    prerequisite.setContent(lst_predicate);
    lst_predicate.clear();
    //std::cout << "Set prerequisite: " << prerequisite.toString() << std::endl;
  }

  void DFProcessor::set_conclusion(const char* first, const char* last)
  {
    conclusion.setContent(lst_predicate);
    lst_predicate.clear();
    //std::cout << "Set conclusion: " << conclusion.toString() << std::endl;
  }

  void DFProcessor::set_justifications(const char*, const char*)
  {
    justifications.setContent(lst_justification);
    lst_justification.clear();
    //std::cout << "Set justification(s): " << justifications.toString() << std::endl;
  }

  void DFProcessor::set_argument(const char* first, const char* last)
  {
    Terms ts(lst_term);
    lst_term.clear();
    std::string s(first, last);
    std::size_t i = s.find('(');
    s = s.substr(1, --i);
    //std::cout << s << std::endl;
    //std::cout << ts.toString() << std::endl;
    Predicate p(s, ts);
    argument = p;
    argument.setNULL(false);
    //std::cout << "Set argument: " << argument.toString() << std::endl;
  }

  void DFProcessor::push_default(const char* first, const char* last)
  {
    if (!argument.isNULL())
      {
	Default df(prerequisite, justifications, conclusion, argument);
	defaults.push_back(df);
	argument.setNULL(true);
      }
    else
      {
	Default df(prerequisite, justifications, conclusion);
	defaults.push_back(df);
      }
    //std::cout << "Push default: " << df.toString() << std::endl;
    
  }
    
} // namespace df
} // namespace dlvhex
