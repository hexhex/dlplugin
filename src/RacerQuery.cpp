/* -*- C++ -*- */

/**
 * @file   RacerQuery.cpp
 * @author Thomas Krennwallner
 * @date   Sat Aug 20 17:23:08 2005
 * 
 * @brief  Encapsulates a query and the corresponding answer.
 * 
 * 
 */

#include "OWLParser.h"
#include "RacerQuery.h"
#include "RacerNRQL.h"
#include "RacerQueryExpr.h"
#include "RacerError.h"

#include <dlvhex/PluginInterface.h>
#include <dlvhex/Atom.h>
#include <dlvhex/Term.h>

#include <cassert>


using namespace dlvhex::racer;


namespace dlvhex {
namespace racer {

  std::ostream&
  operator<< (std::ostream& os, const Query& q)
  {
    return os << q.getNamespace()
	      << "DL["
	      << q.getOntology()
	      << ','
	      << q.getPlusC()
	      << ','
	      << q.getMinusC()
	      << ','
	      << q.getPlusR()
	      << ','
	      << q.getMinusR()
	      << ','
	      << q.getQuery()
	      << "]("
	      << q.getPatternTuple()
	      << ')';
  }
  
  
  bool
  operator< (const Query& q1, const Query& q2)
  {
    // check each component of a query if its string representation is
    // less than the components of the other query
    bool lessthan =
      q1.getNamespace()          < q2.getNamespace()          ? true :
      q1.getOntology()           < q2.getOntology()           ? true :
      q1.getPlusC().getString()  < q2.getPlusC().getString()  ? true :
      q1.getMinusC().getString() < q2.getMinusC().getString() ? true :
      q1.getPlusR().getString()  < q2.getPlusR().getString()  ? true :
      q1.getMinusR().getString() < q2.getMinusR().getString() ? true :
      q1.getQuery().getString()  < q2.getQuery().getString()  ? true : false;


    // if q1 >= q2 we have to look at the query types in order to
    // compute the < relation on them
    if (!lessthan)
      {
	// check equality on q1 and q2
	bool eq = q1 == q2;

	const Tuple& p1 = q1.getPatternTuple();
	const Tuple& p2 = q2.getPatternTuple();
	
	// if query types are equal we have to distinguish between the
	// actual pattern tuples in a component-wise fashion
	if (eq &&
	    q1.getTypeFlags() == q2.getTypeFlags() &&
	    p1.size() == p2.size())
	  {
	    unsigned long type = q1.getTypeFlags();
	    
	    Tuple::const_iterator it1 = p1.begin();
	    Tuple::const_iterator it2 = p2.begin();
	    unsigned long mask = 0x1;

	    for (;
		 it1 != p1.end() && it2 != p2.end();
		 it1++, it2++, mask <<= 1
		 )
	      {
		if ((type & mask) == mask)
		  {
		    if (*it1 < *it2)
		      {
			lessthan = true;
			break;
		      }
		  }
	      }
	  }
	else if (eq && p1.size() < p2.size())
	  {
	    // equal but arity differs
	    lessthan = true;
	  }
	else if (eq && p1.size() == p2.size() &&
		 q1.getTypeFlags() < q2.getTypeFlags())
	  {
	    // equal but types differ
	    lessthan = true;
	  }
	else
	  {
	    // nothing to do, either q1 > q2, or q1 == q1 and
	    // typeFlags or sizes are >=
	  }
      }
    
    return lessthan;
  }
  
  
  bool
  operator== (const Query& q1, const Query& q2)
  {
    return
      q1.getNamespace() == q2.getNamespace()
      && q1.getOntology() == q2.getOntology()
      && q1.getPlusC() == q2.getPlusC()
      && q1.getMinusC() == q2.getMinusC()
      && q1.getPlusR() == q2.getPlusR()
      && q1.getMinusR() == q2.getMinusR()
      && q1.getQuery() == q2.getQuery();
  }
  
  
  bool
  operator!= (const Query& q1, const Query& q2)
  {
    return !(q1 == q2);
  }

} // namespace racer
} // namespace dlvhex


Query::Query()
  : typeFlags(0)
{ }

Query::~Query()
{ }


unsigned long
Query::getTypeFlags() const
{
  return typeFlags;
}


bool
Query::isBoolean() const
{
  // shift a 1 size times to the left - 1 => bitmask selects the valid
  // bits
  unsigned long mask = (1 << pattern.size()) - 1;

  // if any flag in the negation of flags is true, i.e. we have a
  // variable term in pattern, we don't have a purely boolean query
  return (~typeFlags & mask) == 0;
}


bool
Query::isRetrieval() const
{
  // if any flag is true, i.e. we have a ground term in pattern, we
  // don't have a purely retrieval query
  return pattern.empty() ? false : typeFlags == 0;
}


bool
Query::isMixed() const
{
  // if both flags and the negated flags are positive, we have a mixed
  // query
  return pattern.empty() ? false : typeFlags > 0 && ~typeFlags > 0;
}


void
Query::setNamespace(const std::string& nspace)
{
  this->nspace = nspace;
}

const std::string&
Query::getNamespace() const
{
  return this->nspace;
}

void
Query::setOntology(const std::string& ontology)
{
  this->ontology = ontology;
}

const std::string&
Query::getOntology() const
{
  return this->ontology;
}

void
Query::setQuery(const Term& query)
{
  this->query = query;
}

const Term&
Query::getQuery() const
{
  return this->query;
}

void
Query::setPatternTuple(const Tuple& pattern)
{
  assert(pattern.size() < 8 * sizeof(typeFlags));

  this->pattern = pattern;
  this->typeFlags = 0;

  unsigned long mask = 0x1;
  for (Tuple::const_iterator it = pattern.begin();
       it != pattern.end();
       it++, mask <<= 1)
    {
      // for every ground term we set a flag in typeFlags
      if (!it->isVariable())
	{
	  typeFlags |= mask;
	}
    }
}

const Tuple&
Query::getPatternTuple() const
{
  return this->pattern;
}

void
Query::setInterpretation(const Interpretation& ints)
{
  this->interpretation = ints;
}

const Interpretation&
Query::getInterpretation() const
{
  return this->interpretation;
}

void
Query::setPlusC(const Term& plusC)
{
  this->plusC = plusC;
}

const Term&
Query::getPlusC() const
{
  return this->plusC;
}

void
Query::setMinusC(const Term& minusC)
{
  this->minusC = minusC;
}

const Term&
Query::getMinusC() const
{
  return this->minusC;
}

void
Query::setPlusR(const Term& plusR)
{
  this->plusR = plusR;
}

const Term&
Query::getPlusR() const
{
  return this->plusR;
}

void
Query::setMinusR(const Term& minusR)
{
  this->minusR = minusR;
}

const Term&
Query::getMinusR() const
{
  return this->minusR;
}


bool
Query::isSubseteq(const Query& q2) const
{
  //
  // isSubseteq() does not lexicographically compare q1 to q2 so this is
  // not an appropriate operator for std::less<Query>
  //
  const Query& q1 = *this;

  const AtomSet& i1 = q1.getInterpretation();
  const AtomSet& i2 = q2.getInterpretation();

  //
  // q1 is a subset of q2:
  //
  // interpretation of q1 is contained in the interpretation of q2.
  //
  return std::includes(i2.begin(), i2.end(), i1.begin(), i1.end());
}

bool
Query::isSuperseteq(const Query& q2) const
{
  //
  // q1 \supset q2 <=> q2 \subset q1
  //
  const Query& q1 = *this;
  return q2.isSubseteq(q1);
}




const std::vector<NRQLBody::shared_pointer>*
Query::createBody() const
{
  return new std::vector<NRQLBody::shared_pointer>;
}


const std::vector<ABoxQueryObject::shared_pointer>*
Query::createHead() const
{
  std::vector<ABoxQueryObject::shared_pointer>* v =
    new std::vector<ABoxQueryObject::shared_pointer>;

  for (Tuple::const_iterator it = getPatternTuple().begin();
       it != getPatternTuple().end();
       ++it)
    {
      ABoxQueryObject::shared_pointer sp;

      if (it->isVariable())
	{
	  sp.reset(new ABoxQueryVariable(it->getVariable()));
	}
      else
	{
	  sp.reset(new ABoxQueryIndividual(it->getUnquotedString()));
	}

      v->push_back(sp);
    }

  return v;
}

const std::vector<ABoxAssertion::shared_pointer>*
Query::createPremise() const
{
  std::vector<ABoxAssertion::shared_pointer>* v = new std::vector<ABoxAssertion::shared_pointer>;

  for (AtomSet::const_iterator it = getInterpretation().begin();
       it != getInterpretation().end(); it++)
    {
      const Atom& a = *it;
      const Term pred = a.getArgument(0);

      if (pred == getPlusC()) // plusC
	{
	  ABoxInstance::shared_pointer sp
	    (new ABoxInstance
	     (new ABoxQueryConcept
	      (a.getArgument(1).getUnquotedString(), getNamespace()),
	      new ABoxQueryIndividual
	      (a.getArgument(2).getUnquotedString(), getNamespace())
	      )
	     );

	  v->push_back(sp);
	}
      else if (pred == getMinusC()) // minusC
	{
	  ABoxInstance::shared_pointer sp
	    (new ABoxInstance
	     (new ABoxNegatedConcept
	      (new ABoxQueryConcept
	       (a.getArgument(1).getUnquotedString(), getNamespace())),
	      new ABoxQueryIndividual
	      (a.getArgument(2).getUnquotedString(), getNamespace())
	      )
	     );

	  v->push_back(sp);
	}
      else if (pred == getPlusR()) // plusR
	{
	  ABoxRelated::shared_pointer sp
	    (new ABoxRelated
	     (new ABoxQueryRole
	      (a.getArgument(1).getUnquotedString(), getNamespace()),
	      new ABoxQueryIndividual
	      (a.getArgument(2).getUnquotedString(), getNamespace()),
	      new ABoxQueryIndividual
	      (a.getArgument(3).getUnquotedString(), getNamespace())
	      )
	     );
	  v->push_back(sp);
	}
      else if (pred == getMinusR()) // minusR
	{
	  // dunno
	}
      else
	{
	  // just ignore unknown stuff...
	}
    }

#if 1
  if (v->empty())
    {
      // this is a preliminary workaround for the abox-cloning bug
      ABoxInstance::shared_pointer foo(new ABoxInstance
				       (new ABoxQueryConcept("foo"),
					new ABoxQueryIndividual("bar")
					)
				       );
      v->push_back(foo);
    }
#endif

  return v;
}





Answer::Answer()
  : PluginAtom::Answer(),
    isIncoherent(false),
    answer(false),
    query(0) // don't care
{ }

Answer::Answer(const Query* q)
  : PluginAtom::Answer(),
    isIncoherent(false),
    answer(false),
    query(q)
{ }

Answer::~Answer()
{ }

void
Answer::setQuery(const Query* q)
{
  this->query = q;
}

void
Answer::setIncoherent(bool isIncoherent)
{
  this->isIncoherent = isIncoherent;
}

bool
Answer::getIncoherent() const
{
  return this->isIncoherent;
}

void
Answer::setErrorMessage(const std::string& errorMsg)
{
  this->errorMsg = errorMsg;
}

const std::string&
Answer::getErrorMessage() const
{
  return this->errorMsg;
}

void
Answer::setAnswer(bool answer)
{
  this->answer = answer;
}

bool
Answer::getAnswer() const
{
  return this->answer;
}

void
Answer::addTuple(const Tuple& out)
{
  if (query == 0)
    {
      PluginAtom::Answer::addTuple(out);
    }
  else
    {
      unsigned long type = query->getTypeFlags() & std::numeric_limits<unsigned long>::max();
      
      ///@todo we need to take care of arbitrary ground terms in the answer

      if (type == 0x2)
	{
	  Tuple tmp(out);

	  std::string p = query->getPatternTuple()[1].getUnquotedString();

	  if (p.find('#') == std::string::npos)
	    {
	      tmp.push_back(Term(query->getNamespace() + p, true));
	    }
	  else
	    {
	      tmp.push_back(Term(p, true));
	    }

	  PluginAtom::Answer::addTuple(tmp);
	}
      else if (type == 0x1)
	{
	  Tuple tmp;

	  std::string p = query->getPatternTuple()[0].getUnquotedString();

	  if (p.find('#') == std::string::npos)
	    {
	      tmp.push_back(Term(query->getNamespace() + p, true));
	    }
	  else
	    {
	      tmp.push_back(Term(p, true));
	    }

	  tmp.insert(tmp.end(), out.begin(), out.end());

	  PluginAtom::Answer::addTuple(tmp);
	}
      else
	{
	  PluginAtom::Answer::addTuple(out);
	}
    }
}


QueryCtx::QueryCtx()
  : q(new Query), a(new Answer(q))
{ }

QueryCtx::QueryCtx(const PluginAtom::Query& query)
  : q(new Query), a(new Answer(q))
{
  // inputtuple[0] contains the KB URI constant
  std::string ontostr = query.getInputTuple()[0].getUnquotedString();
  q->setOntology(ontostr);

  // set query if input tuple contains a query atom
  if (query.getInputTuple().size() > 5)
    {
      q->setQuery(query.getInputTuple()[5]);
    }

  // get namespace from owl document
  OWLParser p(ontostr);
  std::string defaultNS;

  try
    {
      p.parseNamespace(defaultNS);
    }
  catch (RacerParsingError&)
    {
      // just ignore it
    }

  q->setNamespace(defaultNS);
  q->setInterpretation(query.getInterpretation());
  q->setPatternTuple(query.getPatternTuple());
  q->setPlusC(query.getInputTuple()[1]);
  q->setMinusC(query.getInputTuple()[2]);
  q->setPlusR(query.getInputTuple()[3]);
  q->setMinusR(query.getInputTuple()[4]);
}

QueryCtx::QueryCtx(const QueryCtx& qctx)
{
  setQuery(qctx.q);
  setAnswer(qctx.a);
}

QueryCtx::~QueryCtx()
{
  delete q;
  delete a;
}

void
QueryCtx::setQuery(Query* q)
{
  if (q && this->q != q)
    {
      if (this->q)
	{
	  delete this->q;
	}
      this->q = q;
      this->a->setQuery(this->q);
    }
}

Query&
QueryCtx::getQuery() const
{
  assert(this->q != 0);
  return *this->q;
}

void
QueryCtx::setAnswer(Answer* a)
{
  if (a && this->a != a)
    {
      if (this->a)
	{
	  delete this->a;
	}
      this->a = a;
      this->a->setQuery(this->q);
    }
}

Answer&
QueryCtx::getAnswer() const
{
  assert(this->a != 0);
  return *this->a;
}

