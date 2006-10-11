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
  

} // namespace dl
} // namespace dlvhex

#endif /* _ATOMSEPARATOR_H */
