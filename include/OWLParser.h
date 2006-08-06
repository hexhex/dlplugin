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

#include "dlvhex/Term.h"
#include "RacerError.h"

#include <raptor.h>
#include <string>
#include <set>


namespace dlvhex {
namespace racer {

  /**
   * @brief Parse individuals and default namespace of an OWL KB.
   */
  class OWLParser
  {
  private:
    /// URI to the OWL KB
    std::string uri;

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

    /**
     * setup parser with userData and handler.
     */
    virtual void
    parse(void *userData,
	  raptor_statement_handler handler,
	  raptor_namespace_handler nsHandler) throw (RacerParsingError);

  protected:
    /**
     * The individual handler for libraptor.
     *
     * @param userData a downcasted Answer object for adding the individuals
     *
     * @param statement libraptor statement contains triples and
     * various other information
     */
    static void
    aboxHandler(void* userData, const raptor_statement* statement);

    /**
     * The concept and role name handler for libraptor.
     */
    static void
    tboxHandler(void* userData, const raptor_statement* statement);

    /**
     * The namespace callback handler for libraptor.
     *
     * @param userData a downcasted Query object for setting the default namespace
     * @param nspace libraptor nspace contains currently parsed namespace
     */
    static void
    namespaceHandler(void* userData, raptor_namespace* nspace);

    static void
    writeBytesHandler(raptor_www* w3, void* userData,
		      const void* ptr, size_t size, size_t nmemb);


    static void
    errorHandler(void* userData, raptor_locator* locator, const char* message);

  public:
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
    parseIndividuals(std::set<Term>& indvs) throw (RacerParsingError);

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
    parseNames(std::set<Term>& concepts, std::set<Term>& roles) throw (RacerParsingError);

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
