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

#include <dlvhex/ComfortPluginInterface.hpp>

#include <string>
#include <iterator>
#include <limits>
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
	  if (a.size() > 0)
	    {
	      for (std::set<ComfortTuple>::const_iterator it = a.begin();
		   it != a.end(); ++it)
		{
		  os.put('(');
		  if (!it->empty())
		    {
		      std::copy(it->begin(), --it->end(),
				std::ostream_iterator<ComfortTerm>(os, ", "));
		      os << it->back();
		    }
		  os << ")";
                  if (it != a.end()) os << ", ";
		}
	    }
	}
      
      return os;
    }

  } // namespace dl
} //namespace dlvhex


Answer::Answer(const Query* q)
  : ComfortPluginAtom::ComfortAnswer(),
    errorMsg(),
    warningMsg(),
    isIncoherent(false),
    answer(false),
    query(q)
{ }


Answer::Answer(const Answer& a)
  : ComfortPluginAtom::ComfortAnswer(a),
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
Answer::addTuple(const ComfortTuple& out)
{
  if (query == 0) // just in case we don't have a corresponding query
    {
      insert(out);
    }
  else if (query->getDLQuery()->isConjQuery() ||
	   query->getDLQuery()->isUnionConjQuery()) // in CQs and UCQs
						    // we only check
						    // for anon. vars
    {


      const DLQuery::shared_pointer& dlq = query->getDLQuery();
      const ComfortTuple& pat = dlq->getPatternTuple();

      if (out.size() == pat.size())
	{
	  insert(out);
	}
      else // take care of anonymous variables
	{
	  ComfortTuple tmp;
	  ComfortTuple::const_iterator pit = pat.begin();
	  ComfortTuple::const_iterator oit = out.begin();

          assert(out.size() <= pat.size());

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
		  tmp.push_back(ComfortTerm::createConstant(""));
		}
	      else
		{
                  assert(oit != out.end());
		  tmp.push_back(*oit);
		  ++oit;
		}
	    }

	  insert(tmp);
	}
    }
  else // a non-conjunctive query needs special treatment
    {
      const DLQuery::shared_pointer& dlq = query->getDLQuery();
      const ComfortTuple& pat = dlq->getPatternTuple();
      unsigned long type = dlq->getTypeFlags() & std::numeric_limits<unsigned long>::max();
      const std::string& nspace = dlq->getOntology()->getNamespace();

      if (type == 0x2) // left retrieval
	{
	  ComfortTuple tmp(out);

	  std::string p = pat[1].getUnquotedString();

	  if (!URI::isValid(p))
	    {
	      tmp.push_back(ComfortTerm::createConstant(nspace + p));
	    }
	  else
	    {
	      tmp.push_back(ComfortTerm::createConstant(p));
	    }

	  insert(tmp);
	}
      else if (type == 0x1) // right retrieval
	{
	  ComfortTuple tmp;

	  std::string p = pat[0].getUnquotedString();

	  if (!URI::isValid(p))
	    {
	      tmp.push_back(ComfortTerm::createConstant(nspace + p));
	    }
	  else
	    {
	      tmp.push_back(ComfortTerm::createConstant(p));
	    }

	  tmp.insert(tmp.end(), out.begin(), out.end());

	  insert(tmp);
	}
      else if (dlq->isBoolean()) // ground query
	{
	  insert(pat);
	}
      else // full retrieval query
	{
          assert(pat.size() == out.size());
	  insert(out);
	}
    }
}


// Local Variables:
// mode: C++
// End:
