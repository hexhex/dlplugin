/* -*- C++ -*- */

/**
 * @file   UserDir.h
 * @author Thomas Krennwallner
 * @date   Sun Oct 16 14:48:37 2005
 * 
 * @brief  Simple file handling for the user directory.
 * 
 * 
 */


#ifndef _USERDIR_H
#define _USERDIR_H

#include <iosfwd>
#include <string>

namespace dlvhex {
namespace util {

  /**
   * @brief Handles files in the dlvhex user directory.
   *
   * A small managing class for handling the files in the dlvhex user
   * directory usually found in ~/.dlvhex
   */
  class UserDir
  {
  public:
    /// tries to get the home directory and setups the user directory
    /// @return absolute pathname of the user directory
    static const std::string&
    getUserDirectory();

    /// create @a file in the user directory
    void
    create(const std::string& file) const;

    /// create a temp file inside the user directory with @a prefix
    /// @return filename for the temporary file
    std::string
    createTemp(const std::string& prefix) const;

    /// open @a file in the user directory
    void
    open(std::fstream& s, const std::string& file) const;

    /// remove @a file in the user directory
    void
    remove(const std::string& file) const;
  };

} // namespace util
} // namespace dlvhex

#endif /* _USERDIR_H */
