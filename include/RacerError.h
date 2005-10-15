/* -*- C++ -*- */

/**
 * @file   RacerError.h
 * @author Thomas Krennwallner
 * @date   Sat Jul 16 11:16:33 2005
 * 
 * @brief  RACER exceptions.
 * 
 * 
 */

#ifndef _RACERERROR_H
#define _RACERERROR_H

#include <stdexcept>

namespace dlvhex {
namespace racer {

  /**
   * @brief Base Exception for RACER errors.
   */
  class RacerError : public std::runtime_error
  {
  public:
    explicit
    RacerError(const std::string& arg)
      : std::runtime_error(arg)
    { }
  };


  /**
   * @brief Exception thrown when a parsing error occurs.
   */
  class RacerParsingError : public RacerError
  {
  public:
    explicit
    RacerParsingError(const std::string& arg)
      : RacerError(arg)
    { }
  };


  /**
   * @brief Exception thrown when a building error occurs.
   */
  class RacerBuildingError : public RacerError
  {
  public:
    explicit
    RacerBuildingError(const std::string& arg)
      : RacerError(arg)
    { }
  };

} // namespace racer
} // namespace dlvhex

#endif /* _RACERERROR_H */
