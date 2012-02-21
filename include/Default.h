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
 * @file   Default.h
 * @author DAO Tran Minh 
 * @date   Tue Dec 18 17:20:24 2007 
 *  
 * @brief  A Default of the form [A;B]/[C]. 
 *  
 *  
 */ 

#ifndef _DLVHEX_DF_DEFAULT_H_ 
#define _DLVHEX_DF_DEFAULT_H_ 

#include <list>
#include <dlvhex2/PluginInterface.h>
#include "Defaults.h"
#include "DLRules.h"
#include "Pred2Dim.h"
#include "Updates.h"

#define PREFIX_IN	"in_" 
#define	PREFIX_PRED	"p_"
#define	PREFIX_CONS	"cons_"
#define PREFIX_INCONS   "incons_"
#define	PREFIX_OUT	"out_"

namespace dlvhex {
namespace df {

  class Defaults;

  class Default
  {
  private:
    Defaults* parent;
    unsigned int id;
    Pred1Dim prerequisite;
    Pred2Dim justifications;
    Pred1Dim conclusion;
    Predicate argument;
    
    DLRules
      getFacts_Pi();

    DLRules
      getFacts_Omega_Upsilon();

    DLRules
      additionalDLRules4Conclusion(Pred1Dim&, Predicate&, std::string prefix = PREFIX_IN);

    DLAtoms
      getDLBodies(Updates&, Pred1Dim&, bool);

  public:
    Default(Pred1Dim, Pred2Dim, Pred1Dim);
    Default(Pred1Dim, Pred2Dim, Pred1Dim, Predicate);
    
    unsigned int
      getID();
    
    void
      setID(unsigned int);

    void
      setParent(Defaults*);

    /** 
     * @return An Updates to build \lambda 
     *         \union(concept/role +=/-= (prefix)_concept/p_role) 
     *         for all concepts/roles in the conclusion 
     */ 
    Updates 
      getUpdates(std::string); 

    DLRules
      getDLRules_Pi(bool);

    DLRules
      getDLRules_Omega(bool);

    DLRules
      getDLRules_Upsilon(bool);

    std::list<std::string>
      getConclusionNames();

    std::string 
      toString();
  };
  
} // namespace df
} // namespace dlvhex

#endif /* _DLVHEX_DF_DEFAULT_H */
