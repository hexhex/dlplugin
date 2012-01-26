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
 * @file   DFConverter.cpp 
 * @author DAO Tran Minh 
 * @date   Tue Dec 18 17:20:24 2007 
 *  
 * @brief  A converter which converts default rules to dl-rules. 
 *  
 *  
 */ 

#include <iostream>
#include <string>
#include "DFConverter.h"

namespace dlvhex {
namespace df {

  DFConverter::DFConverter()
    : cqmode(false),
      dftrans(0) // default setting: don't rewrite
  { }

  bool
  DFConverter::hasDefaults()
  {
    return DFProcessor::hasDefaults();
  }

  void
  DFConverter::setCQmode(bool cqmode_)
  {
    cqmode = cqmode_;
  }

  void
  DFConverter::setDFTrans(int dftrans_)
    
  { 
    dftrans = dftrans_;
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
    
    for (std::set<ComfortTerm>::iterator pos = inds->begin(); pos != inds->end(); ++pos) 
      {
	const std::string& ind = pos->getString();
	individuals.push_back(ind);
      }
  }
  
  void
  DFConverter::convert(std::istream& is, std::ostream& os)
  {
    if (dftrans == 0) // rewriting turned off
      {
	os << is.rdbuf();
	return;
      }

    std::ostringstream inputcontent;
    std::string line;
    while (!is.eof())
      {
	std::getline(is, line);
	inputcontent << line << std::endl;
      }
    
    DefaultParser dfp;
    BOOST_SPIRIT_CLASSIC_NS :: parse_info<> info = BOOST_SPIRIT_CLASSIC_NS :: parse(inputcontent.str().c_str(),
							    dfp >> BOOST_SPIRIT_CLASSIC_NS :: end_p,
							    BOOST_SPIRIT_CLASSIC_NS :: space_p);
    if (!info.full)
      {
	throw PluginError("Default rules syntax error!");
      }

    std::string other_stuff = DFProcessor::others.str();
    std::string dlprogram;

    if (DFProcessor::hasDefaults())
      {    
	if (!ontology) 
	  {
	    throw PluginError("No ontology specified!");
	  }

	dlprogram = DFProcessor::defaults.getDLRules(cqmode, dftrans).toString();
	
	DLRules rules;
    
	readIndividuals(ontology);
	
	for (std::list<std::string>::iterator i = individuals.begin(); i != individuals.end(); ++i) 
	  {
	    MTerm t(*i);
	    Predicate p_dom("dom", t);
	    DLRule r(p_dom);
	    rules.push_back(r);
	  }
	dlprogram = other_stuff + "\n" + dlprogram + "\n" + rules.toString();
	std::cout << dlprogram << std::endl;
	
	if (dlvhex::dl::Registry::getVerbose() > 1) 
	  {
	    std::cerr << "Transformed dlrules from defaults:" << std::endl;
	    std::cerr << dlprogram << std::endl;
	  }
	
	os << dlprogram;
      }
    else // NO default at all. We just need to switch input and output stream
      {
	os << inputcontent.str();
      }

  }
  
} // namespace df
} // namespace dlvhex
