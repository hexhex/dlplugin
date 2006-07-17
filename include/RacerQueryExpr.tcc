/* -*- C++ -*- */

/**
 * @file   RacerQueryExpr.tcc
 * @author Thomas Krennwallner
 * @date   Wed Apr 19 11:31:28 2006
 * 
 * @brief  Template implementation for operator<< on a std::vector
 * filled with boost::shared_ptr.
 * 
 * 
 */

#ifndef _RACERQUERYEXPR_TCC
#define _RACERQUERYEXPR_TCC

#include <boost/shared_ptr.hpp>

#include <iosfwd>
#include <iterator>
#include <vector>


namespace dlvhex {
namespace racer {


  /**
   * Dereferences a boost::shared_ptr.
   */
  struct Dereference
  {
    /** 
     * Returns a reference to the object contained in ptr.
     *
     * @param ptr
     * 
     * @return ptr.operator*()
     */
    template<typename T>
    T&
    operator() (const boost::shared_ptr<T>& ptr) const
    {
      return *ptr;
    }
  };


  /** 
   * Calls operator<< on every element of v, separated by a blank.
   * 
   * @todo feature: maybe some homebrewn manipulators for setting the
   * delimiter character?
   *
   * @param s 
   * @param v a vector of boost::shared_ptr<T>
   * 
   * @return s
   */
  template<typename T>
  std::ostream&
  operator<< (std::ostream& s,
	      const std::vector<boost::shared_ptr<T> >& v)
  {
    if (!v.empty())
      {
	std::transform(v.begin(),
		       v.end() - 1,
		       std::ostream_iterator<T>(s, " "),
		       Dereference()
		       );

	s << *v.back();
      }
    
    return s;
  }


} // namespace racer
} // namespace dlvhex

#endif /* _RACERQUERYEXPR_TCC */
