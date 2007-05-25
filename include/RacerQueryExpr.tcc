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
 * @file   RacerQueryExpr.tcc
 * @author Thomas Krennwallner
 * @date   Wed Apr 19 11:31:28 2006
 * 
 * @brief  Template implementation for operator<< on a std::vector
 * filled with boost::shared_ptr<T> and boost::ptr_vector<T>.
 * 
 * 
 */

#ifndef _RACERQUERYEXPR_TCC
#define _RACERQUERYEXPR_TCC

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/iterator/indirect_iterator.hpp>

#include <iosfwd>
#include <iterator>
#include <vector>


namespace dlvhex {
namespace dl {
namespace racer {


  /** 
   * Calls operator<< on every element of @a v, separated by a blank.
   * 
   * @todo feature: maybe some homebrewn manipulators for setting the
   * delimiter character?
   *
   * @param s output content of v to this stream
   * @param v a vector of boost::shared_ptr<T>
   * 
   * @return @a s
   */
  template<typename T>
  std::ostream&
  operator<< (std::ostream& s,
	      const std::vector<boost::shared_ptr<T> >& v)
  {
    if (!v.empty())
      {
	std::copy(boost::make_indirect_iterator(v.begin()), 
		  boost::make_indirect_iterator(v.end() - 1),
		  std::ostream_iterator<T>(s, " ")
		  );

	s << *v.back();
      }
    
    return s;
  }

  /** 
   * Calls operator<< on every element of @a v, separated by a blank.
   * 
   * @todo feature: maybe some homebrewn manipulators for setting the
   * delimiter character?
   *
   * @param s output content of v to this stream
   * @param v a boost::ptr_vector<T>
   * 
   * @return @a s
   */
  template<typename T>
  std::ostream&
  operator<< (std::ostream& s, const boost::ptr_vector<T>& v)
  {
    if (!v.empty())
      {
	std::copy(v.begin(),
		  v.end() - 1,
		  std::ostream_iterator<T>(s, " ")
		  );

	s << v.back();
      }
    
    return s;
  }


} // namespace racer
} // namespace dl
} // namespace dlvhex

#endif /* _RACERQUERYEXPR_TCC */


// Local Variables:
// mode: C++
// End:
