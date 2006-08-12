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

#include "RacerError.h"
#include "Ontology.h"

#include <string>


namespace dlvhex {
namespace racer {

  /**
   * @brief Parse individuals, concepts names, role names and default
   * namespace of an OWL KB.
   */
  class OWLParser
  {
  private:
    /// URI to the OWL KB
    std::string uri;

    // forward declaration
    class HandlerFuns;


    /** 
     * setup parser with @a userData and @a handler and parses the OWL
     * document found at #uri.
     * 
     * @param userData 
     * @param handler 
     */
    void
    parse(void* userData, const HandlerFuns* handler) throw (RacerParsingError);


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
    OWLParser(const std::string& uri);

    /// Dtor
    virtual
    ~OWLParser();


    /**
     * set new OWL uri.
     *
     * @param uri the new OWL uri
     */
    void
    open(const std::string& uri);

    /**
     * get universe of OWL document
     *
     * @param indvs add individuals to set
     */
    virtual void
    parseIndividuals(Ontology::Objects& indvs) throw (RacerParsingError);

    /**
     * get default namespace
     *
     * @param ns set namespace to this string
     */
    virtual void
    parseNamespace(std::string& ns) throw (RacerParsingError);

    /**
     * get all possible concept and role names.
     */
    virtual void
    parseNames(Ontology::Objects& concepts, Ontology::Objects& roles) throw (RacerParsingError);

    /** 
     * Fetch uri to file.
     * 
     * @param file 
     */
    virtual void
    fetchURI(const std::string& file) throw (RacerParsingError);

  };

} // namespace racer
} // namespace dlvhex

#endif /* _OWLPARSER_H */
