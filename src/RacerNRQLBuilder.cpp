/* -*- C++ -*- */

/**
 * @file   RacerNRQLBuilder.cpp
 * @author Thomas Krennwallner
 * @date   Thu Jul 27 14:22:55 2006
 * 
 * @brief  
 * 
 * 
 */

#include "RacerNRQLBuilder.h"
#include "RacerNRQL.h"
#include "Query.h"
#include "RacerError.h"

#include <iosfwd>

using namespace dlvhex::racer;


NRQLBuilder::~NRQLBuilder()
{ }


bool
NRQLBuilder::createBody(std::ostream& /* stream */, const Query& /* query */) const
  throw(RacerBuildingError)
{
  return false;
}


bool
NRQLBuilder::createHead(std::ostream& stream, const Query& query) const
  throw(RacerBuildingError)
{
  const DLQuery& dlq = query.getDLQuery();
  const Tuple& pat = dlq.getPatternTuple();
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

	  stream <<
	    ABoxQueryVariable
	    (it->getVariable(),
	     ABoxQueryVariable::VariableType::noninjective
	     );
	}
      else if (!it->isAnon()) // individual
	{
	  isEmpty = false;

	  stream << ABoxQueryIndividual(it->getUnquotedString(), query.getNamespace());
	}
    }

  return !isEmpty;
}


bool
NRQLBuilder::createPremise(std::ostream& stream, const Query& query) const
  throw(RacerBuildingError)
{
  const AtomSet& ints = query.getInterpretation();
  bool isEmpty = true;

  for (AtomSet::const_iterator it = ints.begin(); it != ints.end(); it++)
    {
      if (!isEmpty) // this skips the beginning of the output
	{
	  stream << ' ';
	}

      const Atom& a = *it;
      const Term pred = a.getArgument(0);

      if (pred == query.getPlusC()) // plusC
	{
	  if (a.getArity() < 3)
	    {
	      throw RacerBuildingError(pred.getUnquotedString() + " has wrong arity.");
	    }

	  isEmpty = false;

	  stream <<
	    ABoxInstance
	    (new ABoxQueryConcept
	     (a.getArgument(1).getUnquotedString(),
	      query.getNamespace()),
	     new ABoxQueryIndividual
	     (a.getArgument(2).getUnquotedString(),
	      query.getNamespace())
	     );
	}
      else if (pred == query.getMinusC()) // minusC
	{
	  if (a.getArity() < 3)
	    {
	      throw RacerBuildingError(pred.getUnquotedString() + " has wrong arity.");
	    }

	  isEmpty = false;

	  stream <<
	    ABoxInstance
	    (new ABoxNegatedConcept
	     (new ABoxQueryConcept
	      (a.getArgument(1).getUnquotedString(),
	       query.getNamespace())
	      ),
	     new ABoxQueryIndividual
	     (a.getArgument(2).getUnquotedString(),
	      query.getNamespace())
	     );
	}
      else if (pred == query.getPlusR()) // plusR
	{
	  if (a.getArity() < 4)
	    {
	      throw RacerBuildingError(pred.getUnquotedString() + " has wrong arity.");
	    }

	  isEmpty = false;

	  stream << 
	    ABoxRelated
	    (new ABoxQueryRole
	     (a.getArgument(1).getUnquotedString(),
	      query.getNamespace()),
	     new ABoxQueryIndividual
	     (a.getArgument(2).getUnquotedString(),
	      query.getNamespace()),
	     new ABoxQueryIndividual
	     (a.getArgument(3).getUnquotedString(),
	      query.getNamespace())
	     );
	}
      else if (pred == query.getMinusR()) // minusR
	{
	  if (a.getArity() < 4)
	    {
	      throw RacerBuildingError(pred.getUnquotedString() + " has wrong arity.");
	    }

	  isEmpty = false;

	  ABoxOneOfConcept::IndividualVector iv;
	  iv.push_back(new ABoxQueryIndividual
		       (a.getArgument(3).getUnquotedString(),
			query.getNamespace())
		       );

	  // -R(a,b) -> (instance a (not (some R (one-of b))))
	  // does not work? seems like there is a bug in Racer
	  stream << 
	    ABoxInstance
	    (new ABoxNegatedConcept
	     (new ABoxSomeConcept
	      (new ABoxQueryRole
	       (a.getArgument(1).getUnquotedString(),
		query.getNamespace()),
	       new ABoxOneOfConcept(iv)
	       )
	      ),
	     new ABoxQueryIndividual
	     (a.getArgument(2).getUnquotedString(),
	      query.getNamespace())
	     );
	}
      else
	{
	  // just ignore unknown stuff...
	}
    }

  ///@todo this is a preliminary workaround for the Racer abox-cloning bug 
#if 1
  if (isEmpty)
    {
      isEmpty = false;

      stream << 
	ABoxInstance
	(new ABoxQueryConcept("foo"),
	 new ABoxQueryIndividual("bar")
	 );
    }
#endif

  return !isEmpty;
}



