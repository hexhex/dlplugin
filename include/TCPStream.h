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

#include <boost/shared_ptr.hpp>

#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Connector.h>

namespace dlvhex {
namespace racer {

  /**
   * @brief provides input/output buffering for the network connection.
   *
   * TCPStreamBuf connects to a TCP server and buffers the input and
   * output character stream. The connection is going to be
   * established in a lazy manner, i.e. only when something is
   * actually sent or received.
   */
  class TCPStreamBuf : public std::streambuf
  {
  private:
    /// hostname
    const std::string host;
    /// tcp port
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
    void
    initBuffers();


  protected:
    /**
     * Called when output buffer is full.
     *
     * @param c put this character into the output buffer
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
     * Tries to establish a connection to the tcp server.
     *
     * @return false on error, true on success.
     */
    virtual bool
    open();

    /**
     * Closes the connection.
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
    boost::shared_ptr<std::streambuf> sb;

  public:
    /// Default Ctor
    TCPIOStream(const std::string& host, unsigned port);

    /// Copy Ctor
    TCPIOStream(const TCPIOStream& iostr);

    void
    setConnection(const std::string& host = "localhost",
		  unsigned port = 8088);
  };

} // namespace racer
} // namespace dlvhex



#endif /* _TCPSTREAM_H */
