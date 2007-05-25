/* dlvhex-dlplugin -- Integration of Answer-Set Programming and Description Logics.
 *
 * Copyright (C) 2005, 2006, 2007  Thomas Krennwallner
 * 
 * This file is part of dlvhex-dlplugin.
 *
 * dlvhex-dlplugin is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * dlvhex-dlplugin is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dlvhex-dlplugin; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


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
namespace util {

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


    /// private copy ctor
    LogBuf(const LogBuf&)
      : std::streambuf(), out(0), sstream()
    { /* ignore */ }


    /// private assignment op
    LogBuf&
    operator= (const LogBuf&)
    {
      return *this; // ignore
    }


  protected:
    /**
     * Write @a n bytes of @a s to #sstream.
     *
     * @param s
     * @param n
     *
     * @return write(s,n) of #sstream if #out != 0, otw. -1.
     */
    virtual std::streamsize
    xsputn(const std::streambuf::char_type* s, std::streamsize n);

    /**
     * Called when output buffer is full.
     *
     * @param c put this character into the output buffer
     *
     * @return put(c) of #sstream if #out != 0, otw. traits_type::eof().
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

} // namespace util
} // namespace dlvhex

#endif /* _LOGBUF_H */


// Local Variables:
// mode: C++
// End:
