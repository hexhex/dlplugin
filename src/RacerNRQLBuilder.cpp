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
 * @file   RacerNRQLBuilder.cpp
 * @author Thomas Krennwallner
 * @date   Thu Jul 27 14:22:55 2006
 * 
 * @brief  Various builders for nRQL requests.
 * 
 * @todo injective variables are not properly calculated (in
 * {W1!=W2,W2!=W3} we create 3 injective variables ?W1,?W2,?W3
 * s.t. W1!=W2!=W3, but this is wrong since it is allowed that
 * W1==W3).
 * 
 */

#include "RacerNRQLBuilder.h"
#include "RacerNRQL.h"
#include "Query.h"
#include "DLError.h"

#include <iosfwd>
#include <sstream>

using namespace dlvhex::dl::racer;


bool
NRQLBaseBuilder::createBody(std::ostream& /* stream */, const Query& /* query */) const
  throw(DLBuildingError)
{
  return false;
}


bool
NRQLBaseBuilder::createHead(std::ostream& stream, const Query& query) const
  throw(DLBuildingError)
{
  const DLQuery::shared_pointer& dlq = query.getDLQuery();
  const Tuple& pat = dlq->getPatternTuple();
  bool isEmpty = true;

  ///@todo injective variable calculation is incorrect, see file header

  std::set<Term> injectiveVars;

  if (dlq->isConjQuery())
    {
      // get all inequalities
      const AtomSet& as = dlq->getConjQuery();
      AtomSet injCandidates;

      as.matchPredicate("!=", injCandidates);

      for (AtomSet::const_iterator it = injCandidates.begin(); it != injCandidates.end(); ++it)
	{
	  if (it->getArity() == 2) // ignore malformed (in)equalities
	    {
	      const Tuple& t = it->getArguments();
	      const Term& t0 = t[0];
	      const Term& t1 = t[1];
	      
	      if (t0.isVariable())
		injectiveVars.insert(t0);
	      
	      if (t1.isVariable())
		injectiveVars.insert(t1);
	    }
	}
    }
  else if (dlq->isUnionConjQuery())
    {
      ///@todo UCQ ignores inequalities here
    }

  // Iterate through the output list and build a nRQL head. Anonymous
  // variables are ignored, they are going to be taken care of when we
  // call Answer::addTuple().
  for (Tuple::const_iterator it = pat.begin(); it != pat.end(); ++it)
    {
      if (!isEmpty) // this skips the beginning of the output
	{
	  stream << ' ';
	}
      
      if (it->isVariable()) // variable
	{
	  isEmpty = false;

	  if (injectiveVars.find(*it) == injectiveVars.end())
	    {
	      stream << ABoxQueryVariable(*it, ABoxQueryVariable::VariableType::noninjective);
	    }
	  else // injective
	    {
	      stream << ABoxQueryVariable(*it);
	    }
	}
      else if (!it->isAnon()) // individual
	{
	  isEmpty = false;

	  stream << ABoxQueryIndividual(*it, dlq->getOntology()->getNamespace());
	}
    }

  return !isEmpty;
}


namespace dlvhex {
  namespace dl {
    namespace racer {

      /// base class for transforming Atom objects to ABoxAssertion objects
      struct InterToAssertion : public std::unary_function<const Atom&, void>
      {
	mutable std::ostream& stream;
	mutable AtomSet::atomset_t::size_type count;
	const Query& query;
	mutable bool& empty;
	bool abox;
	
	InterToAssertion(std::ostream& s,
			 AtomSet::atomset_t::size_type c,
			 const Query& q,
			 bool& isEmpty,
			 bool withABox = false)
	  : stream(s), count(c), query(q), empty(isEmpty), abox(withABox)
	{ }

