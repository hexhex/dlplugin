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
#include "RacerError.h"

#include <set>
#include <string>
#include <sstream>

#include <raptor.h>

using namespace dlvhex::racer;


const std::string OWLParser::owlThing = "http://www.w3.org/2002/07/owl#Thing";
const std::string OWLParser::rdfType  = "http://www.w3.org/1999/02/22-rdf-syntax-ns#type";
const std::string OWLParser::rdfsNspace = "http://www.w3.org/2000/01/rdf-schema";
const std::string OWLParser::rdfNspace = "http://www.w3.org/1999/02/22-rdf-syntax-ns";
const std::string OWLParser::owlNspace = "http://www.w3.org/2002/07/owl";

const std::string OWLParser::rdfsSubPropertyOf = "http://www.w3.org/2000/01/rdf-schema#subPropertyOf";
const std::string OWLParser::owlClass = "http://www.w3.org/2002/07/owl#Class";
const std::string OWLParser::owlSubClassOf = "http://www.w3.org/2000/01/rdf-schema#subClassOf";
const std::string OWLParser::owlObjectProperty = "http://www.w3.org/2002/07/owl#ObjectProperty";
const std::string OWLParser::owlInverseOf = "http://www.w3.org/2002/07/owl#inverseOf";


OWLParser::OWLParser(const std::string& u)
{
  open(u);
}


OWLParser::~OWLParser()
{ }


void
OWLParser::open(const std::string& uri)
{
  std::string scheme = uri.substr(0,7);

  if (scheme == "http://" || scheme == "file://" || scheme.find("file:") == 0)
    {
      this->uri = uri;
    }
  else
    {
      this->uri = "file:" + uri;
    }
}


void
OWLParser::namespaceHandler(void* userData, raptor_namespace* nspace)
{
  std::string* nsData = (std::string*) userData;

  const char* nsPrefix = (const char*) raptor_namespace_get_prefix(nspace);

  if (nsPrefix == 0) // default namespace
    {
      raptor_uri* ns = raptor_namespace_get_uri(nspace);
      nsData->assign((const char*) raptor_uri_as_string(ns));
    }
}

namespace dlvhex {
  namespace racer {

    /// helper struct for parsing concept and role names
    struct TBoxNames
    {
      std::set<Term>* conceptNames;
      std::set<Term>* roleNames;
    };

  } // namespace racer
} // namespace dlvhex

void
OWLParser::tboxHandler(void* userData, const raptor_statement* statement)
{
  const TBoxNames* t = (const TBoxNames*) userData;

  std::string subj = (const char*) statement->subject;
  std::string pred = (const char*) statement->predicate;
  std::string obj  = (const char*) statement->object;

  std::string::size_type pos = obj.find_first_of('#');
  std::string objNspace = obj.substr(0, pos);

  if ((rdfType == pred && owlObjectProperty == obj)
      ||
      (rdfsSubPropertyOf == pred)
      ||
      (owlInverseOf == pred)
      )
    {
      t->roleNames->insert(Term(subj));
    }
  else if (
	   (rdfType == pred && owlClass == obj)
	   ||
	   (owlSubClassOf == pred)
	   )
    {
      t->conceptNames->insert(Term(subj));
    }
}

void
OWLParser::aboxHandler(void* userData, const raptor_statement* statement)
{
  std::set<Term>* indvs = (std::set<Term>*) userData;

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
      indvs->insert(Term(subj, true));
    }
}


void
OWLParser::writeBytesHandler(raptor_www* /* w3 */, void* userData,
			     const void* ptr, size_t size, size_t nmemb)
{
  raptor_iostream_write_bytes((raptor_iostream *) userData, ptr, size, nmemb);
}


void
OWLParser::errorHandler(void* userData, raptor_locator* locator, const char* message)
{
  std::string* err = (std::string*) userData;

  if (err)
    {
      std::ostringstream oss;

      oss << message;

      int line = raptor_locator_line(locator);
      if (line >= 0)
	{
	  oss << " (line " << line << ')';
	}

      *err = oss.str();
    }
}


void
OWLParser::parse(void* userData,
		 raptor_statement_handler handler,
		 raptor_namespace_handler nsHandler)
  throw (RacerParsingError)
{
  std::string error;

  raptor_init();

  raptor_parser* parser = raptor_new_parser("rdfxml");

  raptor_set_fatal_error_handler(parser, &error, OWLParser::errorHandler);
  raptor_set_error_handler(parser, &error, OWLParser::errorHandler);
  raptor_set_warning_handler(parser, 0, OWLParser::errorHandler);

  raptor_uri* parseURI  = raptor_new_uri((const unsigned char*) uri.c_str());

  if (handler)
    {
      raptor_set_statement_handler(parser, userData, handler);
    }

  if (nsHandler)
    {
      raptor_set_namespace_handler(parser, userData, nsHandler);
    }

  raptor_parse_uri(parser, parseURI, 0);
    
  raptor_free_uri(parseURI);
  raptor_free_parser(parser);

  raptor_finish();

  if (!error.empty())
    {
      throw RacerParsingError(error);
    }
}

void
OWLParser::parseIndividuals(std::set<Term>& indvs) throw (RacerParsingError)
{
  parse(&indvs, OWLParser::aboxHandler, 0);
}

void
OWLParser::parseNamespace(std::string& ns) throw (RacerParsingError)
{
  parse(&ns, 0, OWLParser::namespaceHandler);
}

void
OWLParser::parseNames(std::set<Term>& concepts, std::set<Term>& roles)
  throw (RacerParsingError)
{
  TBoxNames names;
  names.conceptNames = &concepts;
  names.roleNames = &roles;

  parse(&names, OWLParser::tboxHandler, 0);
}

void
OWLParser::fetchURI(const std::string& file)
  throw (RacerParsingError)
{
  std::string error;

  raptor_init();
  raptor_www_init();

  raptor_uri* fetchURI = raptor_new_uri((const unsigned char*) uri.c_str());

  raptor_iostream* io = raptor_new_iostream_to_filename(file.c_str());

  raptor_www* rw3 = raptor_www_new();

  raptor_www_set_write_bytes_handler(rw3, OWLParser::writeBytesHandler, io);
  raptor_www_set_error_handler(rw3, OWLParser::errorHandler, &error);

  raptor_www_fetch(rw3, fetchURI);

  raptor_www_free(rw3);
  raptor_free_uri(fetchURI);
  raptor_free_iostream(io);

  raptor_www_finish();
  raptor_finish();
  
  if (!error.empty())
    {
      throw RacerParsingError(error);
    }
}
