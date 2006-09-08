/* -*- C++ -*- */

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

    static const std::string rdfsSubPropertyOf;
    static const std::string owlClass;
    static const std::string owlSubClassOf;
    static const std::string owlObjectProperty;
    static const std::string owlInverseOf;

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
     * @param indvs add individuals to set
     */
    virtual void
    parseIndividuals(Ontology::Objects& indvs) throw (DLParsingError);

    /**
     * get default namespace
     *
     * @param ns set namespace to this string
     */
    virtual void
    parseNamespace(std::string& ns) throw (DLParsingError);

    /**
     * get all possible concept and role names.
     */
    virtual void
    parseNames(Ontology::Objects& concepts, Ontology::Objects& roles) throw (DLParsingError);

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
