/* dlvhex -- Answer-Set Programming with external interfaces.
 * Copyright (C) 2008 DAO Tran Minh
 * 
 * This file is part of dlvhex-dlplugin.
 *
 * dlvhex is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * dlvhex is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with dlvhex; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */


/**
 * @file   DFOutputBuilder.cpp
 * @author DAO Tran Minh
 * @date   Fri Dec 12 16:24:00 CET 2008
 * 
 * @brief  Builders for default rules output.
 * 
 * 
 */

#include "DFOutputBuilder.h"
#include "dlvhex/ComfortPluginInterface.hpp"

namespace dlvhex {
namespace df {

  DFOutputBuilder::DFOutputBuilder()
  { }

  DFOutputBuilder::~DFOutputBuilder()
  { }  

  void
  DFOutputBuilder::buildResult(std::ostream& stream, const dlvhex::ResultContainer& facts)
  {
    const dlvhex::ResultContainer::result_t& results = facts.getAnswerSets();

    if (results.empty())
      {
	return;
      }

    std::string p1 = std::string(PREFIX_IN) + std::string(PREFIX_NEG);
    std::string p2 = PREFIX_IN;
    std::list<std::string> cnames = DFProcessor::defaults.getConclusionsNames();
    for (dlvhex::ResultContainer::result_t::const_iterator rit = results.begin(); rit != results.end(); ++rit)
    {
      dlvhex::ComfortInterpretation new_as;
      for (dlvhex::ComfortInterpretation::const_iterator i = (*rit)->begin(); i != (*rit)->end(); ++i)
	{
	  //std::cout << i->getPredicate().getString() << std::endl;
	  //std::cout << i->getArguments() << std::endl;
	  std::string pname = i->getPredicate().strval;
	  if (pname.find(p1) == 0)
	    {
	      std::string newname = pname.substr(p1.length());
	      //std::cout << newname << std::endl;
	      dlvhex::AtomPtr ap(new Atom(newname, i->getArguments(), true));
	      new_as.insert(ap);
	    }
	  else
	    {
	      if (pname.find(p2) == 0)
		{
		  std::string newname = pname.substr(p2.length());
		  //std::cout << newname << std::endl;
		  dlvhex::AtomPtr ap(new Atom(newname, i->getArguments()));
		  new_as.insert(ap);
		}
	    }
	}
      dlvhex::RawPrintVisitor rpv(stream);
      new_as.accept(rpv);
      stream << std::endl;      
    }
  }

} // namespace df
} // namespace dlvhex
