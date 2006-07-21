/* -*- C++ -*- */

/**
 * @file   RacerBuilder.cpp
 * @author Thomas Krennwallner
 * @date   Tue Jul 12 11:51:43 2005
 * 
 * @brief  Various builders for RACER commands.
 * 
 * 
 */

#include "RacerBuilder.h"

#include "RacerQuery.h"

#include <dlvhex/Atom.h>
#include <dlvhex/Term.h>

#include <sstream>
#include <iostream>
#include <string>
#include <memory>

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

using namespace dlvhex::racer;


RacerBuilder::RacerBuilder(std::ostream& s)
  : stream(s)
{ }

RacerBuilder::~RacerBuilder()
{ }


RacerStateBuilder::RacerStateBuilder(std::ostream& s)
  : RacerBuilder(s)
{ }

bool
RacerStateBuilder::buildCommand(Query& query) throw (RacerBuildingError)
{
  //
  // create a state list. If there is no individual or pair to add
  // an empty "(state )" command is not an error.
  //
  // RACER keeps the enlarged ABox if the command results in an
  // inconsistent ABox.
  //

  try
    {
      boost::shared_ptr<const boost::ptr_vector<ABoxAssertion> > v
	(query.createPremise());

      if (v->empty())
	{
	  return false; // nothing to sent, ignore this command
	}

      stream << "(state " << *v << ')' << std::endl;
    }
  catch (std::exception& e)
    {
      throw RacerBuildingError(e.what());
    }

  return true;
}




RacerIsConceptMemberBuilder::RacerIsConceptMemberBuilder(std::ostream& s)
  : RacerBuilder(s)
{ }

bool
RacerIsConceptMemberBuilder::buildCommand(Query& query) throw (RacerBuildingError)
{
  const Term& q = query.getQuery();
  const Tuple& indv = query.getPatternTuple();

  if (!(query.isBoolean() && indv.size() == 1))
    {
      throw RacerBuildingError("Incompatible pattern supplied.");
    }

  try
    {
      stream << "(individual-instance? "
	     << ABoxQueryIndividual(indv[0].getUnquotedString(),
				    query.getNamespace()
				    )
	     << ' '
	     << ABoxQueryConcept(q.getUnquotedString(),
				 query.getNamespace()
				 )
	     << ')'
	     << std::endl;
    }
  catch (std::exception& e)
    {
      throw RacerBuildingError(e.what());
    }

  return true;
}



RacerIsRoleMemberBuilder::RacerIsRoleMemberBuilder(std::ostream& s)
  : RacerBuilder(s)
{ }

bool
RacerIsRoleMemberBuilder::buildCommand(Query& query) throw (RacerBuildingError)
{
  const Term& q = query.getQuery();
  const Tuple& indv = query.getPatternTuple();

  if (!(query.isBoolean() && indv.size() == 2))
    {
      throw RacerBuildingError("Incompatible pattern supplied.");
    }

  try
    {
      stream << "(individuals-related? "
	     << ABoxQueryIndividual(indv[0].getUnquotedString(),
				    query.getNamespace()
				    )
	     << ' '
	     << ABoxQueryIndividual(indv[1].getUnquotedString(),
				    query.getNamespace()
				    )
	     << ' '
	     << ABoxQueryRole(q.getUnquotedString(),
			      query.getNamespace()
			      )
	     << ')'
	     << std::endl;
    }
  catch (std::exception& e)
    {
      throw RacerBuildingError(e.what());
    }

  return true;
}



RacerIndividualFillersBuilder::RacerIndividualFillersBuilder(std::ostream& s)
  : RacerBuilder(s)
{ }

bool
RacerIndividualFillersBuilder::buildCommand(Query& query)
  throw (RacerBuildingError)
{
  const Term& q = query.getQuery();
  const Tuple& indv = query.getPatternTuple();

  unsigned long type = query.getTypeFlags() & std::numeric_limits<unsigned long>::max();

  if (!(type == 0x1 || type == 0x2) || indv.size() != 2)
    {
      throw RacerBuildingError("Incompatible pattern supplied.");
    }

  try
    {
      std::auto_ptr<ABoxQueryIndividual> i;
      std::auto_ptr<ABoxRoleDescrExpr> r;

      // for a (const,variable) pattern only the first bit is allowed to be true
      if (type == 0x1)
	{
	  i.reset(new ABoxQueryIndividual(indv[0].getUnquotedString(),
					  query.getNamespace()
					  )
		  );

	  r.reset(new ABoxQueryRole(q.getUnquotedString(),
				    query.getNamespace()
				    )
		  );
	}
      else // (variable,const) pattern
	{
	  i.reset(new ABoxQueryIndividual(indv[1].getUnquotedString(),
					  query.getNamespace()
					  )
		  );


	  r.reset(new ABoxInvertedRole
		  (new ABoxQueryRole(q.getUnquotedString(),
				     query.getNamespace()
				     )
		   )
		  );
	}

      stream << "(individual-fillers " << *i << ' ' << *r << ')' << std::endl;
    }
  catch (std::exception& e)
    {
      throw RacerBuildingError(e.what());
    }

  return true;
}





RacerConceptInstancesBuilder::RacerConceptInstancesBuilder(std::ostream& s)
  : RacerBuilder(s)
{ }

bool
RacerConceptInstancesBuilder::buildCommand(Query& query) throw (RacerBuildingError)
{
  const Term& q = query.getQuery();
  std::string concept = q.getUnquotedString();

  try
    {
      std::auto_ptr<ABoxConceptDescrExpr> c;

      if (concept[0] != '-')
	{
	  c.reset(new ABoxQueryConcept(concept, query.getNamespace()));
	}
      else
	{
	  concept.erase(0, 1); // remove first character "-"

	  c.reset(new ABoxNegatedConcept
		  (new ABoxQueryConcept(concept, query.getNamespace())
		   )
		  );
	}

      stream << "(concept-instances " << *c << ')' << std::endl;
    }
  catch (std::exception& e)
    {
      throw RacerBuildingError(e.what());
    }

  return true;
}




RacerRoleIndividualsBuilder::RacerRoleIndividualsBuilder(std::ostream& s)
  : RacerBuilder(s)
{ }

bool
RacerRoleIndividualsBuilder::buildCommand(Query& query) throw (RacerBuildingError)
{
  const Term& q = query.getQuery();

  try
    {
      stream << "(retrieve-related-individuals "
	     << ABoxQueryRole(q.getUnquotedString(),
			      query.getNamespace()
			      )
	     << ')'
	     << std::endl;
    }
  catch (std::exception& e)
    {
      throw RacerBuildingError(e.what());
    }

  return true;
}


RacerOpenOWLBuilder::RacerOpenOWLBuilder(std::ostream& s)
  : RacerBuilder(s)
{ }

bool
RacerOpenOWLBuilder::buildCommand(Query& query) throw (RacerBuildingError)
{
  std::string doc = query.getOntology();

  // first 7 chars contains the URL scheme
  std::string scheme = doc.substr(0,7);

  try
    {
      if (scheme == "http://" || scheme == "file://") ///@todo file:// is foobar
	{
	  stream << "(owl-read-document \""
		 << doc
		 << "\" :kb-name DEFAULT)"
		 << std::endl;
	}
      else
	{
	  stream << "(owl-read-file \""
		 << doc
		 << "\" :kb-name DEFAULT)"
		 << std::endl;
	}
    }
  catch (std::exception& e)
    {
      throw RacerBuildingError(e.what());
    }

  return true;
}



RacerIndividualDatatypeFillersBuilder::RacerIndividualDatatypeFillersBuilder(std::ostream& s)
  : RacerBuilder(s)
{ }

bool
RacerIndividualDatatypeFillersBuilder::buildCommand(Query& query)
  throw (RacerBuildingError)
{
  const Term& q = query.getQuery();
  const Tuple& indv = query.getPatternTuple();

  unsigned long type = query.getTypeFlags() & std::numeric_limits<unsigned long>::max();

  if (!(type == 0x0 || type == 0x1) || indv.size() != 2)
    {
      throw RacerBuildingError("Incompatible pattern supplied.");
    }

  try
    {
      NRQLRetrieve retrieve;

      if (type == 0x1) // (const,variable) pattern
	{
	  NRQLConjunction::shared_pointer body(new NRQLConjunction);
	  
	  body->addAtom(new NRQLQueryAtom
			(new RoleQuery
			 (new ABoxQueryRole(q.getUnquotedString(), query.getNamespace(), true),
			  new ABoxQueryVariable("X", ABoxQueryVariable::VariableType::substrate),
			  new ABoxQueryVariable("Y", ABoxQueryVariable::VariableType::noninjective | ABoxQueryVariable::VariableType::substrate)
			  )
			 )
			);
	  body->addAtom(new NRQLQueryAtom
			(new SameAsQuery
			 (new ABoxQueryVariable("X"),
			  new ABoxQueryIndividual
			  (indv[0].getUnquotedString(), query.getNamespace())
			  )
			 )
			);
	  
	  // only retrieve the datatype, let Answer add the
	  // corresponding individual
	  retrieve.addHead(new ABoxQueryVariable("Y", ABoxQueryVariable::VariableType::noninjective | ABoxQueryVariable::VariableType::substrate));

	  retrieve.setBody(body);
	}
      else // (variable,variable) pattern
	{
	  NRQLQueryAtom::shared_pointer body
	    (new NRQLQueryAtom
	     (new RoleQuery
	      (new ABoxQueryRole(q.getUnquotedString(), query.getNamespace(), true),
	       new ABoxQueryVariable("X", ABoxQueryVariable::VariableType::substrate),
	       new ABoxQueryVariable("Y", ABoxQueryVariable::VariableType::noninjective | ABoxQueryVariable::VariableType::substrate)
	       )
	      )
	     );
	  
	  retrieve.addHead(new ABoxQueryVariable("X", ABoxQueryVariable::VariableType::substrate));
	  retrieve.addHead(new ABoxQueryVariable("Y", ABoxQueryVariable::VariableType::noninjective | ABoxQueryVariable::VariableType::substrate));

	  retrieve.setBody(body);
	}

      stream << retrieve << std::endl;
    }
  catch (std::exception& e)
    {
      throw RacerBuildingError(e.what());
    }

  return true;
}


RacerNRQLBuilder::RacerNRQLBuilder(std::ostream& s)
: RacerBuilder(s)
{ }


bool
RacerNRQLBuilder::buildCommand(Query& query) throw (RacerBuildingError)
{
  try
    {
      NRQLRetrieve retrieve;

      retrieve.setBody(query.createBody());
      
      retrieve.setHead(query.createHead());
  
      stream << retrieve << std::endl;
    }
  catch (std::exception& e)
    {
      throw RacerBuildingError(e.what());
    }

  return true;
}
