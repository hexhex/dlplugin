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

using namespace dlvex_racer;


RacerBuilder::RacerBuilder(std::ostream& s)
  : stream(s)
{ }

RacerBuilder::~RacerBuilder()
{ }



RacerStateBuilder::RacerStateBuilder(std::ostream& s)
  : RacerBuilder(s)
{ }

RacerStateBuilder::~RacerStateBuilder()
{ }

void
RacerStateBuilder::buildPosInstance(const GAtom& atom,
				    const std::string& nspace)
{
  if (atom.getArity() != 3)
    {
      throw RacerBuildingError("Atom has wrong arity.");
    }

  stream << "(instance |"
	 << nspace
	 << atom.getArgument(2).getUnquotedString()
	 << "| |"
	 << nspace
	 << atom.getArgument(1).getUnquotedString()
	 << "|)";
}

void
RacerStateBuilder::buildNegInstance(const GAtom& atom,
				    const std::string& nspace)
{
  if (atom.getArity() != 3)
    {
      throw RacerBuildingError("Atom has wrong arity.");
    }

  stream << "(instance |"
	 << nspace
	 << atom.getArgument(2).getUnquotedString()
	 << "| (not |"
	 << nspace
	 << atom.getArgument(1).getUnquotedString()
	 << "|))";
}

void
RacerStateBuilder::buildPosRelated(const GAtom& atom,
				   const std::string& nspace)
{
  if (atom.getArity() != 4)
    {
      throw RacerBuildingError("Atom has wrong arity.");
    }

  stream << "(related |"
	 << nspace
	 << atom.getArgument(2).getUnquotedString()
	 << "| |"
	 << nspace
	 << atom.getArgument(3).getUnquotedString()
	 << "| |"
	 << nspace
	 << atom.getArgument(1).getUnquotedString()
	 << "|)";
}

//@todo how does negated roles work?
// void
// RacerStateBuilder::buildNegRelated(const GAtom& atom,
// 				   const std::string& nspace)
// {
//   if (atom.getArity() != 4)
//     {
//       throw RacerBuildingError("Atom has wrong arity.");
//     }

//   stream << "(related |"
// 	 << nspace
// 	 << atom.getArgument(2).getUnquotedString()
// 	 << "| |"
// 	 << nspace
// 	 << atom.getArgument(3).getUnquotedString()
// 	 << "| (not |"
// 	 << nspace
// 	 << atom.getArgument(1).getUnquotedString()
// 	 << "|))";
// }


void
RacerStateBuilder::buildCommand(Query& query) throw (RacerBuildingError)
{
  std::string nspace = query.getNamespace();
  const GAtomSet& pc = query.getPlusConcept();
  const GAtomSet& mc = query.getMinusConcept();
  const GAtomSet& pr = query.getPlusRole();

  if (!pc.empty() || !mc.empty() || !pr.empty())
    {
      stream << "(state ";

      for (GAtomSet::const_iterator it = pc.begin(); it != pc.end(); it++)
	{
	  buildPosInstance(*it, nspace);
	}

      for (GAtomSet::const_iterator it = mc.begin(); it != mc.end(); it++)
	{
	  buildNegInstance(*it, nspace);
	}

      for (GAtomSet::const_iterator it = pr.begin(); it != pr.end(); it++)
	{
	  buildPosRelated(*it, nspace);
	}

      stream << ")" << std::endl;
    }
  else
    {
      ///@todo what should we do when all ints are empty?  stream is
      ///in a wrecked state...
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
  std::string nspace = query.getNamespace();
  const GAtom& atom = query.getQuery();

  if (atom.getArity() != 3)
    {
      throw RacerBuildingError("Atom has wrong arity.");
    }

  stream << "(individual-instance? |"
	 << nspace
	 << atom.getArgument(2).getUnquotedString()
	 << "| |"
	 << nspace
	 << atom.getArgument(1).getUnquotedString()
	 << "|)"
	 << std::endl;
}



RacerIsRoleMemberBuilder::RacerIsRoleMemberBuilder(std::ostream& s)
  : RacerBuilder(s)
{ }


RacerIsRoleMemberBuilder::~RacerIsRoleMemberBuilder()
{ }

void
RacerIsRoleMemberBuilder::buildCommand(Query& query) throw (RacerBuildingError)
{
  std::string nspace = query.getNamespace();
  const GAtom& atom = query.getQuery();

  if (atom.getArity() != 4)
    {
      throw RacerBuildingError("Atom has wrong arity.");
    }

  stream << "(related-individuals? |"
	 << nspace
	 << atom.getArgument(2).getUnquotedString()
	 << "| |"
	 << nspace
	 << atom.getArgument(3).getUnquotedString()
	 << "| |"
	 << nspace
	 << atom.getArgument(1).getUnquotedString()
	 << "|)"
	 << std::endl;
}





RacerConceptInstancesBuilder::RacerConceptInstancesBuilder(std::ostream& s)
  : RacerBuilder(s)
{ }

RacerConceptInstancesBuilder::~RacerConceptInstancesBuilder()
{ }

void
RacerConceptInstancesBuilder::buildCommand(Query& query) throw (RacerBuildingError)
{
  std::string nspace = query.getNamespace();
  const GAtom& atom = query.getQuery();

  if (atom.getArity() != 2)
    {
      throw RacerBuildingError("Atom has wrong arity.");
    }

  std::string concept = atom.getArgument(1).getUnquotedString();

  if (concept[0] != '-')
    {
      stream << "(concept-instances |"
	     << nspace
	     << concept
	     << "|)"
	     << std::endl;
    }
  else
    {
      concept.erase(0, 1); // remove first character "-"

      stream << "(concept-instances (not |"
	     << nspace
	     << concept
	     << "|))"
	     << std::endl;
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
  std::string nspace = query.getNamespace();
  const GAtom& atom = query.getQuery();

  if (atom.getArity() != 2)
    {
      throw RacerBuildingError("Atom has wrong arity.");
    }

  stream << "(retrieve-related-individuals |"
	 << nspace
	 << atom.getArgument(1).getUnquotedString()
	 << "|)"
	 << std::endl;
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
