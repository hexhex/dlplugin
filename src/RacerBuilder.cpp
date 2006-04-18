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

using namespace dlvhex::racer;


RacerBuilder::RacerBuilder(std::ostream& s)
  : stream(s)
{ }

RacerBuilder::~RacerBuilder()
{ }

std::string
RacerBuilder::createName(const Query& query,
			 const std::string& name) const
{
  if (name.find('#') == std::string::npos)
    {
      return "|" + query.getNamespace() + name + "|";
    }
  else
    {
      return "|" + name + "|";
    }
}


RacerStateBuilder::RacerStateBuilder(std::ostream& s)
  : RacerBuilder(s)
{ }

RacerStateBuilder::~RacerStateBuilder()
{ }


void
RacerStateBuilder::buildCommand(Query& query) throw (RacerBuildingError)
{
  const Interpretation& ints = query.getInterpretation();

  //
  // create a state list. If there is no individual or pair to add
  // an empty "(state )" command is not an error.
  //
  // RACER keeps the enlarged ABox if the command results in an
  // inconsistent ABox.
  //

  try
    {
      const std::vector<ABoxAssertion::shared_pointer>* v = query.createPremise();

      stream << "(state " << *v << ")" << std::endl;

      delete v;
    }
  catch (std::exception& e)
    {
      throw RacerBuildingError(e.what());
    }
}




RacerIsConceptMemberBuilder::RacerIsConceptMemberBuilder(std::ostream& s)
  : RacerBuilder(s)
{ }


RacerIsConceptMemberBuilder::~RacerIsConceptMemberBuilder()
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
	     << createName(query, indv[0].getUnquotedString())
	     << " "
	     << createName(query, q.getUnquotedString())
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


RacerIsRoleMemberBuilder::~RacerIsRoleMemberBuilder()
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
	     << createName(query, indv[0].getUnquotedString())
	     << " "
	     << createName(query, indv[1].getUnquotedString())
	     << " "
	     << createName(query, q.getUnquotedString())
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


RacerIndividualFillersBuilder::~RacerIndividualFillersBuilder()
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
      stream << "(individual-fillers ";

      if (query.getType() == Query::RightRetrieval) // (const,variable) pattern
	{
	  stream << createName(query, indv[0].getUnquotedString())
		 << " "
		 << createName(query, q.getUnquotedString());
	}
      else // (variable,const) pattern
	{
	  stream << createName(query, indv[1].getUnquotedString())
		 << " (inv "
		 << createName(query, q.getUnquotedString())
		 << ")";
	}

      stream << ")" << std::endl;
    }
  catch (std::exception& e)
    {
      throw RacerBuildingError(e.what());
    }
}





RacerConceptInstancesBuilder::RacerConceptInstancesBuilder(std::ostream& s)
  : RacerBuilder(s)
{ }

RacerConceptInstancesBuilder::~RacerConceptInstancesBuilder()
{ }

void
RacerConceptInstancesBuilder::buildCommand(Query& query) throw (RacerBuildingError)
{
  const Term& q = query.getQuery();
  std::string concept = q.getUnquotedString();

  try
    {
      if (concept[0] != '-')
	{
	  stream << "(concept-instances "
		 << createName(query, concept)
		 << ")"
		 << std::endl;
	}
      else
	{
	  concept.erase(0, 1); // remove first character "-"
	  
	  stream << "(concept-instances (not "
		 << createName(query, concept)
		 << "))"
		 << std::endl;
	}
    }
  catch (std::exception& e)
    {
      throw RacerBuildingError(e.what());
    }
}




RacerRoleIndividualsBuilder::RacerRoleIndividualsBuilder(std::ostream& s)
  : RacerBuilder(s)
{ }

RacerRoleIndividualsBuilder::~RacerRoleIndividualsBuilder()
{ }

void
RacerRoleIndividualsBuilder::buildCommand(Query& query) throw (RacerBuildingError)
{
  const Term& q = query.getQuery();

  try
    {
      stream << "(retrieve-related-individuals "
	     << createName(query, q.getUnquotedString())
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

RacerOpenOWLBuilder::~RacerOpenOWLBuilder()
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


RacerIndividualDatatypeFillersBuilder::~RacerIndividualDatatypeFillersBuilder()
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
		 << createName(query, q.getUnquotedString())
		 << ")) (same-as ?X "
 		 << createName(query, indv[0].getUnquotedString())
 		 << "))";
	}
      else // (variable,variable) pattern
	{
	  stream << "(?*X $?*Y) (?*X $?*Y (:owl-datatype-role "
		 << createName(query, q.getUnquotedString())
		 << "))";
	}

      stream << ")" << std::endl;
    }
  catch (std::exception& e)
    {
      throw RacerBuildingError(e.what());
    }
}
