/* -*- C++ -*- */

/**
 * @file   UserDir.cpp
 * @author Thomas Krennwallner
 * @date   Sun Oct 16 15:49:11 2005
 * 
 * @brief  Simple file handling for the user directory.
 * 
 * 
 */

#include "UserDir.h"

#include <ace/OS.h>

#include <fstream>
#include <iostream>
#include <string>

using namespace dlvhex::racer;


UserDir::UserDir()
{
  char user[ACE_MAX_USERID];

  if (ACE_OS::cuserid(user, ACE_MAX_USERID) != 0)
    {
      struct passwd* pwd = ACE_OS::getpwnam(user);

      userDirectory = std::string(pwd->pw_dir) + std::string("/.dlvhex/");

      ACE_OS::mkdir(userDirectory.c_str());
    }
}

UserDir::~UserDir()
{
}

void
UserDir::create(const std::string& file)
{
  std::string f = userDirectory + file;
  ACE_OS::close(ACE_OS::creat(f.c_str(), S_IRUSR | S_IWUSR));
}

void
UserDir::open(std::fstream& s, const std::string& file)
{
  std::string f = userDirectory + file;
  s.open(f.c_str());
}

void
UserDir::remove(const std::string& file)
{
  std::string f = userDirectory + file;
  ACE_OS::unlink(f.c_str());
}
