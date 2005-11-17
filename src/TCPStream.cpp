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
#include <ace/OS.h>

using namespace dlvhex::racer;




TCPStreamBuf::TCPStreamBuf(const std::string& host,
			   unsigned port,
			   std::streamsize bufsize)
  : host(host),
    port(port),
    bufsize(bufsize)
{
  initBuffers();
}


TCPStreamBuf::TCPStreamBuf(const TCPStreamBuf& sb)
  : std::streambuf(),
    host(sb.host),
    port(sb.port),
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

  close();
}


void
TCPStreamBuf::initBuffers()
{
  obuf = new std::streambuf::char_type[bufsize];
  ibuf = new std::streambuf::char_type[bufsize];
  
  setp(obuf, obuf + bufsize);
  setg(ibuf, ibuf, ibuf);
}


bool
TCPStreamBuf::open()
{
  if (stream.get_handle() == ACE_INVALID_HANDLE)
    {
      ACE_Time_Value tv(0, 300000);

      // retry until TCP server is up and running
      for (int i = 10; i > 0; i--)
	{
	  ACE_OS::sleep(tv);
	  
	  close();
      
	  // connect to the TCP server at host:port

	  ACE_INET_Addr addr(port, host.c_str());
	  ACE_SOCK_Connector conn;

	  if (conn.connect(stream, addr) == 0)
	    {
	      return true; // connection established
	    }
	}

      return false; // connection failed
    }

  return true;
}


bool
TCPStreamBuf::close()
{
  return !stream.close();
}


std::streambuf::int_type
TCPStreamBuf::overflow(std::streambuf::int_type c)
{
  if (!open())
    {
      return traits_type::eof();
    }

  if (pptr() >= epptr()) // full obuf
    {
      sync(); // check return value?
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
  if (!open())
    {
      return traits_type::eof();
    }

  if (gptr() >= egptr()) // empty ibuf
    {
      // try to receive at most bufsize bytes
      ssize_t n = stream.recv(ibuf, bufsize, 0);

      // nothing received or failure
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
  if (!open())
    {
      return -1;
    }

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
