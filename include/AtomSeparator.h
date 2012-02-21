/* dlvhex-dlplugin -- Integration of Answer-Set Programming and Description Logics.
 *
 * Copyright (C) 2005, 2006, 2007  Thomas Krennwallner
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
 * @file   AtomSeparator.h
 * @author Thomas Krennwallner
 * @date   Wed Oct 11 13:10:14 2006
 * 
 * @brief  Separates atoms in a string.
 * 
 * 
 */


#ifndef _ATOMSEPARATOR_H
#define _ATOMSEPARATOR_H

#include "DLError.h"

#include <dlvhex2/ComfortPluginInterface.h>

#include <string>
#include <vector>


namespace dlvhex {

//
// forward declaration
//
class AtomSet;


namespace dl {
 
  /**
   * Parse a list of atoms in a string to an AtomSet.
   */
  class AtomSeparator
  {
  private:
    const std::string& atomlist;
    ComfortInterpretation& atoms;

  public:
    AtomSeparator(const std::string& al, ComfortInterpretation& as);

    void
    parse() throw (DLParsingError);
  };
  

  /**
   * Parse a list of lists of atoms in a string to an std::vector<AtomSet>.
   */
  class UnionAtomSeparator
  {
  private:
    const std::string& unionatomlist;
    std::vector<ComfortInterpretation>& atoms;

  public:
    UnionAtomSeparator(const std::string& al, std::vector<ComfortInterpretation>& as);

    void
    parse() throw (DLParsingError);
  };

} // namespace dl
} // namespace dlvhex

#endif /* _ATOMSEPARATOR_H */


// Local Variables:
// mode: C++
// End:
