/* -*- C++ -*- */


/**
 * @file   OWLParser.cpp
 * @author Thomas Krennwallner
 * @date   Fri Sep  2 14:19:05 2005
 * 
 * @brief  Parses OWL documents.
 * 
 * 
 */

#include "OWLParser.h"

#include <string>

#include <raptor.h>

using namespace dlvhex::racer;


const std::string OWLParser::owlThing = "http://www.w3.org/2002/07/owl#Thing";
const std::string OWLParser::rdfType  = "http://www.w3.org/1999/02/22-rdf-syntax-ns#type";
const std::string OWLParser::rdfsNspace = "http://www.w3.org/2000/01/rdf-schema";
const std::string OWLParser::rdfNspace = "http://www.w3.org/1999/02/22-rdf-syntax-ns";
const std::string OWLParser::owlNspace = "http://www.w3.org/2002/07/owl";



OWLParser::OWLParser()
{ }

OWLParser::OWLParser(const std::string& u)
  : uri(u)
{ }

OWLParser::OWLParser(const OWLParser& x)
  : uri(x.uri)
{ }

OWLParser::~OWLParser()
{ }

void
OWLParser::open(const std::string& uri)
{
  this->uri = uri;
}

void
OWLParser::namespaceHandler(void* userData, raptor_namespace* nspace)
{
  Query* query = (Query*) userData;

  const char* nsPrefix = (const char*) raptor_namespace_get_prefix(nspace);

  if (nsPrefix == 0) // default namespace
    {
      raptor_uri* ns = raptor_namespace_get_uri(nspace);
      std::string nsUri((const char*) raptor_uri_as_string(ns));
      query->setNamespace(nsUri);
    }
}

void
OWLParser::statementHandler(void* userData, const raptor_statement* statement)
{
  Answer* answer = (Answer*) userData;

  std::string subj = (const char*) statement->subject;
  std::string pred = (const char*) statement->predicate;
  std::string obj  = (const char*) statement->object;

  std::string::size_type pos = obj.find_first_of('#');
  std::string objNspace = obj.substr(0, pos);

  //???
  // A triple with an individual S has one of following forms
  //  - <S> <rdf:type> <owl:Thing>
  //  - <S> <rdf:type> <O>
  // where namespace of O must not equal to rdf, rdfs or owl.
  //
  ///@todo what about individual equality?
  if ((rdfType == pred && owlThing == obj)
      ||
      (rdfType == pred &&
       rdfNspace  != objNspace &&
       rdfsNspace != objNspace &&
       owlNspace  != objNspace
       )
      )
    {
      Tuple t;
      t.push_back(Term(subj, true));
      answer->addTuple(t);
    }
}

void
OWLParser::parseIndividuals(Answer& answer)
{
  raptor_init();

  raptor_parser* parser = raptor_new_parser("rdfxml");
  raptor_uri* parseURI  = raptor_new_uri((const unsigned char*) uri.c_str());

  raptor_set_statement_handler(parser, &answer, OWLParser::statementHandler);

  raptor_parse_file(parser, parseURI, 0);
    
  raptor_free_uri(parseURI);
  raptor_free_parser(parser);

  raptor_finish();
}

void
OWLParser::parseNamespace(Query& query)
{
  raptor_init();

  raptor_parser* parser = raptor_new_parser("rdfxml");
  raptor_uri* parseURI  = raptor_new_uri((const unsigned char*) uri.c_str());

  raptor_set_namespace_handler(parser, &query, OWLParser::namespaceHandler);

  raptor_parse_file(parser, parseURI, 0);
    
  raptor_free_uri(parseURI);
  raptor_free_parser(parser);

  raptor_finish();
}
