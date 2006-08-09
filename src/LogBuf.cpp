/* -*- C++ -*- */

/**
 * @file   LogBuf.cpp
 * @author Thomas Krennwallner
 * @date   Mon May 22 13:04:33 2006
 * 
 * @brief  Logging facility.
 * 
 * 
 */


#include "LogBuf.h"

#include <streambuf>
#include <iosfwd>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <sys/time.h>

using namespace dlvhex::racer;


/// global log ostream
std::ostream dlvhex::racer::log(new LogBuf);


LogBuf::LogBuf(std::ostream* stream)
  : std::streambuf(),
    out(stream)
{ }


LogBuf::~LogBuf()
{ }

std::streamsize
LogBuf::xsputn(const std::streambuf::char_type* s, std::streamsize n)
{
  if (out)
    {
      sstream.write(s,n);
      return n;
    }
  else
    {
      return 0;
    }
 
}

std::streambuf::int_type
LogBuf::overflow(std::streambuf::int_type c)
{
  if (out)
    {
      sstream.put(c);
      return c;
    }
  else
    {
      return traits_type::eof();
    }
}


std::streambuf::int_type
LogBuf::sync()
{
  if (out)
    {
      // don't write anything if buffer is empty
      if (sstream.rdbuf()->in_avail() == 0)
	{
	  return 0;
	}

      // get current time
      struct timeval tv;
      ::gettimeofday(&tv, 0);
      struct tm now;
      ::localtime_r(&tv.tv_sec, &now);

      // first log current time
      *out << std::setw(2) << std::setfill('0') << now.tm_hour << ':'
	   << std::setw(2) << std::setfill('0') << now.tm_min << ':'
	   << std::setw(2) << std::setfill('0') << now.tm_sec << '.' 
	   << std::setw(6) << std::setfill('0') << tv.tv_usec << ' ';
 
      // now append get the current buffer
      std::string s = sstream.str();
      *out << s;

      if (*s.rbegin() != '\n')
	{
	  *out << std::endl;
	}

      sstream.str("");

      return 0;
    }
  else
    {
      return -1;
    }
}
