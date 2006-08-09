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
#include "RacerError.h"

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/replace.hpp>


using namespace dlvhex::racer;


namespace dlvhex {
  namespace racer {

    /**
     * @brief A TokenizerFunc for boost::tokenizer<>, model of
     * TokenizerFunction, tokenizes comma-separated atoms in a string.
     *
     * @see boost::tokenizer
     * http://www.boost.org/libs/tokenizer/index.html
     */
    class AtomSeparator
    {
    private:
      enum ScannerState
	{
	  INITIAL = 0,
	  PARENOPEN = 1,
	  PARENCLOSE = 2
	};

      /// internal state keeps track of opened/closed parentheses
      ScannerState state;

    public:
      AtomSeparator() : state(INITIAL) { }
      
      /** 
       * Tokenizes list of atoms found in next and appends each atom
       * to tok.
       *
       * @param next
       * @param end
       * @param tok
       * 
       * @return true if a full atom could be parsed, false otherwise.
       */
      template <typename InputIterator, typename Token>
      bool
      operator() (InputIterator& next, InputIterator end, Token& tok)
	throw (RacerParsingError)
      {
	tok = Token();
	
	if (next == end)
	  {
	    if (state == PARENCLOSE) return false;
	    else throw RacerParsingError("this is foo");
	  }

	bool unexpected = false;
	
	for (; next != end; ++next)
	  {
	    switch (*next)
	      {
	      case '(':
		if (state == INITIAL) { state = PARENOPEN; tok += *next; }
		else unexpected = true;
		break;
		
	      case ')':
		if (state == PARENOPEN) { state = PARENCLOSE; tok += *next; }
		else unexpected = true;
		break;
		
	      case ',':
		if (state == PARENOPEN) { tok += *next; } // ignore
		else if (state == PARENCLOSE) { reset(); ++next; return true; } // done
		else unexpected = true;
		break;

	      case ' ':
	      case '\t':
		tok += *next; // always ignore whitespaces
		break;

	      default:
		if (state == INITIAL || state == PARENOPEN) { tok += *next; }
		else unexpected = true;
		break;
	      }

	    if (unexpected)
	      throw RacerParsingError("unexpected " + *next);
	  }
	
	if (state == PARENCLOSE)
	  {
	    return true;
	  }
	
	return false;
      }

      void
      reset()
      {
	state = INITIAL;
      }

    };

  } // namespace racer
} //namespace dlvhex


QueryCtx::QueryCtx(const PluginAtom::Query& query) throw (RacerError)
{
  const Tuple& inputtuple = query.getInputTuple();

  if (inputtuple.size() < 4)
    {
      throw RacerError("Incompatible input list.");
    }

  // inputtuple[0] contains the KB URI constant
  std::string ontostr = inputtuple[0].getUnquotedString();

  Ontology::shared_pointer onto;

  // get the ontology and parse the default namespace
  try
    {
      onto = Ontology::createOntology(ontostr);
    }
  catch (RacerError& e)
    {
      ///@todo should we ignore this?
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
	  Tuple tup;

	  // tokenize the atoms of the query string
	  boost::tokenizer<AtomSeparator> tok(qstr);

	  for (boost::tokenizer<AtomSeparator>::const_iterator it = tok.begin();
	       it != tok.end(); ++it)
	    {
	      tup.clear();

	      std::string atom = *it;

	      //
	      // determine the namespace prefix and replace every
	      // abbr. name by the corresponding fully-fledged
	      // namespace which can be found in Term::namespaces
	      //
	      for (std::vector<std::pair<std::string,std::string> >::iterator ns = Term::namespaces.begin();
		   ns != Term::namespaces.end();
		   ++ns)
		{
		  boost::replace_all(atom, ns->second + ":", ns->first);
		}

	      std::string::size_type pred = atom.find('(');
	      std::string::size_type t1 = atom.find(',');
	      std::string::size_type t2 = atom.find(')');

	      // predicate is always quoted, otherwise we would end up
	      // in a higher-order atom if concept or role name is
	      // uppercase
	      std::string predicate = atom.substr(0, pred);
	      boost::trim(predicate);

	      bool isNegated = predicate[0] == '-' ? true : false;

	      if (isNegated)
		{
		  predicate = "\"" + predicate.substr(1) + "\"";
		}
	      else
		{
		  predicate = "\"" + predicate + "\"";
		}

	      if (t1 != std::string::npos)
		{
		  std::string a1 = atom.substr(pred + 1, t1 - pred - 1);
		  std::string a2 = atom.substr(t1 + 1, t2 - t1 - 1);

		  boost::trim(a1);
		  boost::trim(a2);

		  tup.push_back(Term(a1));
		  tup.push_back(Term(a2));
		}
	      else
		{
		  std::string a1 = atom.substr(pred + 1, t2 - pred - 1);
		  boost::trim(a1);
		  tup.push_back(Term(a1));
		}

	      AtomPtr ap(new Atom(predicate, tup, isNegated));

	      as.insert(ap);
	    }

	  q = new Query(onto,
			inputtuple[1],
			inputtuple[2],
			inputtuple[3],
			inputtuple[4],
			DLQuery(as, query.getPatternTuple()),
			query.getInterpretation()
			);
	}
      else // this is a plain query
	{
	  std::string querystr = inputtuple[5].getUnquotedString();

	  if (querystr.find('#') == std::string::npos) // no namespace in query
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

	  q = new Query(onto,
			inputtuple[1],
			inputtuple[2],
			inputtuple[3],
			inputtuple[4],
			DLQuery(qu, query.getPatternTuple()),
			query.getInterpretation()
			);
	}
    }
  else // no query term
    {
      q = new Query(onto,
		    inputtuple[1],
		    inputtuple[2],
		    inputtuple[3],
		    inputtuple[4],
		    DLQuery(Term(), query.getPatternTuple()),
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

