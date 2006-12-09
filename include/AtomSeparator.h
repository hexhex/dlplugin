/* -*- C++ -*- */

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

#include <string>
#include <vector>

//
// forward declaration
//
class AtomSet;


namespace dlvhex {
namespace dl {
 
  /**
   * Parse a list of atoms in a string to an AtomSet.
   */
  class AtomSeparator
  {
  private:
    const std::string& atomlist;
    AtomSet& atoms;

  public:
    AtomSeparator(const std::string& al, AtomSet& as);

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
    std::vector<AtomSet>& atoms;

  public:
    UnionAtomSeparator(const std::string& al, std::vector<AtomSet>& as);

    void
    parse() throw (DLParsingError);
  };

} // namespace dl
} // namespace dlvhex

#endif /* _ATOMSEPARATOR_H */
