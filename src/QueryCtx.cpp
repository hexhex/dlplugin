/* -*- C++ -*- */

/**
 * @file   QueryCtx.cpp
 * @author Thomas Krennwallner
 * @date   Thu Aug  3 12:15:06 2006
 * 
 * @brief  Encapsulates a Query and the corresponding Answer.
 * 
 * 
 */


#include "QueryCtx.h"
#include "Query.h"
#include "Answer.h"
#include "DLError.h"
#include "KBManager.h"
#include "URI.h"
#include "AtomSeparator.h"


using namespace dlvhex::dl;


QueryCtx::QueryCtx(const QueryCtx&)
  : q(0), a(0)
{ }


QueryCtx&
QueryCtx::operator= (const QueryCtx&)
{
  return *this;
}


QueryCtx::QueryCtx(Query* qq, Answer* aa)
  : q(qq), a(aa)
{ }


QueryCtx::QueryCtx(const PluginAtom::Query& query, KBManager& kb) throw (DLError)
  : q(0), a(0)
{
  const Tuple& inputtuple = query.getInputTuple();

  if (inputtuple.size() < 4)
    {
      throw DLError("Incompatible input list.");
    }

  // inputtuple[0] contains the KB URI constant
  std::string ontostr = inputtuple[0].getUnquotedString();

  Ontology::shared_pointer onto;

  // get the ontology and parse the default namespace
  try
    {
      onto = Ontology::createOntology(ontostr);
    }
  catch (DLError& e)
    {
      throw e;
    }

  // setup the query if input tuple contains a query atom or a
  // conjunctive query
  if (inputtuple.size() > 5)
    {
      const std::string& qstr = inputtuple[5].getUnquotedString();

      if (qstr.find('(') != std::string::npos) // parse conjunctive query
	{
	  AtomSet as;

	  // separate atomlist
	  AtomSeparator(qstr, as).parse();

	  DLQuery::shared_pointer dlq(new DLQuery(onto, as, query.getPatternTuple()));

	  q = new Query(kb, dlq,
			inputtuple[1],
			inputtuple[2],
			inputtuple[3],
			inputtuple[4],
			query.getInterpretation()
			);
	}
      else // this is a plain query
	{
	  std::string querystr = inputtuple[5].getUnquotedString();

	  // no namespace in query
	  if (!URI::isValid(querystr))
	    {
	      if (querystr[0] == '-') // negated query
		{
		  querystr.insert(1, onto->getNamespace());
		}
	      else
		{
		  querystr.insert(0, onto->getNamespace());
		}
	    }

	  Term qu(querystr);

	  DLQuery::shared_pointer dlq(new DLQuery(onto, qu, query.getPatternTuple()));

	  q = new Query(kb, dlq,
			inputtuple[1],
			inputtuple[2],
			inputtuple[3],
			inputtuple[4],
			query.getInterpretation()
			);
	}
    }
  else // no query term, what now?
    {
      DLQuery::shared_pointer dlq(new DLQuery(onto, Term(), query.getPatternTuple()));

      q = new Query(kb, dlq,
		    inputtuple[1],
		    inputtuple[2],
		    inputtuple[3],
		    inputtuple[4],
		    query.getInterpretation()
		    );
    }

  a = new Answer(q);
}


QueryCtx::~QueryCtx()
{
  if (q) delete q;
  if (a) delete a;
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

