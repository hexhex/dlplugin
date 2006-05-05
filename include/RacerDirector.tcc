/* -*- C++ -*- */

/**
 * @file   RacerDirector.tcc
 * @author Thomas Krennwallner
 * @date   Fri Jul 15 13:35:52 2005
 * 
 * @brief  Template implementation for RacerDirector class.
 * 
 * 
 */

#ifndef _RACERDIRECTOR_TCC
#define _RACERDIRECTOR_TCC

namespace dlvhex {
namespace racer {

  template <class Builder, class Parser>
  RacerDirector<Builder, Parser>::RacerDirector(std::iostream& s)
    : RacerBaseDirector(),
      builder(s),
      parser(s)
  { }

  template <class Builder, class Parser>
  QueryCtx::shared_pointer
  RacerDirector<Builder, Parser>::query(QueryCtx::shared_pointer qctx)
    throw(RacerError)
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

} // namespace racer
} // namespace dlvhex


#endif /* _RACERDIRECTOR_TCC */