	void
	operator() (const Atom& a) const
	{
	  this->empty = false;
	  
	  std::string nspace = query.getDLQuery()->getOntology()->getNamespace();

	  if (a.getArity() == 1) // concept assertion
	    {
	      ABoxQueryConcept::const_pointer c =
		new ABoxQueryConcept(a.getArgument(0), nspace);
	      ABoxQueryIndividual::const_pointer i =
		new ABoxQueryIndividual(a.getArgument(1), nspace);
	      
	      if (abox)
		{
		  stream << ABoxAddConceptAssertion(a.isStronglyNegated() ?
						    new ABoxNegatedConcept(c) : c,
						    i,
						    query.getKBManager().getKBName()
						    );
		}
	      else
		{
		  stream << ABoxInstanceAssertion(a.isStronglyNegated() ?
						  new ABoxNegatedConcept(c) : c,
						  i
						  );
		}
	    }
	  else if (a.getArity() == 2 && a.isStronglyNegated()) // negated role assertion
	    {
	      ABoxQueryRole::const_pointer r = 
		new ABoxQueryRole(a.getArgument(0), nspace);
	      ABoxQueryIndividual* i1 =
		new ABoxQueryIndividual(a.getArgument(1), nspace);
	      ABoxQueryIndividual* i2 =
		new ABoxQueryIndividual(a.getArgument(2), nspace);
	      
	      ABoxOneOfConcept::IndividualVector iv;
	      iv.push_back(i2);
	      
	      // -R(a,b) -> (instance a (not (some R (one-of b))))
	      // does not work? seems like there is a bug in Racer
	      if (abox)
		{
		  stream << ABoxAddConceptAssertion(new ABoxNegatedConcept
						    (new ABoxSomeConcept
						     (r, new ABoxOneOfConcept(iv))
						     ),
						    i1,
						    query.getKBManager().getKBName()
						    );

		}
	      else
		{
		  stream << ABoxInstanceAssertion(new ABoxNegatedConcept
						  (new ABoxSomeConcept
						   (r, new ABoxOneOfConcept(iv))
						   ),
						  i1
						  );
		}
	    }
	  else if (a.getArity() == 2 && !a.isStronglyNegated())
	    {
	      ABoxQueryRole::const_pointer r = 
		new ABoxQueryRole(a.getArgument(0), nspace);
	      ABoxQueryIndividual::const_pointer i1 =
		new ABoxQueryIndividual(a.getArgument(1), nspace);
	      ABoxQueryIndividual::const_pointer i2 =
		new ABoxQueryIndividual(a.getArgument(2), nspace);
	      
	      if (abox)
		{
		  stream << ABoxAddRoleAssertion(r, i1, i2, query.getKBManager().getKBName());
		}
	      else
		{
		  stream << ABoxRelatedAssertion(r, i1, i2);
		}
	    }
	  else
	    {
	      std::ostringstream oss;
	      oss << a << " has wrong arity.";
	      throw DLBuildingError(oss.str());
	    }
	  
	  if (--count)
	    {
	      stream.put(' ');
	    }
	}
	
      };
      
    } // namespace racer
  } // namespace dl
} // namespace dlvhex


bool
NRQLBaseBuilder::createPremise(std::ostream& stream, const Query& query) const
  throw(DLBuildingError)
{
  bool isEmpty = true;

  const AtomSet& ints = query.getProjectedInterpretation();

  if (!ints.empty())
    {
      std::for_each(ints.begin(), ints.end(),
		    InterToAssertion(stream, ints.size(), query, isEmpty)
		    );
    }

  ///@todo this is a preliminary workaround for the Racer abox-cloning bug 
#if 1
  if (isEmpty)
    {
      isEmpty = false;

      stream << 
	ABoxInstanceAssertion
	(new ABoxQueryConcept("foo"),
	 new ABoxQueryIndividual("bar")
	 );
    }
#endif

  return !isEmpty;
}


bool
NRQLStateBuilder::createPremise(std::ostream& stream, const Query& query) const
  throw(DLBuildingError)
{
  bool isEmpty = true;

  const AtomSet& ints = query.getProjectedInterpretation();

  if (!ints.empty())
    {
      std::for_each(ints.begin(), ints.end(),
		    InterToAssertion(stream, ints.size(), query, isEmpty, true)
		    );
    }

  ///@todo this is a preliminary workaround for the Racer abox-cloning bug 
#if 1
  if (isEmpty)
    {
      isEmpty = false;

      stream << 
	ABoxAddConceptAssertion
	(new ABoxQueryConcept("foo"),
	 new ABoxQueryIndividual("bar"),
	 query.getKBManager().getKBName()
	 );
    }
#endif

  return !isEmpty;
}


bool
NRQLConjunctionBuilder::createBody(std::ostream& stream, const Query& query) const
  throw(DLBuildingError)
{
  const DLQuery::shared_pointer& dlq = query.getDLQuery();
  const AtomSet& as = dlq->getConjQuery();
  const std::string& nspace = dlq->getOntology()->getNamespace();

  NRQLConjunction body;

  ///@todo injective variable calculation is incorrect, see file header

  // get all inequalities
  AtomSet injCandidates;
  as.matchPredicate("!=", injCandidates);

  std::set<Term> injectiveVars;

  for (AtomSet::const_iterator it = injCandidates.begin(); it != injCandidates.end(); ++it)
    {
      if (it->getArity() == 2) // ignore malformed (in)equalities
	{
	  const Tuple& t = it->getArguments();
	  const Term& t0 = t[0];
	  const Term& t1 = t[1];

	  if (t0.isVariable())
	    injectiveVars.insert(t0);
	  
	  if (t1.isVariable())
	    injectiveVars.insert(t1);
	}
    }

  for (AtomSet::const_iterator it = as.begin(); it != as.end(); ++it)
    {
      switch (it->getArity())
	{
	case 2: // role query or (in)equality
	  {
	    const Term& t1 = it->getArgument(1);
	    const Term& t2 = it->getArgument(2);
	    ABoxQueryObject* o1 = 0;
	    ABoxQueryObject* o2 = 0;

	    if (t1.isVariable())
	      {
		if (injectiveVars.find(t1) == injectiveVars.end())
		  {
		    o1 = new ABoxQueryVariable(t1, ABoxQueryVariable::VariableType::noninjective);
		  }
		else // injective
		  {
		    o1 = new ABoxQueryVariable(t1);
		  }
	      }
	    else
	      {
		o1 = new ABoxQueryIndividual(t1, nspace);
	      }
	    
	    if (t2.isVariable())
	      {
		if (injectiveVars.find(t2) == injectiveVars.end())
		  {
		    o2 = new ABoxQueryVariable(t2, ABoxQueryVariable::VariableType::noninjective);
		  }
		else // injective
		  {
		    o2 = new ABoxQueryVariable(t2);
		  }
	      }
	    else
	      {
		o2 = new ABoxQueryIndividual(t2, nspace);
	      }

	    const Term& pred = it->getPredicate();
	    
	    if (pred == Term("==")) // equality
	      {
		body.addAtom(new NRQLQueryAtom(new SameAsQuery(o1, o2)));
	      }
	    else if (pred == Term("!=")) // ignore inequalities -> use injective variables
	      {
		// body.addAtom(new NRQLQueryAtom(new NAFQuery(new SameAsQuery(o1, o2))));
		delete o1;
		delete o2;
	      }
 	    else // role query
	      {
		body.addAtom(new NRQLQueryAtom
			     (new RoleQuery
			      (new ABoxQueryRole(pred, nspace), o1, o2)
			      )
			     );
	      }
	  }
	  break;

	case 1: // concept query
	  {
	    const Term& t1 = it->getArgument(1);
	    ABoxQueryObject* o1 = 0;

	    if (t1.isVariable())
	      {
		if (injectiveVars.find(t1) == injectiveVars.end())
		  {
		    o1 = new ABoxQueryVariable(t1, ABoxQueryVariable::VariableType::noninjective);
		  }
		else // injective
		  {
		    o1 = new ABoxQueryVariable(t1);
		  }
	      }
	    else
	      {
		o1 = new ABoxQueryIndividual(t1, nspace);
	      }

	    if (it->isStronglyNegated())
	      {
		body.addAtom(new NRQLQueryAtom
			     (new ConceptQuery
			      (new ABoxNegatedConcept
			       (new ABoxQueryConcept(it->getPredicate(), nspace)), o1)
			      )
			     );
	      }
	    else
	      {
		body.addAtom(new NRQLQueryAtom
			     (new ConceptQuery
			      (new ABoxQueryConcept(it->getPredicate(), nspace), o1)
			      )
			     );
	      }
	  }
	  break;

	default: // bail out
	  throw DLBuildingError("wrong arity in conjunctive query");
	  break;
	}
    }

  stream << body;

  return true;
}




bool
NRQLDisjunctionBuilder::createBody(std::ostream& stream, const Query& query) const
  throw(DLBuildingError)
{
  const DLQuery::shared_pointer& dlq = query.getDLQuery();
  const std::vector<AtomSet>& as = dlq->getUnionConjQuery();
  const std::string& nspace = dlq->getOntology()->getNamespace();

  ///@todo we don't handle (in)equalities here
  std::set<Term> injectiveVars;

  NRQLUnion unbody;

  for (std::vector<AtomSet>::const_iterator it = as.begin(); it != as.end(); ++it)
    {
      NRQLConjunction* body = new NRQLConjunction;

      for (AtomSet::const_iterator it2 = it->begin(); it2 != it->end(); ++it2)
	{
	  switch (it2->getArity())
	    {
	    case 2: // role query or (in)equality
	      {
		const Term& t1 = it2->getArgument(1);
		const Term& t2 = it2->getArgument(2);
		ABoxQueryObject* o1 = 0;
		ABoxQueryObject* o2 = 0;
		
		if (t1.isVariable())
		  {
		    if (injectiveVars.find(t1) == injectiveVars.end())
		      {
			o1 = new ABoxQueryVariable(t1, ABoxQueryVariable::VariableType::noninjective);
		      }
		    else // injective
		      {
			o1 = new ABoxQueryVariable(t1);
		      }
		  }
		else
		  {
		    o1 = new ABoxQueryIndividual(t1, nspace);
		  }
		
		if (t2.isVariable())
		  {
		    if (injectiveVars.find(t2) == injectiveVars.end())
		      {
			o2 = new ABoxQueryVariable(t2, ABoxQueryVariable::VariableType::noninjective);
		      }
		    else // injective
		      {
			o2 = new ABoxQueryVariable(t2);
		      }
		  }
		else
		  {
		    o2 = new ABoxQueryIndividual(t2, nspace);
		  }
		
		const Term& pred = it2->getPredicate();
		
		if (pred == Term("==")) // equality
		  {
		    body->addAtom(new NRQLQueryAtom(new SameAsQuery(o1, o2)));
		  }
		else if (pred == Term("!=")) // ignore inequalities -> use injective variables
		  {
		    // body.addAtom(new NRQLQueryAtom(new NAFQuery(new SameAsQuery(o1, o2))));
		    delete o1;
		    delete o2;
		  }
		else // role query
		  {
		    body->addAtom(new NRQLQueryAtom
				  (new RoleQuery
				   (new ABoxQueryRole(pred, nspace), o1, o2)
				   )
				  );
		  }
	      }
	      break;
	      
	    case 1: // concept query
	      {
		const Term& t1 = it2->getArgument(1);
		ABoxQueryObject* o1 = 0;
		
		if (t1.isVariable())
		  {
		    if (injectiveVars.find(t1) == injectiveVars.end())
		      {
			o1 = new ABoxQueryVariable(t1, ABoxQueryVariable::VariableType::noninjective);
		      }
		    else // injective
		      {
			o1 = new ABoxQueryVariable(t1);
		      }
		  }
		else
		  {
		    o1 = new ABoxQueryIndividual(t1, nspace);
		  }
		
		if (it2->isStronglyNegated())
		  {
		    body->addAtom(new NRQLQueryAtom
				  (new ConceptQuery
				   (new ABoxNegatedConcept
				    (new ABoxQueryConcept(it2->getPredicate(), nspace)), o1)
				   )
				  );
		  }
		else
		  {
		    body->addAtom(new NRQLQueryAtom
				  (new ConceptQuery
				   (new ABoxQueryConcept(it2->getPredicate(), nspace), o1)
				   )
				  );
		  }
	      }
	      break;
	      
	    default: // bail out
	      throw DLBuildingError("wrong arity in conjunctive query");
	      break;
	    }
	}
      
      unbody.addBody(body);
    }

  stream << unbody;

  return true;
}


bool
NRQLDatatypeBuilder::createHead(std::ostream& stream, const Query& query) const
  throw(DLBuildingError)
{
  const DLQuery::shared_pointer& dlq = query.getDLQuery();
  const Tuple& pat = dlq->getPatternTuple();

  unsigned long type = dlq->getTypeFlags() & std::numeric_limits<unsigned long>::max();

  if (!(type == 0x0 || type == 0x1) || pat.size() != 2)
    {
      throw DLBuildingError("Incompatible pattern supplied.");
    }

  if (type == 0x1) // (const,variable) pattern
    {
      // only retrieve the datatype, let Answer add the
      // corresponding individual
      stream << ABoxQueryVariable("Y",
				  ABoxQueryVariable::VariableType::noninjective | ABoxQueryVariable::VariableType::substrate
				  );
    }
  else if (type == 0x0) // (variable,variable) pattern
    {
      stream << ABoxQueryVariable("X",
				  ABoxQueryVariable::VariableType::substrate)
	     << ' '
	     << ABoxQueryVariable("Y", ABoxQueryVariable::VariableType::noninjective | ABoxQueryVariable::VariableType::substrate
				  );
    }

  return true;
}


bool
NRQLDatatypeBuilder::createBody(std::ostream& stream, const Query& query) const
  throw(DLBuildingError)
{
  const DLQuery::shared_pointer& dlq = query.getDLQuery();
  const Term& q = dlq->getQuery();
  const Tuple& pat = dlq->getPatternTuple();
  const std::string& nspace = dlq->getOntology()->getNamespace();

  unsigned long type = dlq->getTypeFlags() & std::numeric_limits<unsigned long>::max();

  if (!(type == 0x0 || type == 0x1) || pat.size() != 2)
    {
      throw DLBuildingError("Incompatible pattern supplied.");
    }

  if (type == 0x1) // (const,variable) pattern
    {
      NRQLConjunction body;
	  
      body.addAtom(new NRQLQueryAtom
		   (new RoleQuery
		    (new ABoxQueryRole(q, nspace, true),
		     new ABoxQueryVariable("X", ABoxQueryVariable::VariableType::substrate),
		     new ABoxQueryVariable("Y", ABoxQueryVariable::VariableType::noninjective | ABoxQueryVariable::VariableType::substrate)
		     )
		    )
		   );
      body.addAtom(new NRQLQueryAtom
		   (new SameAsQuery
		    (new ABoxQueryVariable("X"),
		     new ABoxQueryIndividual
		     (pat[0], nspace)
		     )
		    )
		   );

      stream << body;
    }
  else if (type == 0x0) // (variable,variable) pattern
    {
      stream <<
	NRQLQueryAtom
	(new RoleQuery
	 (new ABoxQueryRole(q, nspace, true),
	  new ABoxQueryVariable("X", ABoxQueryVariable::VariableType::substrate),
	  new ABoxQueryVariable("Y", ABoxQueryVariable::VariableType::noninjective | ABoxQueryVariable::VariableType::substrate)
	  )
	 );
    }

  return true;
}


// Local Variables:
// mode: C++
// End:
