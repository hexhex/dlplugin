/* -*- C++ -*- */

/**
 * @file   Ontology.cpp
 * @author Thomas Krennwallner
 * @date   Sat Aug  5 14:00:05 2006
 * 
 * @brief  Basic information about ontologies.
 * 
 * 
 */

#include "Ontology.h"
#include "OWLParser.h"
#include "UserDir.h"

#include <string>
#include <map>
#include <iterator>

using namespace dlvhex::dl;
using dlvhex::util::UserDir;


Ontology::~Ontology()
{
  if (isTemp) // remove downloaded temporary file
    {
      UserDir().remove(uri);
    }
}


Ontology::Ontology(const std::string& u)
  : uri(u),
    realuri(u),
    isTemp(false)
{
  OWLParser p(uri);

  if (!isLocal())
    {
      std::string tmp = UserDir().createTemp("owl-");

      p.fetchURI(tmp);
      uri = tmp;
      p.open(tmp);

      isTemp = true;
    }

  p.parseNamespace(nspace);
}


Ontology::Ontology(const Ontology&)
{ }


bool
Ontology::isLocal() const
{
  return uri.find("http://") == std::string::npos;
}


Ontology::shared_pointer
Ontology::createOntology(const std::string& uri)
{
  ///@todo get rid of the static variable: due to an unforseeable
  ///destruction order during the program exit time, this has a bad
  ///interaction with UserDir (@see Ontology::~Ontology).

  typedef std::map<std::string, Ontology::shared_pointer> OntologyMap;
  static OntologyMap ontomap;

  std::string finduri;

  if (uri.find("file://") != 0 && uri.find("file:") != 0 && uri.find("http://") != 0)
    {
      // must be a pathname
      finduri = "file:" + uri;
    }
  else if (uri.find("file://") == 0) // this needs a special massage
    {
      finduri = "file:" + uri.substr(7);
    }
  else // a valid URI
    {
      finduri = uri;
    }

  OntologyMap::const_iterator o = ontomap.find(finduri);

  if (o != ontomap.end())
    {
      return o->second;
    }

  try
    {
      Ontology::shared_pointer osp(new Ontology(finduri));

      ontomap[finduri] = osp;

      return osp;
    }
  catch (DLParsingError& e)
    {
      throw DLParsingError("Couldn't parse document " + uri + ": " + e.what());
    }
}


const std::string&
Ontology::getURI() const
{
  return uri;
}


const std::string&
Ontology::getRealURI() const
{
  return realuri;
}

    
const std::string&
Ontology::getNamespace() const
{
  return nspace;
}


Ontology::ObjectsPtr
Ontology::getConcepts() const
{
  if (!concepts)
    {
      Ontology::ObjectsPtr c(new Ontology::Objects);
      Ontology::ObjectsPtr r;

      if (roles)
	{
	  r = roles;
	}
      else
	{
	  r = Ontology::ObjectsPtr(new Ontology::Objects);
	}

      try
	{
	  OWLParser p(uri);
	  p.parseNames(*c, *r);
	}
      catch (DLParsingError& e)
	{
	  throw DLParsingError("Couldn't parse document " + uri + ": " + e.what());
	}

      concepts = c;
      roles = r;
    }

  return concepts;
}

    
Ontology::ObjectsPtr
Ontology::getRoles() const
{
  if (!roles)
    {
      Ontology::ObjectsPtr r(new Ontology::Objects);
      Ontology::ObjectsPtr c;

      if (concepts)
	{
	  c = concepts;
	}
      else
	{
	  c = Ontology::ObjectsPtr(new Ontology::Objects);
	}

      try
	{
	  OWLParser p(uri);
	  p.parseNames(*c, *r);
	}
      catch (DLParsingError& e)
	{
	  throw DLParsingError("Couldn't parse document " + uri + ": " + e.what());
	}

      concepts = c;
      roles = r;
    }

  return roles;
}


Ontology::ObjectsPtr
Ontology::getIndividuals() const
{
  if (!individuals)
    {
      Ontology::ObjectsPtr i(new Ontology::Objects);

      try
	{
	  OWLParser p(uri);
	  p.parseIndividuals(*i);
	}
      catch (DLParsingError& e)
	{
	  throw DLParsingError("Couldn't parse document " + uri + ": " + e.what());
	}

      individuals = i;
    }

  return individuals;
}
