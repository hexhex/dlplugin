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

#include "RacerQuery.h"

#include <dlvhex/PluginInterface.h>
#include <dlvhex/Atom.h>
#include <dlvhex/Term.h>

#include <assert.h>

using namespace dlvhex::racer;


Query::Query()
{ }

Query::~Query()
{ }

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
  this->pattern = pattern;
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
Query::isSubset(const Query& q2) const
{
  //
  // isSubset() does not lexicographically compare q1 to q2 so this is
  // not an appropriate operator for std::less<Query>
  //
  const Query& q1 = *this;

  const GAtomSet& i1 = q1.getInterpretation().getAtomSet();
  const GAtomSet& i2 = q2.getInterpretation().getAtomSet();

  //
  // q1 is a proper subset of q2:
  // query atoms, nspaces and ontologies are equal
  // and interpretation of q1 is contained in the
  // interpretation of q2 but not equal.
  //
  if (q1.getQuery() == q2.getQuery()
      &&
      q1.getNamespace() == q2.getNamespace()
      &&
      q1.getOntology() == q2.getOntology()
//       &&
//       q1.getPatternTuple() == q2.getPatternTuplle()
      &&
      !(i1.empty() && i2.empty())
      &&
      std::includes(i2.begin(), i2.end(),
		    i1.begin(), i1.end())
      &&
      (i1 != i2 || (i1.empty() && i2.empty()))
      &&
      q1.getPlusC() == q2.getPlusC()
      &&
      q1.getMinusC() == q2.getMinusC()
      &&
      q1.getPlusR() == q2.getPlusR()
      &&
      q1.getMinusR() == q2.getMinusR()
      )
    {
      return true;
    }

  return false;
}

bool
Query::isSuperset(const Query& q2) const
{
  //
  // q1 \supset q2 <=> q2 \subset q1
  //
  const Query& q1 = *this;
  return q2.isSubset(q1);
}




Answer::Answer()
  : PluginAtom::Answer(),
    isIncoherent(false),
    answer(false)
{ }

Answer::~Answer()
{ }

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



QueryCtx::QueryCtx()
  : q(new Query), a(new Answer)
{ }

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
    }
}

Answer&
QueryCtx::getAnswer() const
{
  assert(this->a != 0);
  return *this->a;
}

