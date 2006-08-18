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

#include "RacerNRQL.h"
#include "RacerNRQLBuilder.h"
#include "Query.h"

#include <dlvhex/Atom.h>
#include <dlvhex/Term.h>

#include <sstream>
#include <iostream>
#include <string>
#include <memory>

using namespace dlvhex::dl::racer;


RacerStateBuilder::RacerStateBuilder(std::ostream& s)
  : QueryBaseBuilder(s)
{ }

bool
RacerStateBuilder::buildCommand(Query& query) throw (DLBuildingError)
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
      std::ostringstream oss;

      if (!NRQLStateBuilder().createPremise(oss, query))
	{
	  return false; // nothing to sent, ignore this command
	}

      stream << "(state " << oss.str() << ')' << std::endl;
    }
  catch (std::exception& e)
    {
      throw DLBuildingError(e.what());
    }

  return true;
}




RacerIsConceptMemberBuilder::RacerIsConceptMemberBuilder(std::ostream& s)
  : QueryBaseBuilder(s)
{ }

bool
RacerIsConceptMemberBuilder::buildCommand(Query& query) throw (DLBuildingError)
{
  const DLQuery dlq = query.getDLQuery();
  const Term& q = dlq.getQuery();
  const Tuple& indv = dlq.getPatternTuple();

  if (!(dlq.isBoolean() && indv.size() == 1))
    {
      throw DLBuildingError("Incompatible pattern supplied.");
    }

  try
    {
      stream << "(individual-instance? "
	     << ABoxQueryIndividual(indv[0].getUnquotedString(),
				    query.getOntology()->getNamespace()
				    )
	     << ' '
	     << ABoxQueryConcept(q.getUnquotedString(),
				 query.getOntology()->getNamespace()
				 )
	     << ' '
	     << query.getKBManager().getKBName()
	     << ')'
	     << std::endl;
    }
  catch (std::exception& e)
    {
      throw DLBuildingError(e.what());
    }

  return true;
}



RacerIsRoleMemberBuilder::RacerIsRoleMemberBuilder(std::ostream& s)
  : QueryBaseBuilder(s)
{ }

bool
RacerIsRoleMemberBuilder::buildCommand(Query& query) throw (DLBuildingError)
{
  const DLQuery dlq = query.getDLQuery();
  const Term& q = dlq.getQuery();
  const Tuple& indv = dlq.getPatternTuple();

  if (!(dlq.isBoolean() && indv.size() == 2))
    {
      throw DLBuildingError("Incompatible pattern supplied.");
    }

  try
    {
      stream << "(individuals-related? "
	     << ABoxQueryIndividual(indv[0].getUnquotedString(),
				    query.getOntology()->getNamespace()
				    )
	     << ' '
	     << ABoxQueryIndividual(indv[1].getUnquotedString(),
				    query.getOntology()->getNamespace()
				    )
	     << ' '
	     << ABoxQueryRole(q.getUnquotedString(),
			      query.getOntology()->getNamespace()
			      )
	     << ' '
	     << query.getKBManager().getKBName()
	     << ')'
	     << std::endl;
    }
  catch (std::exception& e)
    {
      throw DLBuildingError(e.what());
    }

  return true;
}



RacerIndividualFillersBuilder::RacerIndividualFillersBuilder(std::ostream& s)
  : QueryBaseBuilder(s)
{ }

bool
RacerIndividualFillersBuilder::buildCommand(Query& query)
  throw (DLBuildingError)
{
  const DLQuery dlq = query.getDLQuery();
  const Term& q = dlq.getQuery();
  const Tuple& indv = dlq.getPatternTuple();

  unsigned long type = dlq.getTypeFlags() & std::numeric_limits<unsigned long>::max();

  if (!(type == 0x1 || type == 0x2) || indv.size() != 2)
    {
      throw DLBuildingError("Incompatible pattern supplied.");
    }

  try
    {
      std::auto_ptr<ABoxQueryIndividual> i;
      std::auto_ptr<ABoxRoleDescrExpr> r;

      // for a (const,variable) pattern only the first bit is allowed to be true
      if (type == 0x1)
	{
	  i.reset(new ABoxQueryIndividual(indv[0].getUnquotedString(),
					  query.getOntology()->getNamespace()
					  )
		  );

	  r.reset(new ABoxQueryRole(q.getUnquotedString(),
				    query.getOntology()->getNamespace()
				    )
		  );
	}
      else // (variable,const) pattern
	{
	  i.reset(new ABoxQueryIndividual(indv[1].getUnquotedString(),
					  query.getOntology()->getNamespace()
					  )
		  );


	  r.reset(new ABoxInvertedRole
		  (new ABoxQueryRole(q.getUnquotedString(),
				     query.getOntology()->getNamespace()
				     )
		   )
		  );
	}

      stream << "(individual-fillers "
	     << *i
	     << ' '
	     << *r 
	     << ' '
	     << query.getKBManager().getKBName()
	     << ')' << std::endl;
    }
  catch (std::exception& e)
    {
      throw DLBuildingError(e.what());
    }

  return true;
}





RacerConceptInstancesBuilder::RacerConceptInstancesBuilder(std::ostream& s)
  : QueryBaseBuilder(s)
{ }

bool
RacerConceptInstancesBuilder::buildCommand(Query& query) throw (DLBuildingError)
{
  const DLQuery dlq = query.getDLQuery();
  const Term& q = dlq.getQuery();
  std::string concept = q.getUnquotedString();

  try
    {
      std::auto_ptr<ABoxConceptDescrExpr> c;

      if (concept[0] != '-')
	{
	  c.reset(new ABoxQueryConcept(concept, query.getOntology()->getNamespace()));
	}
      else
	{
	  concept.erase(0, 1); // remove first character "-"

	  c.reset(new ABoxNegatedConcept
		  (new ABoxQueryConcept(concept, query.getOntology()->getNamespace())
		   )
		  );
	}

      stream << "(concept-instances "
	     << *c
	     << ' '
	     << query.getKBManager().getKBName()
	     << ')' << std::endl;
    }
  catch (std::exception& e)
    {
      throw DLBuildingError(e.what());
    }

  return true;
}




RacerRoleIndividualsBuilder::RacerRoleIndividualsBuilder(std::ostream& s)
  : QueryBaseBuilder(s)
{ }

bool
RacerRoleIndividualsBuilder::buildCommand(Query& query) throw (DLBuildingError)
{
  const DLQuery dlq = query.getDLQuery();
  const Term& q = dlq.getQuery();

  try
    {
      stream << "(retrieve-related-individuals "
	     << ABoxQueryRole(q.getUnquotedString(),
			      query.getOntology()->getNamespace()
			      )
	     << ' '
	     << query.getKBManager().getKBName()
	     << ')'
	     << std::endl;
    }
  catch (std::exception& e)
    {
      throw DLBuildingError(e.what());
    }

  return true;
}


RacerOpenOWLBuilder::RacerOpenOWLBuilder(std::ostream& s)
  : QueryBaseBuilder(s)
{ }

bool
RacerOpenOWLBuilder::buildCommand(Query& query) throw (DLBuildingError)
{
  const std::string& realuri = query.getOntology()->getRealURI();
  const std::string& uri = query.getOntology()->getURI();

  // we read the owl document uri into kb-name uri

  try
    {
      if (uri.find("http://") == 0) // a http document
	{
	  stream << "(owl-read-document \""
		 << uri
		 << "\" :kb-name |"
		 << realuri
		 << "|)"
		 << std::endl;
	}
      else // a file document
	{
	  std::string tmpuri;

	  if (uri.find("file:") == 0) // get rid of file prefix
	    {
	      tmpuri = uri.substr(5);

	      if (tmpuri.find("//") == 0)
		{
		  tmpuri.erase(0, 2);
		}
	    }
	  else
	    {
	      tmpuri = uri;
	    }

	  stream << "(owl-read-file \""
		 << tmpuri
		 << "\" :kb-name |"
		 << realuri
		 << "|)"
		 << std::endl;
	}
    }
  catch (std::exception& e)
    {
      throw DLBuildingError(e.what());
    }

  return true;
}
