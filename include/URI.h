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
 * @file   URI.h
 * @author Thomas Krennwallner
 * @date   Fri Sep  8 09:45:42 2006
 * 
 * @brief  URI convenience class.
 * 
 * 
 */


#ifndef _URI_H
#define _URI_H

#include <string>


namespace dlvhex {

//
// forward declarations
//
class ComfortTerm;

namespace dl {

  /**
   * @brief URI convenience class with some nice helper methods.
   */
  class URI
  {
  private:
    /// the string representation of an URI
    std::string uri;

    std::string
    getAbsolutePath(const std::string&);

    void
    setupURI(const std::string&, bool);

  public:
    URI(const std::string&, bool absolute = false);

    URI(const ComfortTerm&, bool absolute = false);

    /** 
     * @return #uri
     */
    inline const std::string&
    getString() const
    {
      return uri;
    }

    /** 
     * @return true if #uri is a local file.
     */
    inline bool
    isLocal() const
    {
      return uri.find("http://") != 0;
    }

    /** 
     * @return the path component of #uri.
     */
    inline std::string
    getPath() const
    {
      std::string::size_type c = uri.find(":");
      return isLocal() ? uri.substr(c + 1) : uri.substr(c + uri.find("/", c));
    }

    /** 
     * @param u2 
     * 
     * @return #uri.compare()
     */
    inline int
    compare(const URI& u2) const
    {
      return uri.compare(u2.getString());
    }


    /** 
     * @return true if @a s is a valid URI, otw. false
     */
    static bool
    isValid(const std::string& s);

    /** 
     * @return true if @a t is a valid URI term, otw. false
     */
    static bool
    isValid(const ComfortTerm& t);


    /**
     * @return the plain URI as string without fancy turtle syntax and without negation
     */
    static std::string
    getPlainURI(const std::string& s);


    friend std::ostream&
    operator<< (std::ostream& os, const URI& u);

    friend bool
    operator< (const URI& u1, const URI& u2);

    friend bool
    operator== (const URI& u1, const URI& u2);

    friend bool
    operator!= (const URI& u1, const URI& u2);
  };


  inline std::ostream&
  operator<< (std::ostream& os, const URI& u)
  {
    return os << u.getString();
  }

  inline bool
  operator< (const URI& u1, const URI& u2)
  {
    return u1.getString() < u2.getString();
  }

  inline bool
  operator== (const URI& u1, const URI& u2)
  {
    return u1.getString() == u2.getString();
  }

  inline bool
  operator!= (const URI& u1, const URI& u2)
  {
    return !(u1 == u2);
  }

} // namespace dl
} // namespace dlvhex

#endif /* _URI_H */


// Local Variables:
// mode: C++
// End:
