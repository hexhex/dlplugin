/* -*- C++ -*- */

/**
 * @file   TCPStream.h
 * @author Thomas Krennwallner
 * @date   Sun Jul  3 16:56:52 2005
 * 
 * @brief  Represent a TCP connection to a TCP server via an std::iostream.
 * 
 * 
 */

#ifndef _TCPSTREAM_H
#define _TCPSTREAM_H

#include <string>
#include <streambuf>
#include <iostream>

#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Connector.h>

namespace dlvhex {
namespace racer {

  /**
   * @brief provides input/output buffering for the network connection.
   */
  class TCPStreamBuf : public std::streambuf
  {
  private:
    const std::string host;
    const unsigned port;
    
    /// TCP Socket abstraction
    ACE_SOCK_Stream stream;

    /// size of i/o sequences
    std::streamsize bufsize;

    /// output character sequence
    std::streambuf::char_type* obuf;
    /// input character sequence
    std::streambuf::char_type* ibuf;

    /**
     * Allocates the buffers at initialization time.
     */
    virtual void
    initBuffers();


  protected:
    /**
     * Called when output buffer is full.
     *
     * @param c
     *
     * @return traits::not_eof(c) on success, otherwise traits::eof().
     */
    virtual std::streambuf::int_type
    overflow(std::streambuf::int_type c);

    /**
     * Called when input buffer is empty.
     *
     * @return first character of pending input or traits::eof() for
     * no pending data.
     */
    virtual std::streambuf::int_type
    underflow();

    /**
     * Called when output buffer needs syncronization and resets the
     * input buffer.
     *
     * @return -1 on failure.
     */
    virtual std::streambuf::int_type
    sync();

  public:
    /**
     * Ctor.
     *
     * @param host connect to this host
     * @param port connect to this port
     * @param bufsize buffer bufsize bytes
     */
    explicit
    TCPStreamBuf(const std::string& host,
		 unsigned port,
		 std::streamsize bufsize = 256);

    /// Copy Ctor
    TCPStreamBuf(const TCPStreamBuf& sb);

    /// Dtor
    virtual
    ~TCPStreamBuf();

    /**
     * Connects the underlying stream so sb can send and receive data.
     *
     * @return false on error, true on success.
     */
    virtual bool
    open();

    /**
     * Closes the underlying stream.
     *
     * @return false on error, true on success.
     */
    virtual bool
    close();

  };


  /**
   * @brief An IOStream to a TCP server.
   *
   * Instead of using TCPStreamBuf we could inherit from
   * ACE_IOStream<ACE_SOCK_Stream> and use automatically ACE_Streambuf
   * but for unknown reasons it is disabled on Linux and we have to do
   * the whole buffering ourselves.
   */
  class TCPIOStream : public std::iostream
  {
  private:
    /// buffered IO
    TCPStreamBuf sb;

  public:
    /// Default Ctor
    TCPIOStream(const std::string& host, unsigned port)
      : std::iostream(&sb),
	sb(host, port)
    {
      ///@todo hm, when we turn on exceptions the program terminates...
      // exceptions(std::ios_base::badbit);
    }

    /// Copy Ctor
    TCPIOStream(const TCPIOStream& iostr)
      : std::ios(),
	std::iostream(&sb),
	sb(iostr.sb)
    { }
  };

} // namespace racer
} // namespace dlvhex



#endif /* _TCPSTREAM_H */
