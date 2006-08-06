/* -*- C++ -*- */

/**
 * @file   Ontology.cpp
 * @author Thomas Krennwallner
 * @date   Sat Aug  5 14:00:05 2006
 * 
 * @brief  
 * 
 * 
 */

#include "Ontology.h"
#include "OWLParser.h"
#include "UserDir.h"

#include <string>
#include <map>
#include <iterator>

using namespace dlvhex::racer;

namespace dlvhex {
  namespace racer {

    std::ostream&
    operator<< (std::ostream& os, const Ontology& o)
    {
      return os << o.getURI() << '(' << o.getNamespace() << ')';
    }

    bool
    operator< (const Ontology& o1, const Ontology& o2)
    {
      return o1.getURI() < o2.getURI() ? true :
	o1.getNamespace() < o2.getNamespace() ? true : false;
    }

    bool
    operator== (const Ontology& o1, const Ontology& o2)
    {
      return o1.getURI() == o2.getURI() && o1.getNamespace() == o2.getNamespace();
    }

    bool
    operator!= (const Ontology& o1, const Ontology& o2)
    {
      return !(o1 == o2);
    }

  }
}


Ontology::Ontology()
{ }

Ontology::~Ontology()
{
  if (isTemp)
    {
      UserDir().remove(uri);
    }
}


Ontology::Ontology(const std::string& u)
  : uri(u),
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
  typedef std::map<std::string, Ontology::shared_pointer> OntologyMap;
  static OntologyMap ontomap;

  // rip off "file:" prefix

  std::string finduri = uri;

  if (finduri.find("file:") == 0)
    {
      finduri.erase(0, 5);
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
  catch (RacerParsingError& e)
    {
      throw RacerParsingError("Couldn't parse document " + uri + ": " + e.what());
    }
}

const std::string&
Ontology::getURI() const
{
  return uri;
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
      catch (RacerParsingError& e)
	{
	  throw RacerParsingError("Couldn't parse document " + uri + ": " + e.what());
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
      catch (RacerParsingError& e)
	{
	  throw RacerParsingError("Couldn't parse document " + uri + ": " + e.what());
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
      catch (RacerParsingError& e)
	{
	  throw RacerParsingError("Couldn't parse document " + uri + ": " + e.what());
	}

      individuals = i;
    }

  return individuals;
}
