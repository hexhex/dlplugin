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

#include <dlvhex/ComfortPluginInterface.hpp>

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
  const DLQuery::shared_pointer& dlq = query.getDLQuery();
  const ComfortTerm& q = dlq->getQuery();
  const ComfortTuple& indv = dlq->getPatternTuple();
  const std::string concept = q.getUnquotedString();
  const std::string& nspace = dlq->getOntology()->getNamespace();

  if (!(dlq->isBoolean() && indv.size() == 1))
    {
      throw DLBuildingError("Incompatible pattern supplied.");
    }

  try
    {
      std::auto_ptr<ABoxConceptDescrExpr> c;

      if (concept[0] != '-')
	{
	  c.reset(new ABoxQueryConcept(q, nspace));
	}
      else
	{
	  ComfortTerm tmp = ComfortTerm::createConstant("\"" + concept.substr(1) + "\"");
	  c.reset(new ABoxNegatedConcept(new ABoxQueryConcept(tmp, nspace)));
	}

      stream << "(individual-instance? "
	     << ABoxQueryIndividual(indv[0], nspace)
	     << ' '
	     << *c
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
  const DLQuery::shared_pointer& dlq = query.getDLQuery();
  const ComfortTerm& q = dlq->getQuery();
  const ComfortTuple& indv = dlq->getPatternTuple();
  const std::string& nspace = dlq->getOntology()->getNamespace();

  if (!(dlq->isBoolean() && indv.size() == 2))
    {
      throw DLBuildingError("Incompatible pattern supplied.");
    }

  try
    {
      ///@todo no negated role support

      stream << "(individuals-related? "
	     << ABoxQueryIndividual(indv[0], nspace)
	     << ' '
	     << ABoxQueryIndividual(indv[1], nspace)
	     << ' '
	     << ABoxQueryRole(q, nspace)
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
  const DLQuery::shared_pointer& dlq = query.getDLQuery();
  const ComfortTerm& q = dlq->getQuery();
  const ComfortTuple& indv = dlq->getPatternTuple();
  const std::string& nspace = dlq->getOntology()->getNamespace();
  unsigned long type = dlq->getTypeFlags() & std::numeric_limits<unsigned long>::max();

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
	  i.reset(new ABoxQueryIndividual(indv[0], nspace));

	  r.reset(new ABoxQueryRole(q, nspace));
	}
      else // (variable,const) pattern
	{
	  i.reset(new ABoxQueryIndividual(indv[1], nspace));

	  // note the inverted role
	  r.reset(new ABoxInvertedRole(new ABoxQueryRole(q, nspace)));
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
  const DLQuery::shared_pointer& dlq = query.getDLQuery();
  const ComfortTerm& q = dlq->getQuery();
  const std::string concept = q.getUnquotedString();
  const std::string& nspace = dlq->getOntology()->getNamespace();

  try
    {
      std::auto_ptr<ABoxConceptDescrExpr> c;

      if (concept[0] != '-')
	{
	  c.reset(new ABoxQueryConcept(q, nspace));
	}
      else
	{
	  ComfortTerm tmp = ComfortTerm::createConstant("\"" + concept.substr(1) + "\"");
	  c.reset(new ABoxNegatedConcept(new ABoxQueryConcept(tmp, nspace)));
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
  const DLQuery::shared_pointer& dlq = query.getDLQuery();
  const ComfortTerm& q = dlq->getQuery();
  const std::string& nspace = dlq->getOntology()->getNamespace();

  try
    {
      ///@todo no negated role?

      stream << "(retrieve-related-individuals "
	     << ABoxQueryRole(q, nspace)
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
  const URI& realuri = query.getDLQuery()->getOntology()->getRealURI();
  const URI& uri = query.getDLQuery()->getOntology()->getURI();

  // we read the owl document uri into kb-name uri

  try
    {
      if (!uri.isLocal()) // a http document
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
	  stream << "(owl-read-file \""
		 << uri.getPath()
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


// Local Variables:
// mode: C++
// End:
