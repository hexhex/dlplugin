/* -*- C++ -*- */

/**
 * @file   TCPStream.cpp
 * @author Thomas Krennwallner
 * @date   Sun Jul  3 16:58:32 2005
 * 
 * @brief  Represent a TCP connection to a TCP server via an std::iostream.
 * 
 * 
 */


#include "TCPStream.h"

#include <streambuf>
#include <iosfwd>

#include <ace/SOCK_Connector.h>
#include <ace/SOCK_Stream.h>
#include <ace/INET_Addr.h>

using namespace dlvhex::racer;




TCPStreamBuf::TCPStreamBuf(ACE_SOCK_Stream& stream,
			   std::streamsize bufsize)
  : stream(stream),
    bufsize(bufsize)
{
  initBuffers();
}


TCPStreamBuf::TCPStreamBuf(const TCPStreamBuf& sb)
  : std::streambuf(),
    stream(sb.stream),
    bufsize(sb.bufsize)
{
  initBuffers();
}


TCPStreamBuf::~TCPStreamBuf()
{
  if (ibuf)
    {
      delete[] ibuf;
      ibuf = 0;
    }

  if (obuf)
    {
      delete[] obuf;
      obuf = 0;
    }
}


void
TCPStreamBuf::initBuffers()
{
  obuf = new std::streambuf::char_type[bufsize];
  ibuf = new std::streambuf::char_type[bufsize];
  
  setp(obuf, obuf + bufsize);
  setg(ibuf, ibuf, ibuf);
}


std::streambuf::int_type
TCPStreamBuf::overflow(std::streambuf::int_type c)
{
  if (pptr() >= epptr()) // full obuf
    {
      sync();
    }

  // put c into output buffer so next call to sync() will write it
  if (c != traits_type::eof())
    {
      *pptr() = (char_type) c;
      pbump(1); // increase put pointer by one
    }
  
  return traits_type::not_eof(c);
}


std::streambuf::int_type
TCPStreamBuf::underflow()
{
  if (gptr() >= egptr()) // empty ibuf
    {
      // try to receive len bytes or less
      ssize_t n = stream.recv(ibuf, bufsize, 0);

      // nothing to receive or failure
      if (n <= 0)
	{
	  return traits_type::eof();
	}

      setg(ibuf, ibuf, ibuf + n); // set new input buffer boundaries
    }

  return (streambuf::int_type) (streambuf::char_type) *gptr();
}



std::streambuf::int_type
TCPStreamBuf::sync()
{
  // reset input buffer
  setg(ibuf, ibuf, ibuf);

  if (pptr() != pbase()) // non-empty obuf -> send data
    {
      // loops until whole obuf is sent
      //
      // Warning: when peer disconnects during the sending we receive
      // a SIGPIPE and the default signal handler exits the program.
      // Maybe we should check the connection state before we send any
      // data.
      ssize_t n = stream.send_n(pbase(), pptr() - pbase());

      // reset output buffer right after sending to the stream
      setp(obuf, obuf + bufsize);

      if (n <= 0) // EOF or failure
	{
	  return -1;
	}
    }
  
  return 0;
}
