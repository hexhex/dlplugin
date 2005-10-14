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

namespace dlvex_racer {

  template <class Builder, class Parser>
  RacerDirector<Builder, Parser>::RacerDirector(std::iostream& s, DirectorPtr d)
    : RacerBaseDirector(d),
      builder(s),
      parser(s)
  { }


  template <class Builder, class Parser>
  RacerDirector<Builder, Parser>::~RacerDirector()
  { }


  template <class Builder, class Parser>
  RacerBaseDirector::QueryCtxPtr
  RacerDirector<Builder, Parser>::query(QueryCtxPtr qctx) throw(RacerError)
  {
    if (director.get() != 0)
      {
	qctx = director->query(qctx);
      }
    builder.buildCommand(qctx->getQuery());
    parser.parse(qctx->getAnswer());
    return qctx;
  }


} /* namespace dlvex_racer */


#endif /* _RACERDIRECTOR_TCC */
