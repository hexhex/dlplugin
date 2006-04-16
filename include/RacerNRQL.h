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

#include "RacerQueryExpr.h"

#include <boost/shared_ptr.hpp>

#include <iostream>
#include <vector>

namespace dlvhex {
namespace racer {

  class NRQLBase
  {
  protected:
    virtual std::ostream&
    output(std::ostream&) const = 0;

  public:
    virtual
    ~NRQLBase() {}

    friend std::ostream&
    operator<< (std::ostream&, const NRQLBase&);
  };

  std::ostream&
  operator<< (std::ostream& s, const NRQLBase& b);


  class NRQLBody : public NRQLBase
  {
  protected:
    NRQLBody() {}

  public:
    typedef NRQLBody value_type;
    typedef const value_type* const_pointer;
    typedef boost::shared_ptr<const value_type> shared_pointer;
  };



  class NRQLQueryAtom : public NRQLBody
  {
  private:
    const ABoxQueryAtom::shared_pointer atom;

    std::ostream&
    output(std::ostream& s) const;

  public:
    explicit
    NRQLQueryAtom(ABoxQueryAtom::const_pointer a);
  };



  class NRQLConjunction : public NRQLBody
  {
  private:
    std::vector<NRQLBody::shared_pointer> list;

    std::ostream&
    output(std::ostream& s) const;

  public:
    void
    addAtom(NRQLBody::const_pointer e);
  };



  class NRQLUnion : public NRQLBody
  {
  private:
    std::vector<NRQLBody::shared_pointer> list;

    std::ostream&
    output(std::ostream& s) const;

  public:
    virtual void
    addAtom(NRQLBody::const_pointer e);
  };




  class NRQLRetrieve : public NRQLBase
  {
  private:
    std::vector<ABoxQueryExpr::shared_pointer> head;
    std::vector<NRQLBody::shared_pointer> body;

    std::ostream&
    output(std::ostream& s) const;

  public:
    void
    addHead(ABoxQueryExpr::const_pointer e);

    void
    addBody(NRQLBody::const_pointer e);
  };



  class NRQLTBoxRetrieve : public NRQLBase
  {
  private:
    std::vector<ABoxQueryExpr::shared_pointer> head;
    std::vector<NRQLBody::shared_pointer> body;

    std::ostream&
    output(std::ostream& s) const;

  public:
    void
    addHead(ABoxQueryExpr::const_pointer e);

    void
    addBody(NRQLBody::const_pointer e);
  };



  class NRQLRetrieveUnderPremise : public NRQLBase
  {
  private:
    std::vector<ABoxAssertion::shared_pointer> premise;
    std::vector<ABoxQueryExpr::shared_pointer> head;
    std::vector<NRQLBody::shared_pointer> body;

    std::ostream&
    output(std::ostream& s) const;

  public:
    void
    addPremise(ABoxAssertion::const_pointer e);

    void
    addHead(ABoxQueryExpr::const_pointer e);

    void
    addBody(NRQLBody::const_pointer e);
  };



} // namespace racer
} // namespace dlvhex

#endif /* _RACERNRQL_H */
