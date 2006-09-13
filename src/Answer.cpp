/* -*- C++ -*- */

/**
 * @file   Answer.cpp
 * @author Thomas Krennwallner
 * @date   Thu Aug  3 12:03:57 2006
 * 
 * @brief  The Answer to a Query.
 * 
 * 
 */


#include "Answer.h"
#include "Query.h"
#include "URI.h"

#include <dlvhex/PluginInterface.h>
#include <dlvhex/Term.h>

#include <string>
#include <iterator>
#include <iosfwd>

using namespace dlvhex::dl;

namespace dlvhex {
  namespace dl {

    std::ostream&
    operator<< (std::ostream& os, const Answer& a)
    {
      if (!a.query)
	{
	  return os;
	}
      
      if (a.query->getDLQuery()->isBoolean())
	{
	  os << a.getAnswer();
	}
      else
	{
	  boost::shared_ptr<std::vector<Tuple> > tuples = a.getTuples();

	  if (!tuples->empty())
	    {
	      for (std::vector<Tuple>::const_iterator it = tuples->begin();
		   it != --tuples->end(); ++it)
		{
		  os.put('(');
		  if (!it->empty())
		    {
		      std::copy(it->begin(), --it->end(),
				std::ostream_iterator<Term>(os, ", "));
		      os << it->back();
		    }
		  os << "), ";
		}

	      os.put('(');
	      if (!tuples->back().empty())
		{
		  std::copy(tuples->back().begin(), --tuples->back().end(),
			    std::ostream_iterator<Term>(os, ", "));
		  os << tuples->back().back();
		}
	      os.put(')');
	    }
	}
      
      return os;
    }

  } // namespace dl
} //namespace dlvhex


Answer::Answer(const Query* q)
  : PluginAtom::Answer(),
    errorMsg(),
    warningMsg(),
    isIncoherent(false),
    answer(false),
    query(q)
{ }


Answer::Answer(const Answer& a)
  : PluginAtom::Answer(a),
    errorMsg(a.errorMsg),
    warningMsg(a.warningMsg),
    isIncoherent(a.isIncoherent),
    answer(a.answer),
    query(a.query)
{ }


Answer&
Answer::operator=(const Answer& a)
{
  if (this != &a)
    {
      errorMsg = a.errorMsg;
      warningMsg = a.warningMsg;
      isIncoherent = a.isIncoherent;
      answer = a.answer;
      query = a.query;
    }

  return *this;
}


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
Answer::setWarningMessage(const std::string& warningMsg)
{
  this->warningMsg = warningMsg;
}

const std::string&
Answer::getWarningMessage() const
{
  return this->warningMsg;
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
  if (query == 0) // just in case we don't have a corresponding query
    {
      PluginAtom::Answer::addTuple(out);
    }
  else if (query->getDLQuery()->isConjQuery()) // in conj. queries we
					       // only check for
					       // anon. variables
    {
      const DLQuery::shared_pointer& dlq = query->getDLQuery();
      const Tuple& pat = dlq->getPatternTuple();

      if (out.size() == pat.size())
	{
	  PluginAtom::Answer::addTuple(out);
	}
      else // take care of anonymous variables
	{
	  Tuple tmp;
	  Tuple::const_iterator pit = pat.begin();
	  Tuple::const_iterator oit = out.begin();

	  //
	  // Iterate output list and look for anonymous variables. If
	  // we find one, append the empty string constant Term to the
	  // output tuple, otherwise add the corresponding Term in
	  // out.
	  //
	  // e.g. pt = (_,X,Y,_,Z), out = (a1,a2,a3)
	  //      -> tmp = ("",a1,a2,"",a3)
	  //
	  for (; pit != pat.end(); ++pit)
	    {
	      if (pit->isAnon())
		{
		  tmp.push_back(Term("", true));
		}
	      else
		{
		  tmp.push_back(*oit);
		  ++oit;
		}
	    }

	  PluginAtom::Answer::addTuple(tmp);
	}
    }
  else // a non-conjunctive query needs special treatment
    {
      const DLQuery::shared_pointer& dlq = query->getDLQuery();
      const Tuple& pat = dlq->getPatternTuple();
      unsigned long type = dlq->getTypeFlags() & std::numeric_limits<unsigned long>::max();
      const std::string& nspace = dlq->getOntology()->getNamespace();

      if (type == 0x2) // left retrieval
	{
	  Tuple tmp(out);

	  std::string p = pat[1].getUnquotedString();

	  if (!URI::isValid(p))
	    {
	      tmp.push_back(Term(nspace + p, true));
	    }
	  else
	    {
	      tmp.push_back(Term(p, true));
	    }

	  PluginAtom::Answer::addTuple(tmp);
	}
      else if (type == 0x1) // right retrieval
	{
	  Tuple tmp;

	  std::string p = pat[0].getUnquotedString();

	  if (!URI::isValid(p))
	    {
	      tmp.push_back(Term(nspace + p, true));
	    }
	  else
	    {
	      tmp.push_back(Term(p, true));
	    }

	  tmp.insert(tmp.end(), out.begin(), out.end());

	  PluginAtom::Answer::addTuple(tmp);
	}
      else if (dlq->isBoolean()) // ground query
	{
	  PluginAtom::Answer::addTuple(pat);
	}
      else // full retrieval query
	{
	  PluginAtom::Answer::addTuple(out);
	}
    }
}
