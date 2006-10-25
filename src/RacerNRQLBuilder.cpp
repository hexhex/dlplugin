/* -*- C++ -*- */

/**
 * @file   RacerNRQLBuilder.cpp
 * @author Thomas Krennwallner
 * @date   Thu Jul 27 14:22:55 2006
 * 
 * @brief  Various builders for nRQL requests.
 * 
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

	  stream << ABoxQueryVariable(*it, ABoxQueryVariable::VariableType::noninjective);
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

	  if (a.getArity() == 2) // concept assertion
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
	  else if (a.getArity() == 3 && a.isStronglyNegated()) // negated role assertion
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
	  else if (a.getArity() == 3 && !a.isStronglyNegated())
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

  for (AtomSet::const_iterator it = as.begin(); it != as.end(); ++it)
    {
      switch (it->getArity())
	{
	case 3: // role query
	  {
	    const Term& t1 = it->getArgument(1);
	    const Term& t2 = it->getArgument(2);
	    ABoxQueryObject* o1 = 0;
	    ABoxQueryObject* o2 = 0;

	    if (t1.isVariable())
	      {
		o1 = new ABoxQueryVariable(t1, ABoxQueryVariable::VariableType::noninjective);
	      }
	    else
	      {
		o1 = new ABoxQueryIndividual(t1, nspace);
	      }
	    
	    if (t2.isVariable())
	      {
		o2 = new ABoxQueryVariable(t2, ABoxQueryVariable::VariableType::noninjective);
	      }
	    else
	      {
		o2 = new ABoxQueryIndividual(t2, nspace);
	      }
	    
	    body.addAtom(new NRQLQueryAtom
			 (new RoleQuery
			  (new ABoxQueryRole(it->getPredicate(), nspace), o1, o2)
			  )
			 );
	  }
	  break;

	case 2: // concept query
	  {
	    const Term& t1 = it->getArgument(1);
	    ABoxQueryObject* o1 = 0;

	    if (t1.isVariable())
	      {
		o1 = new ABoxQueryVariable(t1, ABoxQueryVariable::VariableType::noninjective);
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
