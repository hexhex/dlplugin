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
Query::setIndividuals(const Tuple& indv)
{
  this->indv = indv;
}

const Tuple&
Query::getIndividuals() const
{
  return this->indv;
}

void
Query::setPlusConcept(const GAtomSet& plusConcept)
{
  this->plusConcept = plusConcept;
}

const GAtomSet&
Query::getPlusConcept() const
{
  return this->plusConcept;
}

void
Query::setMinusConcept(const GAtomSet& minusConcept)
{
  this->minusConcept = minusConcept;
}

const GAtomSet&
Query::getMinusConcept() const
{
  return this->minusConcept;
}

void
Query::setPlusRole(const GAtomSet& plusRole)
{
  this->plusRole = plusRole;
}

const GAtomSet&
Query::getPlusRole() const
{
  return this->plusRole;
}

bool
Query::isSubset(const Query& q2) const
{
  //
  // isSubset() does not lexicographically compare q1 to q2 so this is
  // not an appropriate operator for std::less<Query>
  //
  const Query& q1 = *this;

  const GAtomSet& pc1 = q1.getPlusConcept();
  const GAtomSet& pc2 = q2.getPlusConcept();
  
  const GAtomSet& mc1 = q1.getMinusConcept();
  const GAtomSet& mc2 = q2.getMinusConcept();

  const GAtomSet& pr1 = q1.getPlusRole();
  const GAtomSet& pr2 = q2.getPlusRole();

  //
  // q1 is a proper subset of q2:
  // query atoms, nspaces and ontologies are equal
  // and
  // class/role membership axioms and negated class
  // membership axioms of q1 is contained in the
  // axioms of q2 but not equal.
  //
  if (q1.getQuery() == q2.getQuery()
      &&
      q1.getNamespace() == q2.getNamespace()
      &&
      q1.getOntology() == q2.getOntology()
      &&
      !(pc1.empty() && pc2.empty()
	&& 
	mc1.empty() && mc2.empty()
	&& 
	pr1.empty() && pr2.empty()
	)
      &&
      std::includes(pc2.begin(), pc2.end(),
		    pc1.begin(), pc1.end())
      &&
      (pc1 != pc2 || (pc1.empty() && pc2.empty()))
      &&
      std::includes(mc2.begin(), mc2.end(),
		    mc1.begin(), mc1.end())
      &&
      (mc1 != mc2 || (mc1.empty() && mc2.empty()))
      &&
      std::includes(pr2.begin(), pr2.end(),
		    pr1.begin(), pr1.end())
      &&
      (pr1 != pr2 || (pr1.empty() && pr2.empty()))
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
  : isIncoherent(false),
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
Answer::setTuples(const std::vector<Tuple>& tuples)
{
  this->tuples = tuples;
}

void
Answer::addTuple(const Tuple& tuple)
{
  this->tuples.push_back(tuple);
}

const std::vector<Tuple>&
Answer::getTuples() const
{
  return this->tuples;
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

