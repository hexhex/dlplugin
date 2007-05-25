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

  delete abox;
  delete tbox;
}


Ontology::Ontology(const URI& u, const std::string& tempfile)
  : uri(u),
    realuri(u),
    nspace(),
    tbox(0),
    abox(0)
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
    tbox(o.tbox ? new TBox(*o.tbox) : 0),
    abox(o.abox ? new ABox(*o.abox) : 0)
{ }


Ontology&
Ontology::operator= (const Ontology&)
{
  return *this; // ignore
}


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
	  std::string tmpstr(tmp); // Ontology may throw()
	  std::free(tmp);

	  osp = Ontology::shared_pointer(new Ontology(finduri, tmpstr));
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


const TBox&
Ontology::getTBox() const
{
  if (!tbox)
    {
      try
	{
	  tbox = new TBox;
	  OWLParser p(uri);
	  p.parseTBox(*tbox);
	}
      catch (DLParsingError& e)
	{
	  throw DLParsingError("Couldn't parse document " + uri.getString() + ": " + e.what());
	}
    }

  return *tbox;
}


const ABox&
Ontology::getABox() const
{
  if (!abox)
    {
      try
	{
	  abox = new ABox;
	  OWLParser p(uri);
	  p.parseABox(*abox);
	}
      catch (DLParsingError& e)
	{
	  throw DLParsingError("Couldn't parse document " + uri.getString() + ": " + e.what());
	}
    }

  return *abox;
}


// Local Variables:
// mode: C++
// End:
