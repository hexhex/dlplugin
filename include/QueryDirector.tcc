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
 * @file   QueryDirector.tcc
 * @author Thomas Krennwallner
 * @date   Fri Jul 15 13:35:52 2005
 * 
 * @brief  Template implementation for QueryDirector class.
 * 
 * 
 */

#ifndef _QUERYDIRECTOR_TCC
#define _QUERYDIRECTOR_TCC

namespace dlvhex {
namespace dl {

  template <class Builder, class Parser>
  QueryDirector<Builder, Parser>::QueryDirector(std::iostream& s)
    : QueryBaseDirector(),
      builder(s),
      parser(s)
  { }

  template <class Builder, class Parser>
  QueryCtx::shared_pointer
  QueryDirector<Builder, Parser>::query(QueryCtx::shared_pointer qctx)
    throw(DLError)
  {
    // just in case the builder has nothing to send, we cannot simply
    // parse an answer since there is none. So forget about this
    // command if builder returns false.
    if (builder.buildCommand(qctx->getQuery()))
      {
	parser.parse(qctx->getAnswer());
      }
    return qctx;
  }

} // namespace dl
} // namespace dlvhex


#endif /* _QUERYDIRECTOR_TCC */


// Local Variables:
// mode: C++
// End:
