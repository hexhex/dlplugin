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

#include <fstream>
#include <string>

namespace dlvhex {
namespace racer {

  /**
   * @brief Handles files in the user directory.
   */
  class UserDir
  {
  private:
    std::string userDirectory;

  public:
    UserDir();

    virtual
    ~UserDir();

    virtual void
    create(const std::string& file);

    virtual void
    open(std::fstream& s, const std::string& file);

    virtual void
    remove(const std::string& file);
  };

} // namespace racer
} // namespace dlvhex

#endif /* _USERDIR_H */
