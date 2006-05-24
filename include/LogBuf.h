/* -*- C++ -*- */

/**
 * @file   LogBuf.h
 * @author Thomas Krennwallner
 * @date   Mon May 22 13:00:46 2006
 * 
 * @brief  
 * 
 * 
 */


#ifndef _LOGBUF_H
#define _LOGBUF_H

#include <string>
#include <streambuf>
#include <sstream>
#include <iostream>

namespace dlvhex {
namespace racer {

  /**
   * @brief 
   */
  class LogBuf : public std::streambuf
  {
  private:
    /// nested output stream
    std::streambuf* obuf;
    /// a stringbuffer for log information, dumped to obuf at sync()
    /// execution
    std::stringbuf sbuf;

  protected:
    /**
     * @return sbuf.sputn(s,n) if obuf != 0. Otherwise returns -1.
     */
    virtual std::streamsize
    xsputn(const std::streambuf::char_type* s, std::streamsize n);

    /**
     * Called when output buffer is full.
     *
     * @param c put this character into the output buffer
     *
     * @return sbuf.sputc(c) if obuf != 0, otherwise traits::eof().
     */
    virtual std::streambuf::int_type
    overflow(std::streambuf::int_type c);

    /**
     * Called when output buffer needs syncronization and resets the
     * input buffer.
     *
     * @return Prefix log information with a timestamp if obuf != 0,
     * otherwise -1.
     */
    virtual std::streambuf::int_type
    sync();

  public:
    explicit
    LogBuf(std::streambuf* sb);

    /// Dtor
    virtual
    ~LogBuf();
  };


} // namespace racer
} // namespace dlvhex

#endif /* _LOGBUF_H */
