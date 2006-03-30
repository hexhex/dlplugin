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
#include <ace/Signal.h>
#include <ace/Countdown_Time.h>

using namespace dlvhex::racer;




TCPStreamBuf::TCPStreamBuf(const std::string& host,
			   unsigned port,
			   std::streamsize bufsize)
  : std::streambuf(),
    host(host),
    port(port),
    bufsize(bufsize)
{
  // ignore SIGPIPE
  ACE_Sig_Set sigpipe;
  sigpipe.sig_add(SIGPIPE);
  ACE_Sig_Action siga(SIG_IGN, sigpipe);

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
  ACE_OS::memset(obuf, 0, bufsize);
  ACE_OS::memset(ibuf, 0, bufsize);
  setp(obuf, obuf + bufsize);
  setg(ibuf, ibuf, ibuf);
}


bool
TCPStreamBuf::open()
{
  if (stream.get_handle() == ACE_INVALID_HANDLE)
    {
      ACE_INET_Addr addr(port, host.c_str());
      ACE_SOCK_Connector conn;

      ACE_Time_Value tv(0, 300000);

      ACE_Time_Value timeout(3);

      // retry at most timeout seconds for establishing a connection
      // to the TCP server
      for (ACE_Countdown_Time left(&timeout);
	   timeout > ACE_Time_Value::zero;
	   left.update())
	{
	  close();
      
	  // connect to the TCP server at host:port
	  if (conn.connect(stream, addr) == 0)
	    {
	      return true; // connection established
	    }

	  // do a nice power napping after an unsuccessful attempt to
	  // open the connection
	  ACE_OS::sleep(tv);
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

  if (pptr() >= epptr()) // full obuf -> write buffer
    {
      if (sync() == -1)
	{
	  return traits_type::eof();
	}
    }

  // if c != EOF, put c into output buffer so next call to sync() will
  // write it
  if (!traits_type::eq_int_type(c, traits_type::eof()))
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

  if (gptr() >= egptr()) // empty ibuf -> get data
    {
      if ((egptr() > ibuf) && traits_type::eq(*(egptr() - 1), '\n'))
	{
	  // if last character was a '\n' we are done -> reset buffers
	  sync();
	  return traits_type::eof();
	}

      // try to receive at most bufsize bytes
      ssize_t n = stream.recv(ibuf, bufsize, 0);

      // nothing received or failure
      if (n <= 0)
	{
	  return traits_type::eof();
	}

      setg(ibuf, ibuf, ibuf + n); // set new input buffer boundaries
    }

  return traits_type::to_int_type(*gptr());
}



std::streambuf::int_type
TCPStreamBuf::sync()
{
  if (!open())
    {
      return -1;
    }

  // reset input buffer
  ACE_OS::memset(ibuf, 0, bufsize);
  setg(ibuf, ibuf, ibuf);

  if (pptr() != pbase()) // non-empty obuf -> send data
    {
      errno = ESUCCESS;

      // loops until whole obuf is sent
      //
      // Warning: when peer disconnects during the sending we receive
      // a SIGPIPE and the default signal handler exits the program.
      // Therefore we have to ignore SIGPIPE (in ctor) and reset the
      // obuf followed by an error return value. See chapter 5.13 of
      // W.R. Stevens: Unix Network Programming Vol.1. FYI: Linux has
      // a MSG_NOSIGNAL flag which does the same, but isn't portable
      // enough...
      ssize_t n = stream.send_n(pbase(), pptr() - pbase());

      // reset output buffer right after sending to the stream
      ACE_OS::memset(obuf, 0, bufsize);
      setp(obuf, obuf + bufsize);

      if (n <= 0 || errno == EPIPE) // EOF or failure
	{
	  return -1;
	}
    }
  
  return 0;
}
