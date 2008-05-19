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
 * @file   DefaultParser.cpp
 * @author DAO Tran Minh
 * @date   Tue Dec 18 17:20:24 2007
 * 
 * @brief  A Parser for a Default of the form [A:B]/[C].
 * 
 * 
 */

#include "Registry.h"
#include "DefaultParser.h"

#include <dlvhex/Error.h>
#include <iostream>
#include <fstream>
#include <sstream>

namespace dlvhex { 
namespace df {

DefaultParser::DefaultParser() 
{ }

std::string 
DefaultParser::get_predicate_name(v_t_nvd::iterator pos) 
{
  std::string pn = "";
  v_t_nvd::iterator pos1;
  
  if (pos->children.size() == 0) 
    {
      pn = *pos->value.begin();
    } 
  else 
    {
      for (pos1 = pos->children.begin(); pos1 != pos->children.end(); ++pos1) 
	{
	  pn += *pos1->value.begin();
	}
    }
  return pn;
}

std::string 
DefaultParser::get_term(v_t_nvd::iterator pos) 
{
  v_t_nvd::iterator pos1;
  std::string t;

  if (pos->children.size() == 0) 
    {
      // single name
      t = *pos->value.begin();
    } 
  else 
    {
      // not a single name
      t = "";
      for (pos1 = pos->children.begin(); pos1 != pos->children.end(); pos1++) 
	{
	  t += *pos1->value.begin();
	}
    }
  return t;
}

Terms
DefaultParser::get_terms(v_t_nvd::iterator pos) 
{
  v_t_nvd::iterator pos1;
  std::string t_name;
  Terms ts;
  
  if (pos->children.size() == 0) 
    {
      // The simplest case ..x..
      t_name = *pos->value.begin();
      MTerm t(t_name);
      ts.addTerm(t);
    }
  else 
    {
      pos1 = pos->children.begin();
      ++pos1;
      if (*pos1->value.begin() == ',') 
	{
	  // The most complicated case ..XYZ, "ABC"...
	  --pos1;
	  do 
	    {
	      t_name = get_term(pos1);
	      MTerm t(t_name);
	      ts.addTerm(t);
	      ++pos1;
	      if (pos1 == pos->children.end()) break;
	      ++pos1;
	    } 
	  while (true);
	}
      else 
	{
	  // The second simplest case ..XYZ..			
	  t_name = get_term(pos);
	  MTerm t(t_name);
	  ts.addTerm(t);
	}
    }
  return ts;
}

Predicate
DefaultParser::get_predicate(v_t_nvd::iterator pos, Prefixes& ps) 
{
  v_t_nvd::iterator pos1;
  bool neg = false;
  std::string pn;
  Terms ts;
  
  if (pos->children.size() == 0) 
    {
      // Can be strong negation or a single name
      if (*pos->value.begin() == '-') 
	{
	  neg = true;
	  pos++;
	  pos1 = pos->children.begin();
	  pn = get_predicate_name(pos1);
	  pos1++;
	  pos1++;
	  ts = get_terms(pos1);
	} 
      else 
	{
	  pn = get_predicate_name(pos);
	  pos++;
	  pos++;
	  ts = get_terms(pos);
	}
    } 
  else 
    {
      // Must be a plural name
      pn = get_predicate_name(pos);
      pos++;	// go to children[2] to get terms
      pos++;
      ts = get_terms(pos);
    }
  // Now check if we have a prefix or not
  std::string::size_type sep = pn.find(":");
  if (sep != std::string::npos)
    {
      std::string name_ = pn.substr(0, sep);
      std::string path_ = ps.getPath(name_);
      if (path_.compare("") != 0)
	{
	  pn = pn.substr(++sep);
	  Predicate p(neg, pn, path_, ts);
	  return p;
	}
      else
	{
	  std::cout << "Warning: namespace " << name_ << " not found. Ignore. " <<std::endl;
	  pn = pn.substr(++sep);
	}
    }
	
  Predicate p(neg, pn, ts);	
  return p;
}

Pred1Dim
DefaultParser::analyze_conjunction(v_t_nvd& children_, Prefixes& ps) 
{
  Pred1Dim p_list;
  v_t_nvd::iterator pos;
  
  pos = children_.begin();
  while (pos->children.size() > 0) 
    {
      pos++;
    }
  if (*pos->value.begin() == '&' || *pos->value.begin() == ',') 
    {
      // conjunction or list of literals
      pos = children_.begin();
      do 
	{
	  Predicate p = get_predicate(pos->children.begin(), ps);
	  p_list.push_back(p);
	  pos++;
	  if (pos == children_.end()) break;
	  pos++;
	} 
      while (true);
    } 
  else 
    {
      // single literal
      Predicate p = get_predicate(children_.begin(), ps);
      p_list.push_back(p);
    }
  return p_list;
}

Pred2Dim
DefaultParser::analyze_justification(v_t_nvd& children_, Prefixes& ps) 
{
  Pred2Dim c_list;
  v_t_nvd::iterator pos;
  
  pos = children_.begin();
  while (pos->children.size() > 0) 
    {
      pos++;
    }		
  if (*pos->value.begin() == ',') 
    {
      //std::cout << "list of conjuction" << std::endl;
      pos = children_.begin();
      do 
	{
	  Pred1Dim c = analyze_conjunction(pos->children, ps);				
	  c_list.push_back(c);
	  pos++;
	  if (pos == children_.end()) break;
	  pos++;
	} 
      while (true);
    } 
  else 
    {
      //std::cout << "only one conjunction" << std::endl;
      Pred1Dim c = analyze_conjunction(children_, ps);
      c_list.push_back(c);
    }
  return c_list;
}

Default
DefaultParser::getDefault(v_t_nvd& branch, Prefixes& ps) 
{
  Pred1Dim premise;
  Pred2Dim justification;
  Pred1Dim conclusion;	
  
  switch (branch.size())
    {
    case 9:	
      // [A1 & ... & An : B1,...,Bk ]/[C].
      premise = analyze_conjunction(branch[1].children, ps);
      justification = analyze_justification(branch[3].children, ps);
      conclusion = analyze_conjunction(branch[7].children, ps);
      break;
    case 8:
      // [: B1,...,Bk ]/[C].
      if (std::string(branch[1].value.begin(), branch[1].value.end()) == ":") 
	{
	  justification = analyze_justification(branch[2].children, ps);
	  conclusion = analyze_conjunction(branch[6].children, ps);
	} 
      else 
	{
	  // or
	  // [A1 & ... & An:]/[C].			
	  premise = analyze_conjunction(branch[1].children, ps);
	  conclusion = analyze_conjunction(branch[6].children, ps);
	}			
      break;
    case 7:
      // [:]/[C].
      conclusion = analyze_conjunction(branch[5].children, ps);
      break;		
    }
  Default df(premise, justification, conclusion);	
  return df;
}

Prefix
DefaultParser::getPrefix(v_t_nvd& branch)
{
  std::string name_ = "";
  std::string path_ = "";
  
  v_t_nvd namenode = branch[3].children;
  v_t_nvd::iterator pos;
  for (pos = namenode.begin(); pos != namenode.end(); ++pos)
    {
      name_ += *pos->value.begin();
    }
  
  pos = branch.begin();
  for (int i = 0; i < 7; ++i)
    {
      ++pos;
    }
  v_t_nvd::iterator pos_end = branch.end();
  --pos_end;
  --pos_end;
  
  for (; pos != pos_end; ++pos)
    {
      path_ += *pos->value.begin();
    }
  Prefix p(name_, path_);
  return p;
}

void
DefaultParser::evaluateDefaults(const iter_t& root, Prefixes& ps, Defaults& dfs)
{
  int n = root->children.size();
  for (int i = 0; i < n; ++i)
    {
      v_t_nvd branch = root->children[i].children;
      if (*branch[0].value.begin() == '#')
	{
	  Prefix p = getPrefix(branch);
	  ps.addPrefix(p);
	}
      else
	{
	  Default df = getDefault(branch, ps);
	  dfs.addDefault(df);		
	}		
    }
}

void
DefaultParser::parseInputStream(std::string& filename, std::string& program, bool cqmode, int trans, bool pruning)
{
  std::ifstream iss;
  iss.open(filename.c_str());
	
  if (!iss.is_open()) 
    {
      throw PluginError("Could not open " + filename);
    }

  parseInputStream(iss, program, cqmode, trans, pruning);
}

std::string&
DefaultParser::delete_comment(std::string& s)
{
  ///@todo this is non-functional, consider a URI with escapecodes like "http://foobar.com/foo%20bar"
  std::string::size_type pos = s.find('%');
  if (pos != std::string::npos) 
    {
      s.erase(pos);
    }
  return s;
}

void
DefaultParser::parseInputStream(std::istream& iss, std::string& program, bool cqmode, int trans, bool pruning)
{
  std::ostringstream dfcontent;
  std::string line;
  
  while (!iss.eof()) 
    {
      std::getline(iss, line);
      dfcontent << delete_comment(line) << std::endl;
    }
  
  if (dlvhex::dl::Registry::getVerbose() > 1) 
    {
      std::cerr << "Input defaults:" << std::endl;
      std::cerr << dfcontent.str() << std::endl;
    }
  
  default_p df_p;
  boost::spirit::tree_parse_info<> info = boost::spirit::ast_parse(dfcontent.str().c_str(),
								   df_p, boost::spirit::space_p);

  if (info.full)
    {
      if (dlvhex::dl::Registry::getVerbose() > 1)
	{
	  std::cerr << "Parsing succeeded" << std::endl;
	}
      Defaults dfs;
      Prefixes ps;
      evaluateDefaults(info.trees.begin(), ps, dfs);
      DLRules dlrs = dfs.getDLRules(cqmode, trans, pruning);
      program = dlrs.toString();
    }
  else 
    {
      std::ostringstream oss;
      oss << "Parsing of the default rules failed at position " << info.length;
      throw PluginError(oss.str());
    }
}

} // namespace df
} // namespace dlvhex
