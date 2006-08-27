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

#include <string>
#include <map>
#include <iterator>

#include <unistd.h> // unlink()
#include <cstdio>   // tempnam()
#include <stdlib.h> // free()

using namespace dlvhex::dl;


Ontology::~Ontology()
{
  if (!isLocal()) // remove downloaded temporary file
    {
      ::unlink(uri.c_str());
    }
}


Ontology::Ontology(const std::string& u, const std::string& tempuri)
  : uri(u),
    realuri(u)
{
  OWLParser p(uri);

  if (!isLocal())
    {
      p.fetchURI(tempuri); // download tempuri
      uri = tempuri;
      p.open(tempuri);
    }

  p.parseNamespace(nspace);
}


Ontology::Ontology(const Ontology&)
{ }


bool
Ontology::isLocal() const
{
  return realuri.find("http://") == std::string::npos;
}


Ontology::shared_pointer
Ontology::createOntology(const std::string& uri)
{
  typedef std::map<std::string, Ontology::shared_pointer> OntologyMap;
  static OntologyMap ontomap;

  std::string finduri;
  std::string tempuri;

  if (uri.find("file://") != 0 && uri.find("file:") != 0 && uri.find("http://") != 0)
    {
      // must be a pathname
      finduri = "file:" + uri;
      tempuri = finduri;
    }
  else if (uri.find("file://") == 0) // this needs a special massage
    {
      finduri = "file:" + uri.substr(7);
      tempuri = finduri;
    }
  else if (uri.find("file:") == 0) // use uri as is
    {
      finduri = uri;
      tempuri = uri;
    }
  else // a non-local URI, download it to a local file so we can re-use that file
    {
      finduri = uri;

      // create a temporary file for http URIs

      char *tmp = ::tempnam(0, "owl-");
      tempuri = tmp;

      ::free(tmp);
    }

  OntologyMap::const_iterator o = ontomap.find(finduri);

  if (o != ontomap.end())
    {
      return o->second;
    }

  try
    {
      Ontology::shared_pointer osp(new Ontology(finduri, tempuri));

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
