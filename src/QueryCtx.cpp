/* dlvhex-dlplugin -- Integration of Answer-Set Programming and Description Logics.
 *
 * Copyright (C) 2005, 2006, 2007  Thomas Krennwallner
 * 
 * This file is part of dlvhex-dlplugin.
 *
 * dlvhex-dlplugin is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * dlvhex-dlplugin is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dlvhex-dlplugin; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


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

#include <sstream>

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


QueryCtx::QueryCtx(const ComfortPluginAtom::ComfortQuery& query, KBManager& kb) throw (DLError)
  : q(0), a(0)
{
  const ComfortTuple& inputtuple = query.input;

  if (inputtuple.size() < 4)
    {
      throw DLError("Incompatible input list.");
    }

  // inputtuple[0] contains the KB URI constant
  std::string ontostr = inputtuple[0].strval;

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

  DLQuery::shared_pointer dlq;
  const ComfortTuple& outputlist = query.pattern;
  std::string qstr;

  ///@todo exchange this whole crap by a proper boost spirit parser
  ///for UCQs, CQs, and plain queries.

  // setup the query if input tuple contains a query atom or a
  // (union of) conjunctive query
  if (inputtuple.size() > 5)
    {
      qstr = inputtuple[5].strval;

      if (qstr.length() >= 2) ///@todo kludge: check for turtle syntax (this even breaks for negated uris)
	{
	  int end = qstr.length() - 1;

	  char b = qstr[0];
	  char e = qstr[end];

	  if (b == '<' && e == '>') ///@todo kludge: remove turtle brackets
	    {
	      qstr.erase(end, 1);
	      qstr.erase(0, 1);
	    }
	}

      if (qstr.find(" v ") != std::string::npos) // parse union of conjunctive queries
	{
	  std::vector<ComfortInterpretation> as;

	  // separate union of atomlists
	  UnionAtomSeparator(qstr, as).parse();

	  dlq = DLQuery::shared_pointer(new DLQuery(onto, as, outputlist));
	}
      else if (qstr.find('(') != std::string::npos) // parse conjunctive query
	{
	  ComfortInterpretation as;

	  // separate atomlist
	  AtomSeparator(qstr, as).parse();

	  dlq = DLQuery::shared_pointer(new DLQuery(onto, as, outputlist));
	}
      else // this is a plain query
	{
	  qstr = inputtuple[5].strval();

	  // no namespace in query
	  if (!URI::isValid(qstr))
	    {
	      if (qstr[0] == '-') // negated query
		{
		  qstr.insert(1, onto->getNamespace());
		}
	      else
		{
		  qstr.insert(0, onto->getNamespace());
		}
	    }

	  // kludge: here, we make a case distinction between plain
	  // concept and plain role queries in order to create a CQ
	  // for the latter. Rationale: Racer is unable to process
	  // negated role queries using the oldschool query language,
	  // i.e., (retrieve-related-individuals (not R)) is
	  // broken. But, nRQL queries are able to process (not R).

	  if (outputlist.size() == 2) // (negated) role query, create a CQ for -R, a old-school query for R
	    {
              #if 0
	      // create single atom conjunctive query for R
	      std::ostringstream oss;
	      oss << qstr << '(' << outputlist[0] << ',' << outputlist[1] << ')';
		  
	      AtomSet as;
		  
	      // separate atomlist
	      AtomSeparator(oss.str(), as).parse();
		  
	      dlq = DLQuery::shared_pointer(new DLQuery(onto, as, outputlist));
              #endif // 0


	      if (qstr[0] == '-') // keep the strong negation in front and use a CQ, see above
		{
		    // create single atom conjunctive query for -R
		    std::ostringstream oss;
		    oss << qstr << '(' << outputlist[0] << ',' << outputlist[1] << ')';
		    
		    ComfortInterpretation as;
		    
		    // separate atomlist
		    AtomSeparator(oss.str(), as).parse();
		    
		    dlq = DLQuery::shared_pointer(new DLQuery(onto, as, outputlist));
		}
	      else
	        {
		  ComfortTerm qu = ComfortTerm::createConstant("\"" + qstr + "\"");
		  // create a plain query (oldschool)
		  dlq = DLQuery::shared_pointer(new DLQuery(onto, qu, query.pattern));
		}
	    }
	  else // (negated) concept query
	    {
	      ComfortTerm qu = ComfortTerm::createConstant("");

	      if (qstr[0] == '-') // keep the strong negation in front!
		{
		  qu = ComfortTerm::createConstant("-\"" + qstr.substr(1) + "\"");
		}
	      else
		{
		  qu = ComfortTerm::createConstant("\"" + qstr + "\"");
		}

	      // create a plain query (oldschool)
	      dlq = DLQuery::shared_pointer(new DLQuery(onto, qu, query.pattern));
	    }
	}
    }
  else // no query term, what now?
    {
      // use empty query
      //assert("No query term." == 0);
      dlq = DLQuery::shared_pointer(new DLQuery(onto, ComfortTerm::createConstant(""), query.pattern));
    }

  this->q = new Query(kb, dlq,
		      inputtuple[1],
		      inputtuple[2],
		      inputtuple[3],
		      inputtuple[4],
		      query.interpretation()
		      );

  this->a = new Answer(this->q);
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


// Local Variables:
// mode: C++
// End:
