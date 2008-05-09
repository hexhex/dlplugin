/* dlvhex-dfplugin -- Integration of Answer-Set Programming and Description Logics.
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
 * @file   DFConverter.cpp
 * @author DAO Tran Minh
 * @date   Mon Mar 17 15:47:24 2008
 * 
 * @brief  Converter from default rules to dl-rules.
 * 
 *
 */

#include "DFConverter.h"
#include "Registry.h"

#include <dlvhex/Error.h>

namespace dlvhex {
namespace df {

DFConverter::DFConverter()
{
  cqmode = true;
  trans = 2;
  pruning = true;
}

void
DFConverter::setCQmode(bool cqmode_)
{
  cqmode = cqmode_;
}

void
DFConverter::setTrans(int trans_)
{
  trans = trans_;
}

void
DFConverter::setDFP(bool pruning_)
{
  pruning = pruning_;
}

void
DFConverter::setDefaultFile(const std::string dfname_)
{
  dfname = dfname_;
}

void
DFConverter::setOntology(const dlvhex::dl::Ontology::shared_pointer& o)
{
  ontology = o;
}

void 
DFConverter::readIndividuals(dlvhex::dl::Ontology::shared_pointer o) 
{
  dlvhex::dl::ABox abox = o->getABox();
  dlvhex::dl::ABox::ObjectsPtr inds = abox.getIndividuals();

  for (std::set<Term>::iterator pos = inds->begin(); pos != inds->end(); ++pos) 
    {
      const std::string& ind = pos->getString();
      individuals.push_back(ind);
    }
}

std::string& 
DFConverter::delete_comment(std::string& s) 
{
  int pos = s.find("%", 0);
  if (pos > -1) 
    {
      s.erase(pos, s.length());
    }
  return s;
}

void
DFConverter::convert(std::istream& i, std::ostream& o)
{
  DefaultParser dp;
  std::string program;

  dp.parseInputStream(dfname, program, cqmode, trans, pruning);

  if (!ontology) 
    {
      throw PluginError("No ontology specified!");
    }

  DLRules rules;

  readIndividuals(ontology);

  std::vector<std::string>::iterator pos_i;

  for (pos_i = individuals.begin(); pos_i != individuals.end(); pos_i++) 
    {
      MTerm t(*pos_i);
      Predicate p("dom", t);
      DLRule r(p);
      rules.addDLRule(r);
    }

  program = program + rules.toString();		
  std::cout << program << std::endl;
  
  if (dlvhex::dl::Registry::getVerbose() > 1) 
    {
      std::cerr << "Transformed dlrules from defaults:" << std::endl;
      std::cerr << program << std::endl;
    }
  
  o << i.rdbuf();
  o << program;
}

}} // namespace dlvhex::df
