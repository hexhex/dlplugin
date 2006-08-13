/* -*- C++ -*- */

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
