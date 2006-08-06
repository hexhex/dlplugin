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
namespace racer {

  /**
   * @brief Handles files in the dlvhex user directory.
   *
   * A small managing class for handling the files in the dlvhex user
   * directory usually found in ~/.dlvhex
   */
  class UserDir
  {
  private:
    /// pathname of dlvhex user directory
    std::string userDirectory;

  public:
    /// Ctor tries to get the home directory and sets userDirectory
    /// accordingly
    UserDir();

    virtual
    ~UserDir();

    /// create a file in the user directory
    virtual void
    create(const std::string& file) const;

    /// create a temp file inside the user directory with prefix
    virtual std::string
    createTemp(const std::string& prefix) const;

    /// open a file in the user directory
    virtual void
    open(std::fstream& s, const std::string& file) const;

    /// remove a file in the user directory
    virtual void
    remove(const std::string& file) const;
  };

} // namespace racer
} // namespace dlvhex

#endif /* _USERDIR_H */
