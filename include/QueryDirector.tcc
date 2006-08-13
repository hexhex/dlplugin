/* -*- C++ -*- */

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
