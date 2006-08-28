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
#include "LogBuf.h"

#include <ios>
#include <sstream>

#include <errno.h>      // errno & co.
#include <signal.h>     // sigaction()
#include <cstring>      // memset()
#include <sys/types.h>  // 
#include <sys/socket.h> // socket()
#include <unistd.h>     // close()
#include <netdb.h>      // gethostbyname()
#include <sys/select.h> // select()

using namespace dlvhex::util;



TCPIOStream::TCPIOStream(const std::string& host, unsigned port)
  : std::iostream(new TCPStreamBuf(host, port))
{
  exceptions(std::ios_base::badbit); // let TCPStreamBuf throw std::ios_base::failure
}


TCPIOStream::~TCPIOStream()
{
  delete rdbuf();
}


void
TCPIOStream::setConnection(const std::string& host, unsigned port)
{
  delete rdbuf();		       // delete old streambuf
  rdbuf(new TCPStreamBuf(host, port)); // set new streambuf
}


bool
TCPIOStream::isOpen() const
{
  return dynamic_cast<TCPStreamBuf*>(rdbuf())->isOpen();
}


TCPStreamBuf::TCPStreamBuf(const std::string& host,
			   unsigned port,
			   std::streamsize bufsize)
  : std::streambuf(),
    host(host),
    port(port),
    sockfd(-1),
    bufsize(bufsize)
{
  // ignore SIGPIPE
  struct sigaction sa;
  sa.sa_handler = SIG_IGN;

  if (::sigaction(SIGPIPE, &sa, 0))
    {
      ::perror("sigaction");
      ::exit(1);
    }

  initBuffers(); // don't call virtual methods in the ctor
}


TCPStreamBuf::TCPStreamBuf(const TCPStreamBuf& sb)
  : std::streambuf(),
    host(sb.host),
    port(sb.port),
    sockfd(sb.sockfd),
    bufsize(sb.bufsize)
{
  initBuffers(); // don't call virtual methods in the ctor
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
  ::memset(obuf, 0, bufsize);
  ::memset(ibuf, 0, bufsize);
  setp(obuf, obuf + bufsize);
  setg(ibuf, ibuf, ibuf);
}


bool
TCPStreamBuf::isOpen() const
{
  return sockfd != -1;
}


bool
TCPStreamBuf::open()
{
  if (!isOpen())
    {
      //
      // resolve hostname
      //

      struct hostent* he = ::gethostbyname(host.c_str());
      if (he == 0)
	{
	  std::ostringstream oss;
	  oss << "Could not resolve host " << host << ": " << ::hstrerror(h_errno);
	  throw std::ios_base::failure(oss.str());
	}

      //
      // setup connection parameters      
      //

      struct sockaddr_in sa;
      ::memset (&sa, 0, sizeof (struct sockaddr_in));

      sa.sin_family = AF_INET;
      sa.sin_port   = htons(port);
      ::memcpy(&sa.sin_addr, he->h_addr, he->h_length);
      
      struct timeval tv;
      tv.tv_sec = 0;
      tv.tv_usec = 300000;

      // try for at most 3 seconds (approx. 10 rounds)
      for (unsigned i = 0; i < 10; ++i)
	{
	  close();

	  //
	  // setup socket
	  //

	  sockfd = ::socket(PF_INET, SOCK_STREAM, 0);

	  if (sockfd < 0)
	    {
	      ::perror("socket");
	      ::exit(1);
	    }

	  // connect to the TCP server at host:port

	  if (::connect(sockfd, reinterpret_cast<const struct sockaddr *>(&sa), sizeof sa) == 0)
	    {
	      return true; // connection established
	    }

	  // do a nice power napping after an unsuccessful attempt to
	  // open the connection
	  fd_set fs;
	  FD_ZERO(&fs);
	  ::select(0, &fs, &fs, &fs, &tv);
	}

      // connection failed
      std::ostringstream oss;
      oss << "Could not open connection to " << host << ':' << port << '.';
      throw std::ios_base::failure(oss.str());
    }

  return true;
}

 
bool
TCPStreamBuf::close()
{
  int ret = ::close(sockfd);
  sockfd = -1;
  return ret == 0;
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
      *pptr() = traits_type::to_char_type(c);
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
      ssize_t n = ::recv(sockfd, ibuf, bufsize, 0);

      // Nothing is received (n = 0) when RACER query handling
      // timeouts. In this case RACERs only answer is empty and we
      // cannot use the stream any more, so just bail out. Otherwise
      // (n < 0), a failure occured while receiving from the stream
      if (n <= 0)
	{
	  throw std::ios_base::failure("Peer prematurely closed connection.");
	}

      log << "Received: " << std::string(ibuf, n) << std::flush;

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
  ::memset(ibuf, 0, bufsize);
  setg(ibuf, ibuf, ibuf);

  if (pptr() != pbase()) // non-empty obuf -> send data
    {
      errno = 0;

      // loops until whole obuf is sent
      //
      // Warning: when peer disconnects during the sending we receive
      // a SIGPIPE and the default signal handler exits the program.
      // Therefore we have to ignore SIGPIPE (in ctor) and reset the
      // obuf followed by an error return value. See chapter 5.13 of
      // W.R. Stevens: Unix Network Programming Vol.1. FYI: Linux has
      // a MSG_NOSIGNAL flag which does the same, but isn't portable
      // enough...
      ssize_t len = pptr() - pbase();
      ssize_t ret = 0;

      for (ssize_t written = 0;
	   (written < len) || (ret <= 0);
	   written += ret)
	{
	  ret = ::send (sockfd, pbase() + written, len - written, 0);
	}

      log << "Sent: " << std::string(pbase(), pptr() - pbase()) << std::flush;

      // reset output buffer right after sending to the stream
      ::memset(obuf, 0, bufsize);
      setp(obuf, obuf + bufsize);

      if (ret <= 0 || errno == EPIPE) // EOF or failure
	{
	  std::ostringstream oss;
	  oss << "Could not send to peer (errno = " << errno << ").";
	  throw std::ios_base::failure(oss.str());
	}
    }
  
  return 0;
}
