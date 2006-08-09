/* -*- C++ -*- */

/**
 * @file   LogBuf.h
 * @author Thomas Krennwallner
 * @date   Mon May 22 13:00:46 2006
 * 
 * @brief  Logging facility.
 * 
 * 
 */


#ifndef _LOGBUF_H
#define _LOGBUF_H

#include <streambuf>
#include <sstream>
#include <iostream>

namespace dlvhex {
namespace racer {

  /**
   * @brief A std::streambuf for logging purposes.
   */
  class LogBuf : public std::streambuf
  {
  private:
    /// nested output stream
    std::ostream* out;
    /// a stringbuffer for log information, dumped to out at sync()
    /// execution
    std::ostringstream sstream;

  protected:
    /**
     * Write @a n bytes of @a s to #sstream.
     *
     * @param s
     * @param n
     *
     * @return #sstream::write(s,n) if #out != 0, otw. -1.
     */
    virtual std::streamsize
    xsputn(const std::streambuf::char_type* s, std::streamsize n);

    /**
     * Called when output buffer is full.
     *
     * @param c put this character into the output buffer
     *
     * @return #sstream::put(c) if #out != 0, otw. #traits_type::eof().
     */
    virtual std::streambuf::int_type
    overflow(std::streambuf::int_type c);

    /**
     * Called when output buffer needs syncronization and resets the
     * input buffer.
     *
     * @return Prefix log information with a timestamp if #out != 0,
     * otw. -1.
     */
    virtual std::streambuf::int_type
    sync();

  public:
    explicit
    LogBuf(std::ostream* stream = 0);

    /// Dtor
    virtual
    ~LogBuf();
  };


  /// a logging output stream
  extern std::ostream log;

} // namespace racer
} // namespace dlvhex

#endif /* _LOGBUF_H */
