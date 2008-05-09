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
 * @file   DFConverter.h
 * @author DAO Tran Minh
 * @date   Mon Mar 17 15:26:09 2008
 * 
 * @brief  Converter from default rules to dl-rules.
 * 
 *
 */

#ifndef _DFCONVERTER_H
#define _DFCONVERTER_H

#include <dlvhex/PluginInterface.h>
#include "Ontology.h"
#include "DefaultParser.h"


namespace dlvhex {
namespace df {
  
  class DFConverter : public PluginConverter
  {
  private:
    // cq mode
    bool cqmode;

    // choose transformation
    int trans;

    // choose to use pruning rules or not
    bool pruning;

    // "default logic" file name
    std::string dfname;
    
    // the transformed dl-program
    std::string program;
    
    // current Ontology
    dlvhex::dl::Ontology::shared_pointer ontology;
    
    // list of individuals in the ontology
    std::vector<std::string> individuals;
    
    void readIndividuals(dlvhex::dl::Ontology::shared_pointer);
    
    std::string& delete_comment(std::string&);
  public:
    DFConverter();

    void
    setCQmode(bool);

    void
    setTrans(int);

    void
    setDFP(bool);
    
    void
    setDefaultFile(std::string);
    
    void
    setOntology(const dlvhex::dl::Ontology::shared_pointer&);

    void
    convert(std::istream& i, std::ostream& o);
  };
  
}}	// namespace dlvhex::df

#endif /* _DFCONVERTER_H */

// Local Variables:
// mode: C++
// End:
