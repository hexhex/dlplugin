/* -*- C++ -*- */

/**
 * @file   RacerNRQL.cpp
 * @author Thomas Krennwallner
 * @date   Thu Apr 13 12:11:21 2006
 * 
 * @brief  
 * 
 * 
 */

#include "RacerNRQL.h"
#include "RacerQuery.h"


using namespace dlvhex::racer;



RacerNRQLAtom::RacerNRQLAtom(std::ostream& o)
  : RacerNRQLBase(o)
{}

RacerNRQLAtom::~RacerNRQLAtom()
{}

void
RacerNRQLAtom::build(const Query& query)
{
  getStream() << "(";

  getStream() << ")";
//   const Tuple& q = query.getQuery().getQuery();

//   for (Tuple::const_iterator it = q.begin(); it != q.end(); it++)
//     {
//       getStream() << "(";
	    
//       Atom& a = *it;

//       Term& p = a.getPredicate();
//       Tuple& t = a.getArguments();

//       for (Tuple::const_iterator arg = t.begin(); arg != t.end(); arg++)
// 	{
// 	  Term& x = *arg;
	  
// 	  if (x.isVariable())
// 	    {
// 	      getStream() << "$?" << x.getUnquotedString() << " ";
// 	    }
// 	  else
// 	    {
// 	      ///@todo check nsid
// 	      getStream() << "|" << x.getUnquotedString() << "| ";
// 	    }
// 	}
      
//       ///@todo check nsid
//       getStream() << "|" << p.getUnquotedString() << "|";
	  
//       getStream() << ")";
//     }
}

void
RacerNRQLAtom::walk(const Query& query)
{
  build(query);
}

void
RacerNRQLAtom::retrieve(const Query& query)
{
  build(query);
}


RacerNRQLDecorator::RacerNRQLDecorator(RacerNRQLBasePtr c)
  : RacerNRQLBase(c->getStream()), composite(c)
{}

RacerNRQLDecorator::~RacerNRQLDecorator()
{}

void
RacerNRQLDecorator::build(const Query& query)
{
  composite->build(query);
}

void
RacerNRQLDecorator::walk(const Query& query)
{
  composite->walk(query);
}

void
RacerNRQLDecorator::retrieve(const Query& query)
{
  getStream() << "(";
  walk(query);
  getStream() << ")" << std::endl;
}




RacerNRQLHead::RacerNRQLHead(RacerNRQLBasePtr c)
  : RacerNRQLDecorator(c)
{}

RacerNRQLHead::~RacerNRQLHead()
{}

void
RacerNRQLHead::build(const Query& query)
{
  getStream() << " (";

  const Tuple& out = query.getPatternTuple();
  
  for (Tuple::const_iterator it = out.begin(); it != out.end(); it++)
    {
      const Term* t = &*it;
      
      if (t->isVariable())
	{
	  getStream() << "$?" << t->getVariable();
	}
      else
	{
	  getStream() << "|" << t->getUnquotedString() << "|"; ///@todo check nsid
	}

      getStream() << " ";
    }
  
  getStream() << ")";
}

void
RacerNRQLHead::walk(const Query& query)
{
  RacerNRQLDecorator::walk(query);

  build(query);
}



RacerNRQLBody::RacerNRQLBody(RacerNRQLBasePtr c)
  : RacerNRQLDecorator(c)
{}

RacerNRQLBody::~RacerNRQLBody()
{}

void
RacerNRQLBody::build(const Query& query)
{
  ///@todo check if conjunctive or single query

  RacerNRQLBasePtr ats(new RacerNRQLAtom(getStream()));
  RacerNRQLConjunction(ats).build(query);

  //  RacerNRQLAtom(getStream()).build(query);
}

void
RacerNRQLBody::walk(const Query& query)
{
  RacerNRQLDecorator::walk(query);

  build(query);
}


RacerNRQLConjunction::RacerNRQLConjunction(RacerNRQLBasePtr c)
  : RacerNRQLDecorator(c)
{}

RacerNRQLConjunction::~RacerNRQLConjunction()
{}

void
RacerNRQLConjunction::build(const Query& query)
{
  getStream() << "(and ";
  RacerNRQLDecorator::build(query);
  getStream() << ")";
}

void
RacerNRQLConjunction::walk(const Query& query)
{
  RacerNRQLDecorator::walk(query);

  build(query);
}



RacerNRQLUnion::RacerNRQLUnion(RacerNRQLBasePtr c)
  : RacerNRQLDecorator(c)
{}

RacerNRQLUnion::~RacerNRQLUnion()
{}

void
RacerNRQLUnion::build(const Query& query)
{
  getStream() << "(union ";
  RacerNRQLDecorator::build(query);
  getStream() << ")";
}

void
RacerNRQLUnion::walk(const Query& query)
{
  RacerNRQLDecorator::walk(query);

  build(query);
}



RacerNRQLPremise::RacerNRQLPremise(RacerNRQLBasePtr c)
  : RacerNRQLDecorator(c)
{}

RacerNRQLPremise::~RacerNRQLPremise()
{}

void
RacerNRQLPremise::build(const Query& query)
{
  getStream() << "-under-premise (";

  const Interpretation& ints = query.getInterpretation();
      
  for (AtomSet::const_iterator it = ints.begin();
       it != ints.end(); it++)
    {
//       const Atom& a = *it;
//       const Term& pred = a.getArgument(0);
	  
//       if (pred == query.getPlusC()) // plusC
// 	{
// 	  buildPosInstance(query, a);
// 	}
//       else if (pred == query.getMinusC()) // minusC
// 	{
// 	  buildNegInstance(query, a);
// 	}
//       else if (pred == query.getPlusR()) // plusR
// 	{
// 	  buildPosRelated(query, a);
// 	}
//       else if (pred == query.getMinusR()) // minusR
// 	{
// 	  buildNegRelated(query, a);
// 	}
//       else
// 	{
// 	  // just ignore unknown stuff...
// 	}
    }

  getStream() << ")";
}

void
RacerNRQLPremise::walk(const Query& query)
{
  RacerNRQLDecorator::walk(query);

  build(query);
}
