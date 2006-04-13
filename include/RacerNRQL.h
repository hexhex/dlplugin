/* -*- C++ -*- */

/**
 * @file   RacerNRQL.h
 * @author Thomas Krennwallner
 * @date   Thu Apr 13 11:24:32 2006
 * 
 * @brief  
 * 
 * 
 */

#ifndef _RACERNRQL_H
#define _RACERNRQL_H

#include "RacerQuery.h"

#include <boost/shared_ptr.hpp>

#include <iostream>

namespace dlvhex {
namespace racer {

  class RacerNRQLBase
  {
  private:
    std::ostream& stream;

  protected:
    explicit
    RacerNRQLBase(std::ostream& o) : stream(o) {}

  public:
    virtual
    ~RacerNRQLBase() {}

    virtual void
    build(const Query&) = 0;

    virtual void
    walk(const Query&) = 0;

    virtual void
    retrieve(const Query&) = 0;

    std::ostream&
    getStream() const { return stream; }
  };




  typedef boost::shared_ptr<RacerNRQLBase> RacerNRQLBasePtr;



  class RacerNRQLDecorator : public RacerNRQLBase
  {
  private:
    RacerNRQLBasePtr composite;

  public:
    explicit
    RacerNRQLDecorator(RacerNRQLBasePtr c);

    virtual
    ~RacerNRQLDecorator();

    virtual void
    build(const Query& query);

    virtual void
    walk(const Query& query);

    virtual void
    retrieve(const Query& query);
  };


  class RacerNRQLAtom : public RacerNRQLBase
  {
  public:
    explicit
    RacerNRQLAtom(std::ostream& o);

    virtual
    ~RacerNRQLAtom();

    virtual void
    build(const Query& query);

    virtual void
    walk(const Query& query);

    virtual void
    retrieve(const Query& query);
  };


  class RacerNRQLRetrieve : public RacerNRQLBase
  {
  public:
    explicit
    RacerNRQLRetrieve(std::ostream& o) : RacerNRQLBase(o) {}

    virtual
    ~RacerNRQLRetrieve() {}

    virtual void
    build(const Query& query)
    {
      getStream() << "retrieve";
    }

    virtual void
    walk(const Query& query)
    {
      build(query);
    }

    virtual void
    retrieve(const Query& query)
    { }
  };



  class RacerNRQLTBoxRetrieve : public RacerNRQLBase
  {
  public:
    explicit
    RacerNRQLTBoxRetrieve(std::ostream& o) : RacerNRQLBase(o) {}

    virtual
    ~RacerNRQLTBoxRetrieve() {}

    virtual void
    build(const Query& query)
    {
      getStream() << "tbox-retrieve";
    }

    virtual void
    walk(const Query& query)
    {
      build(query);
    }

    virtual void
    retrieve(const Query& query)
    { }
  };


  class RacerNRQLHead : public RacerNRQLDecorator
  {
  public:
    explicit
    RacerNRQLHead(RacerNRQLBasePtr c);

    virtual
    ~RacerNRQLHead();

    virtual void
    build(const Query& query);

    virtual void
    walk(const Query& query);
  };



  class RacerNRQLConjunction : public RacerNRQLDecorator
  {
  public:
    explicit
    RacerNRQLConjunction(RacerNRQLBasePtr c);
    
    virtual
    ~RacerNRQLConjunction();

    virtual void
    build(const Query& query);

    virtual void
    walk(const Query& query);
  };


  class RacerNRQLUnion : public RacerNRQLDecorator
  {
  public:
    explicit
    RacerNRQLUnion(RacerNRQLBasePtr c);
    
    virtual
    ~RacerNRQLUnion();

    virtual void
    build(const Query& query);

    virtual void
    walk(const Query& query);
  };



  class RacerNRQLBody : public RacerNRQLDecorator
  {
  public:
    explicit
    RacerNRQLBody(RacerNRQLBasePtr c);
    
    virtual
    ~RacerNRQLBody();

    virtual void
    build(const Query& query);

    virtual void
    walk(const Query& query);
  };
 


  class RacerNRQLPremise : public RacerNRQLDecorator
  {
  public:
    explicit
    RacerNRQLPremise(RacerNRQLBasePtr c);

    virtual
    ~RacerNRQLPremise();

    virtual void
    build(const Query& query);

    virtual void
    walk(const Query& query);
  };


} // namespace racer
} // namespace dlvhex

#endif /* _RACERNRQL_H */
