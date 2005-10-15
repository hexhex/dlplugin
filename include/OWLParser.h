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

#include <string>

#include <raptor.h>

#include <dlvhex/PluginInterface.h>

#include "RacerQuery.h"



namespace dlvhex {
namespace racer {

  /**
   * @brief Parse individuals and default namespace of an OWL KB.
   */
  class OWLParser
  {
  private:
    std::string uri;

    static const std::string owlThing;
    static const std::string rdfType;
    static const std::string rdfNspace;
    static const std::string rdfsNspace;
    static const std::string owlNspace;


  protected:
    static void
    statementHandler(void*, const raptor_statement*);

    static void
    namespaceHandler(void*, raptor_namespace*);


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

    virtual void
    open(const std::string& uri);

    /// get universe of OWL document
    virtual void
    parseIndividuals(Answer&);

    /// get default namespace
    virtual void
    parseNamespace(Query&);
  };

} // namespace racer
} // namespace dlvhex

#endif /* _OWLPARSER_H */
