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
 * @file   DLError.h
 * @author Thomas Krennwallner
 * @date   Sat Jul 16 11:16:33 2005
 * 
 * @brief  DL plugin exceptions.
 * 
 * 
 */

#ifndef _DLERROR_H
#define _DLERROR_H

#include <stdexcept>

namespace dlvhex {
namespace dl {

  /**
   * @brief Base Exception for DL plugin errors.
   */
  class DLError : public std::runtime_error
  {
  public:
    explicit
    DLError(const std::string& arg)
      : std::runtime_error(arg)
    { }
  };


  /**
   * @brief Exception thrown when a parsing error occurs.
   */
  class DLParsingError : public DLError
  {
  public:
    explicit
    DLParsingError(const std::string& arg)
      : DLError(arg)
    { }
  };


  /**
   * @brief Exception thrown when a building error occurs.
   */
  class DLBuildingError : public DLError
  {
  public:
    explicit
    DLBuildingError(const std::string& arg)
      : DLError(arg)
    { }
  };

} // namespace dl
} // namespace dlvhex

#endif /* _DLERROR_H */


// Local Variables:
// mode: C++
// End:
