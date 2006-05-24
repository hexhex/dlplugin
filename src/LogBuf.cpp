/* -*- C++ -*- */

/**
 * @file   LogBuf.cpp
 * @author Thomas Krennwallner
 * @date   Mon May 22 13:04:33 2006
 * 
 * @brief  
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



LogBuf::LogBuf(std::streambuf* sb)
  : std::streambuf(),
    obuf(sb)
{
}


LogBuf::~LogBuf()
{
}

std::streamsize
LogBuf::xsputn(const std::streambuf::char_type* s, std::streamsize n)
{
  if (obuf)
    {
      return sbuf.sputn(s, n);
    }
  else
    {
      return 0;
    }
 
}

std::streambuf::int_type
LogBuf::overflow(std::streambuf::int_type c)
{
  if (obuf)
    {
      return sbuf.sputc(c);
    }
  else
    {
      return traits_type::eof();
    }
}


std::streambuf::int_type
LogBuf::sync()
{
  if (obuf)
    {
      // get current time
      struct timeval tv;
      ::gettimeofday(&tv, 0);
      struct tm now;
      ::localtime_r(&tv.tv_sec, &now);

      // first log current time
      std::ostringstream oss;
      oss << std::setw(2) << std::setfill('0') << now.tm_hour << ':'
	  << std::setw(2) << std::setfill('0') << now.tm_min << ':'
	  << std::setw(2) << std::setfill('0') << now.tm_sec << '.' 
	  << std::setw(6) << std::setfill('0') << tv.tv_usec << ' ';
 
      obuf->sputn(oss.str().c_str(), oss.str().length());

      // now append get the current buffer
      std::string s = sbuf.str();
      if (*s.rbegin() != '\n')
	{
	  s += '\n';
	}
      obuf->sputn(s.c_str(), s.length());
      sbuf.str("");

      // and now write the content
      return obuf->pubsync();
    }
  else
    {
      return -1;
    }
}
