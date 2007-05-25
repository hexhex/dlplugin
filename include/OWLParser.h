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
 * @file   OWLParser.h
 * @author Thomas Krennwallner
 * @date   Fri Sep  2 14:11:28 2005
 * 
 * @brief  Parses OWL documents.
 * 
 * 
 */

#ifndef _OWLPARSER_H
#define _OWLPARSER_H

#include "DLError.h"
#include "Ontology.h"
#include "URI.h"

#include <string>


namespace dlvhex {
namespace dl {

  /**
   * @brief Parse individuals, concepts names, role names and default
   * namespace of an OWL KB.
   */
  class OWLParser
  {
  private:
    /// URI to the OWL KB
    URI uri;

    /// helper struct for parsing concept and role names
    struct HandlerFuns;

    /** 
     * setup parser with @a userData and @a handler and parses the OWL
     * document found at #uri.
     * 
     * @param userData 
     * @param handler 
     */
    void
    parse(void* userData, const HandlerFuns* handler) throw (DLParsingError);


  public:
    /// owl:Thing
    static const std::string owlThing;
    /// rdf:type
    static const std::string rdfType;
    /// rdf namespace
    static const std::string rdfNspace;
    /// rdfs namespace
    static const std::string rdfsNspace;
    /// owl namespace
    static const std::string owlNspace;

    static const std::string owlClass;
    static const std::string owlObjectProperty;
    static const std::string owlDatatypeProperty;
    static const std::string owlTransitiveProperty;
    static const std::string owlSymmetricProperty;
    static const std::string owlFunctionalProperty;
    static const std::string owlInverseFunctionalProperty;

    /// Ctor
    explicit
    OWLParser(const URI& uri);

    /// Dtor
    virtual
    ~OWLParser();


    /**
     * set new OWL uri.
     *
     * @param uri the new OWL uri
     */
    void
    open(const URI& uri);

    /**
     * get universe of OWL document
     *
     * @param abox add individuals to @a abox
     */
    virtual void
    parseABox(ABox& abox) throw (DLParsingError);

    /**
     * get all possible concept and role names.
     */
    virtual void
    parseTBox(TBox& tbox) throw (DLParsingError);

    /**
     * get default namespace
     *
     * @param ns set namespace to this string
     */
    virtual void
    parseNamespace(std::string& ns) throw (DLParsingError);

    /** 
     * Fetch uri to file.
     * 
     * @param file 
     */
    virtual void
    fetchURI(const std::string& file) throw (DLParsingError);

  };

} // namespace dl
} // namespace dlvhex

#endif /* _OWLPARSER_H */


// Local Variables:
// mode: C++
// End:
