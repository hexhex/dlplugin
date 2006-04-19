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
#include <functional>
#include <memory>

using namespace dlvhex::racer;


RacerBuilder::RacerBuilder(std::ostream& s)
  : stream(s)
{ }

RacerBuilder::~RacerBuilder()
{ }


RacerStateBuilder::RacerStateBuilder(std::ostream& s)
  : RacerBuilder(s)
{ }

void
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
      std::auto_ptr<const std::vector<ABoxAssertion::shared_pointer> > v
	(query.createPremise());

      stream << "(state " << *v << ")" << std::endl;
    }
  catch (std::exception& e)
    {
      throw RacerBuildingError(e.what());
    }
}




RacerIsConceptMemberBuilder::RacerIsConceptMemberBuilder(std::ostream& s)
  : RacerBuilder(s)
{ }

void
RacerIsConceptMemberBuilder::buildCommand(Query& query) throw (RacerBuildingError)
{
  const Term& q = query.getQuery();
  const Tuple& indv = query.getPatternTuple();

  if (query.getType() != Query::Boolean)
    {
      throw RacerBuildingError("Incompatible pattern supplied.");
    }

  try
    {
      stream << "(individual-instance? "
	     << ABoxQueryIndividual(indv[0].getUnquotedString(),
				    query.getNamespace()
				    )
	     << " "
	     << ABoxQueryConcept(q.getUnquotedString(),
				 query.getNamespace()
				 )
	     << ")"
	     << std::endl;
    }
  catch (std::exception& e)
    {
      throw RacerBuildingError(e.what());
    }
}



RacerIsRoleMemberBuilder::RacerIsRoleMemberBuilder(std::ostream& s)
  : RacerBuilder(s)
{ }

void
RacerIsRoleMemberBuilder::buildCommand(Query& query) throw (RacerBuildingError)
{
  const Term& q = query.getQuery();
  const Tuple& indv = query.getPatternTuple();

  if (query.getType() != Query::RelatedBoolean)
    {
      throw RacerBuildingError("Incompatible pattern supplied.");
    }

  try
    {
      stream << "(individuals-related? "
	     << ABoxQueryIndividual(indv[0].getUnquotedString(),
				    query.getNamespace()
				    )
	     << " "
	     << ABoxQueryIndividual(indv[1].getUnquotedString(),
				    query.getNamespace()
				    )
	     << " "
	     << ABoxQueryRole(q.getUnquotedString(),
			      query.getNamespace()
			      )
	     << ")"
	     << std::endl;
    }
  catch (std::exception& e)
    {
      throw RacerBuildingError(e.what());
    }
}



RacerIndividualFillersBuilder::RacerIndividualFillersBuilder(std::ostream& s)
  : RacerBuilder(s)
{ }

void
RacerIndividualFillersBuilder::buildCommand(Query& query)
  throw (RacerBuildingError)
{
  const Term& q = query.getQuery();
  const Tuple& indv = query.getPatternTuple();

  if (query.getType() != Query::LeftRetrieval
      && query.getType() != Query::RightRetrieval)
    {
      throw RacerBuildingError("Incompatible pattern supplied.");
    }

  try
    {
      std::auto_ptr<ABoxQueryIndividual> i;
      std::auto_ptr<ABoxRoleDescrExpr> r;

      if (query.getType() == Query::RightRetrieval) // (const,variable) pattern
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

      stream << "(individual-fillers " << *i << " " << *r << ")" << std::endl;
    }
  catch (std::exception& e)
    {
      throw RacerBuildingError(e.what());
    }
}





RacerConceptInstancesBuilder::RacerConceptInstancesBuilder(std::ostream& s)
  : RacerBuilder(s)
{ }

void
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

      stream << "(concept-instances " << *c << ")" << std::endl;
    }
  catch (std::exception& e)
    {
      throw RacerBuildingError(e.what());
    }
}




RacerRoleIndividualsBuilder::RacerRoleIndividualsBuilder(std::ostream& s)
  : RacerBuilder(s)
{ }

void
RacerRoleIndividualsBuilder::buildCommand(Query& query) throw (RacerBuildingError)
{
  const Term& q = query.getQuery();

  try
    {
      stream << "(retrieve-related-individuals "
	     << ABoxQueryRole(q.getUnquotedString(),
			      query.getNamespace()
			      )
	     << ")"
	     << std::endl;
    }
  catch (std::exception& e)
    {
      throw RacerBuildingError(e.what());
    }
}


RacerOpenOWLBuilder::RacerOpenOWLBuilder(std::ostream& s)
  : RacerBuilder(s)
{ }

void
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
}



RacerIndividualDatatypeFillersBuilder::RacerIndividualDatatypeFillersBuilder(std::ostream& s)
  : RacerBuilder(s)
{ }

void
RacerIndividualDatatypeFillersBuilder::buildCommand(Query& query)
  throw (RacerBuildingError)
{
  const Term& q = query.getQuery();
  const Tuple& indv = query.getPatternTuple();

  if (query.getType() != Query::RelatedRetrieval
      && query.getType() != Query::RightRetrieval)
    {
      throw RacerBuildingError("Incompatible pattern supplied.");
    }

  try
    {
      stream << "(retrieve ";

      if (query.getType() == Query::RightRetrieval) // (const,variable) pattern
	{
	  // only retrieve the datatype, let Answer add the
	  // corresponding individual
	  stream << "($?*Y) (and (?*X $?*Y (:owl-datatype-role "
		 << ABoxQueryRole(q.getUnquotedString(),
				  query.getNamespace()
				  )
		 << ")) (same-as ?X "
 		 << ABoxQueryIndividual(indv[0].getUnquotedString(),
					query.getNamespace()
					)
 		 << "))";
	}
      else // (variable,variable) pattern
	{
	  stream << "(?*X $?*Y) (?*X $?*Y (:owl-datatype-role "
		 << ABoxQueryRole(q.getUnquotedString(),
				  query.getNamespace()
				  )
		 << "))";
	}

      stream << ")" << std::endl;
    }
  catch (std::exception& e)
    {
      throw RacerBuildingError(e.what());
    }
}
