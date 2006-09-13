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
#include "URI.h"

#include <string>
#include <map>
#include <iterator>

#include <cstdio>   // tempnam(), remove()
#include <cstdlib>  // free()

using namespace dlvhex::dl;


Ontology::~Ontology()
{
  if (!realuri.isLocal()) // remove downloaded temporary file
    {
      std::remove(uri.getPath().c_str());
    }
}


Ontology::Ontology(const URI& u, const std::string& tempfile)
  : uri(u),
    realuri(u),
    nspace(),
    concepts(),
    roles(),
    individuals()
{
  OWLParser p(uri);

  if (!realuri.isLocal() && !tempfile.empty())
    {
      p.fetchURI(tempfile); // download uri to tempfile
      uri = tempfile;
      p.open(uri);
    }

  p.parseNamespace(nspace);
}


Ontology::Ontology(const Ontology& o)
  : uri(o.uri),
    realuri(o.realuri),
    nspace(o.nspace),
    concepts(o.concepts),
    roles(o.roles),
    individuals(o.individuals)
{ }


Ontology::shared_pointer
Ontology::createOntology(const std::string& uri)
{
  // we only want one Ontology instance per uri
  typedef std::map<URI, Ontology::shared_pointer> OntologyMap;
  static OntologyMap ontomap;

  URI finduri(uri, true); // we want absolute pathnames

  OntologyMap::const_iterator o = ontomap.find(finduri);

  if (o != ontomap.end())
    {
      return o->second;
    }

  try
    {
      Ontology::shared_pointer osp;

      if (finduri.isLocal())
	{
	  osp = Ontology::shared_pointer(new Ontology(finduri));
	}
      else // a non-local URI, download it to a local file so we can re-use that file
	{
	  // create an absolute temporary filename for http URIs
	  char *tmp = ::tempnam(0, "owl-");

	  osp = Ontology::shared_pointer(new Ontology(finduri, tmp));

	  std::free(tmp);
	}

      // identify finduri with osp
      ontomap[finduri] = osp;

      return osp;
    }
  catch (DLParsingError& e)
    {
      throw DLParsingError("Couldn't parse document " + uri + ": " + e.what());
    }
}


const URI&
Ontology::getURI() const
{
  return uri;
}


const URI&
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
	  throw DLParsingError("Couldn't parse document " + uri.getString() + ": " + e.what());
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
	  throw DLParsingError("Couldn't parse document " + uri.getString() + ": " + e.what());
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
	  throw DLParsingError("Couldn't parse document " + uri.getString() + ": " + e.what());
	}

      individuals = i;
    }

  return individuals;
}
