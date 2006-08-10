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

#include <dlvhex/PluginInterface.h>
#include <dlvhex/Term.h>

#include <string>

using namespace dlvhex::racer;


Answer::Answer(const Query* q)
  : PluginAtom::Answer(),
    isIncoherent(false),
    answer(false),
    query(q)
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
  else if (query->getDLQuery().isConjQuery()) // in conj. queries we
					      // only check for
					      // anon. variables
    {
      if (out.size() == query->getDLQuery().getPatternTuple().size())
	{
	  PluginAtom::Answer::addTuple(out);
	}
      else // take care of anonymous variables
	{
	  Tuple tmp;
	  const Tuple& pt = query->getDLQuery().getPatternTuple();
	  Tuple::const_iterator pit = pt.begin();
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
	  for (; pit != pt.end(); ++pit)
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
      const Tuple& pat = query->getDLQuery().getPatternTuple();
      unsigned long type = query->getDLQuery().getTypeFlags() & std::numeric_limits<unsigned long>::max();
      
      if (type == 0x2) // left retrieval
	{
	  Tuple tmp(out);

	  std::string p = pat[1].getUnquotedString();

	  if (p.find('#') == std::string::npos)
	    {
	      tmp.push_back(Term(query->getOntology()->getNamespace() + p, true));
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

	  if (p.find('#') == std::string::npos)
	    {
	      tmp.push_back(Term(query->getOntology()->getNamespace() + p, true));
	    }
	  else
	    {
	      tmp.push_back(Term(p, true));
	    }

	  tmp.insert(tmp.end(), out.begin(), out.end());

	  PluginAtom::Answer::addTuple(tmp);
	}
      else if (query->getDLQuery().isBoolean()) // ground query
	{
	  PluginAtom::Answer::addTuple(pat);
	}
      else // full retrieval query
	{
	  PluginAtom::Answer::addTuple(out);
	}
    }
}
