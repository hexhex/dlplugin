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

	  stream << ABoxQueryIndividual(it->getUnquotedString(), query.getOntology()->getNamespace());
	}
    }

  return !isEmpty;
}

namespace dlvhex {
  namespace racer {

    /// base class for transforming Atom objects to ABoxAssertion objects
    struct InterToAssertion
    {
      const Query& query;
      bool& empty;
      bool negate;

      InterToAssertion(const Query& q, bool& isEmpty, bool isNegated)
	: query(q), empty(isEmpty), negate(isNegated)
      { }
    };


    /// transform Atom objects to ABoxInstance objects
    struct InterToInstance : public InterToAssertion
    {
      InterToInstance(const Query& q, bool& isEmpty, bool isNegated = false)
	: InterToAssertion(q, isEmpty, isNegated)
      { }

      ABoxInstance
      operator() (const Atom& a)
      {
	empty = false;

	if (a.getArity() == 2)
	  {
	    ABoxQueryConcept::const_pointer c =
	      new ABoxQueryConcept
	      (a.getArgument(0).getUnquotedString(),
	       query.getOntology()->getNamespace()
	       );
	    ABoxQueryIndividual::const_pointer i =
	      new ABoxQueryIndividual
	      (a.getArgument(1).getUnquotedString(),
	       query.getOntology()->getNamespace()
	       );
	    
	    return ABoxInstance(negate ? new ABoxNegatedConcept(c) : c, i);
	  }
	else if (a.getArity() == 3 && negate)
	  {
	    ABoxQueryRole::const_pointer r = 
	      new ABoxQueryRole
	      (a.getArgument(0).getUnquotedString(),
	       query.getOntology()->getNamespace()
	       );
	    ABoxQueryIndividual* i1 =
	      new ABoxQueryIndividual
	      (a.getArgument(1).getUnquotedString(),
	       query.getOntology()->getNamespace()
	       );
	    ABoxQueryIndividual* i2 =
	      new ABoxQueryIndividual
	      (a.getArgument(2).getUnquotedString(),
	       query.getOntology()->getNamespace()
	       );
	    
	    ABoxOneOfConcept::IndividualVector iv;
	    iv.push_back(i2);

	    // -R(a,b) -> (instance a (not (some R (one-of b))))
	    // does not work? seems like there is a bug in Racer
	    return ABoxInstance
	      (new ABoxNegatedConcept
	       (new ABoxSomeConcept(r, new ABoxOneOfConcept(iv))
		),
	       i1
	       );
	  }

	std::ostringstream oss;
	oss << a << " has wrong arity.";
	throw RacerBuildingError(oss.str());
      }

    };


    /// transform Atom objects to ABoxRelated objects
    struct InterToRelated : public InterToAssertion
    {
      InterToRelated(const Query& q, bool& isEmpty, bool isNegated = false)
	: InterToAssertion(q, isEmpty, isNegated)
      { }


      ABoxRelated
      operator() (const Atom& a)
      {
	empty = false;

	if (a.getArity() == 3 && !negate)
	  {
	    ABoxQueryRole::const_pointer r = 
	      new ABoxQueryRole
	      (a.getArgument(0).getUnquotedString(),
	       query.getOntology()->getNamespace()
	       );
	    ABoxQueryIndividual::const_pointer i1 =
	      new ABoxQueryIndividual
	      (a.getArgument(1).getUnquotedString(),
	     query.getOntology()->getNamespace()
	       );
	    ABoxQueryIndividual::const_pointer i2 =
	      new ABoxQueryIndividual
	      (a.getArgument(2).getUnquotedString(),
	       query.getOntology()->getNamespace()
	       );
	    
	    return ABoxRelated(r, i1, i2);
	  }

	std::ostringstream oss;
	oss << a << " has wrong arity.";
	throw RacerBuildingError(oss.str());
      }

    };

  }
}


bool
NRQLBuilder::createPremise(std::ostream& stream, const Query& query) const
  throw(RacerBuildingError)
{
  bool isEmpty = true;

  if (!query.getPlusC().empty())
    {
      AtomSet::const_iterator backPlusC = --query.getPlusC().end();
      InterToInstance i2i(query, isEmpty);
      std::transform(query.getPlusC().begin(), backPlusC,
		     std::ostream_iterator<ABoxInstance>(stream, " "),
		     i2i
		     );
      stream << i2i(*backPlusC);
    }

  if (!query.getMinusC().empty())
    {
      AtomSet::const_iterator backMinusC = --query.getMinusC().end();
      InterToInstance i2i(query, isEmpty, true);
      std::transform(query.getMinusC().begin(), backMinusC,
		     std::ostream_iterator<ABoxInstance>(stream, " "),
		     i2i
		     );
      stream << i2i(*backMinusC);
    }

  if (!query.getPlusR().empty())
    {
      AtomSet::const_iterator backPlusR = --query.getPlusR().end();
      InterToRelated i2r(query, isEmpty);
      std::transform(query.getPlusR().begin(), backPlusR,
		     std::ostream_iterator<ABoxRelated>(stream, " "),
		     i2r
		     );
      stream << i2r(*backPlusR);
    }

  if (!query.getMinusR().empty())
    {
      AtomSet::const_iterator backMinusR = --query.getMinusR().end();
      InterToInstance i2i(query, isEmpty, true);
      std::transform(query.getMinusR().begin(), backMinusR,
		     std::ostream_iterator<ABoxInstance>(stream, " "),
		     i2i
		     );
      stream << i2i(*backMinusR);
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
		o1 = new ABoxQueryVariable
		  (t1.getVariable(),
		   ABoxQueryVariable::VariableType::noninjective
		   );
	      }
	    else
	      {
		o1 = new ABoxQueryIndividual
		  (t1.getUnquotedString(),
		   query.getOntology()->getNamespace()
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
		   query.getOntology()->getNamespace()
		   );
	      }
	    
	    body.addAtom(new NRQLQueryAtom
			 (new RoleQuery
			  (new ABoxQueryRole
			   (it->getPredicate().getUnquotedString(),
			    query.getOntology()->getNamespace()
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
		   query.getOntology()->getNamespace()
		   );
	      }

	    if (it->isStronglyNegated())
	      {
		body.addAtom(new NRQLQueryAtom
			     (new ConceptQuery
			      (new ABoxNegatedConcept
			       (new ABoxQueryConcept
				(it->getPredicate().getUnquotedString(),
				 query.getOntology()->getNamespace()
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
				query.getOntology()->getNamespace()
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
				       query.getOntology()->getNamespace(),
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
		     (pat[0].getUnquotedString(), query.getOntology()->getNamespace())
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
	 (new ABoxQueryRole(q.getUnquotedString(), query.getOntology()->getNamespace(), true),
	  new ABoxQueryVariable("X", ABoxQueryVariable::VariableType::substrate),
	  new ABoxQueryVariable("Y", ABoxQueryVariable::VariableType::noninjective | ABoxQueryVariable::VariableType::substrate)
	  )
	 );
    }

  return true;
}