bool
NRQLConjunctionBuilder::createBody(std::ostream& stream, const Query& query) const
  throw(RacerBuildingError)
{
  const DLQuery& dlq = query.getDLQuery();
  const AtomSet& as = dlq.getConjQuery();

  NRQLConjunction body;

  for (AtomSet::const_iterator it = as.begin(); it != as.end(); it++)
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
		o1 = new ABoxQueryVariable
		  (t1.getVariable(),
		   ABoxQueryVariable::VariableType::noninjective
		   );
	      }
	    else
	      {
		o1 = new ABoxQueryIndividual
		  (t1.getUnquotedString(),
		   query.getNamespace()
		   );
	      }
	    
	    if (t2.isVariable())
	      {
		o2 = new ABoxQueryVariable
		  (t2.getVariable(),
		   ABoxQueryVariable::VariableType::noninjective
		   );
	      }
	    else
	      {
		o2 = new ABoxQueryIndividual
		  (t2.getUnquotedString(),
		   query.getNamespace()
		   );
	      }
	    
	    body.addAtom(new NRQLQueryAtom
			 (new RoleQuery
			  (new ABoxQueryRole
			   (it->getPredicate().getUnquotedString(),
			    query.getNamespace()
			    ), o1, o2
			   )
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
		o1 = new ABoxQueryVariable
		  (t1.getVariable(),
		   ABoxQueryVariable::VariableType::noninjective
		   );
	      }
	    else
	      {
		o1 = new ABoxQueryIndividual
		  (t1.getUnquotedString(),
		   query.getNamespace()
		   );
	      }

	    if (it->isStronglyNegated())
	      {
		body.addAtom(new NRQLQueryAtom
			     (new ConceptQuery
			      (new ABoxNegatedConcept
			       (new ABoxQueryConcept
				(it->getPredicate().getUnquotedString(),
				 query.getNamespace()
				 )
				), o1
			       )
			      )
			     );
	      }
	    else
	      {
		body.addAtom(new NRQLQueryAtom
			     (new ConceptQuery
			      (new ABoxQueryConcept
			       (it->getPredicate().getUnquotedString(),
				query.getNamespace()
				), o1
			       )
			      )
			     );
	      }
	  }
	  break;

	default: // bail out
	  throw RacerBuildingError("wrong arity in conjunctive query");
	  break;
	}
    }

  stream << body;

  return true;
}


bool
NRQLDatatypeBuilder::createHead(std::ostream& stream, const Query& query) const
  throw(RacerBuildingError)
{
  const DLQuery& dlq = query.getDLQuery();
  const Tuple& pat = dlq.getPatternTuple();

  unsigned long type = dlq.getTypeFlags() & std::numeric_limits<unsigned long>::max();

  if (!(type == 0x0 || type == 0x1) || pat.size() != 2)
    {
      throw RacerBuildingError("Incompatible pattern supplied.");
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
  throw(RacerBuildingError)
{
  const DLQuery& dlq = query.getDLQuery();
  const Term& q = dlq.getQuery();
  const Tuple& pat = dlq.getPatternTuple();

  unsigned long type = dlq.getTypeFlags() & std::numeric_limits<unsigned long>::max();

  if (!(type == 0x0 || type == 0x1) || pat.size() != 2)
    {
      throw RacerBuildingError("Incompatible pattern supplied.");
    }

  if (type == 0x1) // (const,variable) pattern
    {
      NRQLConjunction body;
	  
      body.addAtom(new NRQLQueryAtom
		   (new RoleQuery
		    (new ABoxQueryRole(q.getUnquotedString(),
				       query.getNamespace(),
				       true),
		     new ABoxQueryVariable("X", ABoxQueryVariable::VariableType::substrate),
		     new ABoxQueryVariable("Y", ABoxQueryVariable::VariableType::noninjective | ABoxQueryVariable::VariableType::substrate)
		     )
		    )
		   );
      body.addAtom(new NRQLQueryAtom
		   (new SameAsQuery
		    (new ABoxQueryVariable("X"),
		     new ABoxQueryIndividual
		     (pat[0].getUnquotedString(), query.getNamespace())
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
	 (new ABoxQueryRole(q.getUnquotedString(), query.getNamespace(), true),
	  new ABoxQueryVariable("X", ABoxQueryVariable::VariableType::substrate),
	  new ABoxQueryVariable("Y", ABoxQueryVariable::VariableType::noninjective | ABoxQueryVariable::VariableType::substrate)
	  )
	 );
    }

  return true;
}
