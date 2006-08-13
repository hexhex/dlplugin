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
#include <ace/Null_Mutex.h>
#include <ace/Singleton.h>

#include <fstream>
#include <string>
#include <iostream>

using namespace dlvhex::util;


const std::string&
UserDir::getUserDirectory()
{
  ///@todo get rid of the ACE_Singleton stuff, but beware we can't
  ///just use a static variable here due to an unforseeable
  ///destruction order during the program exit time -> bad interaction
  ///with Ontology.

  /// pathname of dlvhex user directory
  typedef ACE_Singleton<std::string, ACE_Null_Mutex> UserDirectory;

  if (UserDirectory::instance()->empty())
    {
      // get the homedirectory of the current user and set
      // userDirectory to ~/.dlvhex/

      char user[ACE_MAX_USERID];

      if (ACE_OS::cuserid(user, ACE_MAX_USERID) != 0)
	{
	  struct passwd* pwd = ACE_OS::getpwnam(user);

	  *UserDirectory::instance() = std::string(pwd->pw_dir) + std::string("/.dlvhex/");

	  // just in case ~/.dlvhex is not there
	  ACE_OS::mkdir(UserDirectory::instance()->c_str());
	}
    }

  return *UserDirectory::instance();
}

void
UserDir::create(const std::string& file) const
{
  // create "~/.dlvhex/" + file
  std::string f = getUserDirectory() + file;
  ACE_OS::close(ACE_OS::creat(f.c_str(), S_IRUSR | S_IWUSR));
}

std::string
UserDir::createTemp(const std::string& prefix) const
{
  const char *tmp = ACE_OS::tempnam(getUserDirectory().c_str(), prefix.c_str());
  std::string tmpstr(tmp);
  delete tmp;
  return tmpstr;
}

void
UserDir::open(std::fstream& s, const std::string& file) const
{
  // open "~/.dlvhex/" + file
  std::string f = getUserDirectory() + file;
  s.open(f.c_str());
}

void
UserDir::remove(const std::string& file) const
{
  std::string f;

  if (file[0] != '/' && file.find(getUserDirectory()) != 0)
    {
      // remove "~/.dlvhex/" + file
      f = getUserDirectory() + file;
    }
  else
    {
      f = file;
    }

  ACE_OS::unlink(f.c_str());
}
