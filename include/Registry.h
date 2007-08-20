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
 * @file   Registry.h
 * @author Thomas Krennwallner
 * @date   Tue May 30 11:53:17 2006
 * 
 * @brief  Global state.
 * 
 * 
 */


#ifndef _REGISTRY_H
#define _REGISTRY_H

#include <string>

namespace dlvhex {
namespace dl {

  /**
   * @brief The Registry keeps track of global state.
   */
  class Registry
  {
  private:
    /// setup DL-reasoner according to these flags
    static unsigned flags;
    /// verbosity level
    static unsigned verbose;
    /// did we set UNA?
    static bool una;
    /// did we set datasubstrate mirroring?
    static bool datasubstrate;

    /// pure virtual dtor, we don't want an instance or a child
    virtual
    ~Registry() = 0;

  public:
    /// flags for the Registry
    enum
      {
	UNA = 0x1
      };

    static void
    setFlags(unsigned);

    static unsigned
    getFlags();

    static unsigned
    getVerbose();

    static void
    setVerbose(unsigned);

    static bool
    getUNA();

    static void
    setUNA(bool);

    static bool
    getDataSubstrateMirroring();

    static void
    setDataSubstrateMirroring(bool);
  };

} // namespace dl
} // namespace dlvhex

#endif /* _REGISTRY_H */


// Local Variables:
// mode: C++
// End:
