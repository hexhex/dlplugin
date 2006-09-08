/* -*- C++ -*- */

/**
 * @file   RacerKBManager.cpp
 * @author Thomas Krennwallner
 * @date   Mon Aug 14 11:41:57 2006
 * 
 * @brief  KB Manager class for RACER KBs.
 * 
 * 
 */

#include "RacerKBManager.h"
#include "KBManager.h"
#include "QueryDirector.h"
#include "RacerBuilder.h"
#include "RacerAnswerDriver.h"

#include <iostream>
#include <string>
#include <sstream>
#include <iterator>

#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace dlvhex::dl::racer;


namespace dlvhex {
  namespace dl {
    namespace racer {

    }
  }
}

RacerKBManager::RacerKBManager(std::iostream& s, const std::string& name)
  : KBManager(name), stream(s)
{
  // If name is empty, we create a unique KB name with help of
  // tmpfile(3). This call creates a unique temporary file and
  // immediately unlinks it s.t. it is automatically deleted when we
  // close it or the program is terminated. So we keep it always open
  // and get the inode number from the file descriptor which is a
  // system-wide unique identifier suitable for a unique KB name.
  if (name.empty())
    {
      // create a temporary file
      std::FILE* fp = std::tmpfile();

      // get file descriptor
      int fd = ::fileno(fp);
      struct stat stbuf;
      ::fstat(fd, &stbuf);
      
      // and now set #kbName to KB-inode, where inode is the inode
      // number of the prev. created unique file
      std::ostringstream oss;
      oss << "KB-" << (unsigned) stbuf.st_ino;
      
      kbName = oss.str();
    }
}

void
RacerKBManager::removeKB() throw (DLError)
{
  try
    {
      // send the (forget-abox) command to RACER
      RacerForgetABoxCmd cmd;
      stream << cmd(*this) << std::endl;
      RacerIgnoreAnswer p(stream);
      Answer a(0);
      p.parse(a);
    }
  catch (std::exception& e)
    {
      throw DLError(e.what());
    }
}


void
RacerKBManager::updateOpenKB() throw (DLError)
{
  try
    {
      // send the (all-tboxes) command to RACER
      RacerAllTBoxesCmd cmd;
      stream << cmd() << std::endl;

      // parse the list of open TBoxes
      RacerAnswerDriver p(stream);
      Answer a(0);
      p.parse(a);

      // and now set #openKBs
      openKBs.clear();
      std::insert_iterator<KBSet> ii = std::inserter(openKBs, openKBs.begin());

      for (std::vector<Tuple>::const_iterator it1 = a.getTuples()->begin();
	   it1 != a.getTuples()->end(); ++it1)
	{
	  for (Tuple::const_iterator it2 = it1->begin(); it2 != it1->end(); ++it2, ++ii)
	    {
	      *ii = it2->getUnquotedString();
	    }
	}
    }
  catch (std::exception& e)
    {
      throw DLError(e.what());
    }
}
