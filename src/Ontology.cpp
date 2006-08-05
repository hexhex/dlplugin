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

Ontology::Ontology(const std::string& u,
		   const std::string& n)
  : uri(u),
    nspace(n)
{ }

Ontology::Ontology(const Ontology&)
{ }

Ontology::shared_pointer
Ontology::createOntology(const std::string& uri)
{
  typedef std::map<std::string, Ontology::shared_pointer> OntologyMap;
  static OntologyMap ontomap;

  OntologyMap::const_iterator o = ontomap.find(uri);

  if (o != ontomap.end())
    {
      return o->second;
    }

  OWLParser p(uri);

  std::string nspace;
  p.parseNamespace(nspace);

  Ontology::shared_pointer osp(new Ontology(uri, nspace));

  ontomap[uri] = osp;

  return osp;
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
      catch (RacerParsingError&)
	{
	  ///@todo ignore?
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
      catch (RacerParsingError&)
	{
	  ///@todo ignore?
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
      catch (RacerParsingError&)
	{
	  ///@todo ignore?
	}

      individuals = i;
    }

  return individuals;
}
