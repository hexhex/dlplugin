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

#include "RacerQuery.h"

#include <raptor.h>

#include <string>


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


  protected:
    /**
     * The statement callback handler for libraptor.
     *
     * @param userData a downcasted Answer object for adding the individuals
     *
     * @param statement libraptor statement contains triples and
     * various other information
     */
    static void
    statementHandler(void* userData, const raptor_statement* statement);

    /**
     * The namespace callback handler for libraptor.
     *
     * @param userData a downcasted Query object for setting the default namespace
     * @param nspace libraptor nspace contains currently parsed namespace
     */
    static void
    namespaceHandler(void* userData, raptor_namespace* nspace);


  public:
    OWLParser();

    /// Ctor
    explicit
    OWLParser(const std::string& uri);

    /// Copy Ctor
    OWLParser(const OWLParser&);

    /// Dtor
    virtual
    ~OWLParser();

    /**
     * set new OWL uri.
     *
     * @param uri the new OWL uri
     */
    virtual void
    open(const std::string& uri);

    /**
     * get universe of OWL document
     *
     * @param answer add individuals to this Answer object
     */
    virtual void
    parseIndividuals(Answer& answer);

    /**
     * get default namespace
     *
     * @param query set namespace of this Query object
     */
    virtual void
    parseNamespace(Query& query);
  };

} // namespace racer
} // namespace dlvhex

#endif /* _OWLPARSER_H */
