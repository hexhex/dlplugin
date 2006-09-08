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
#include "URI.h"
#include "DLError.h"
#include "Ontology.h"

#include <string>
#include <sstream>

#include <raptor.h>

using namespace dlvhex::dl;


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


OWLParser::OWLParser(const URI& u)
  : uri(u)
{ }

OWLParser::~OWLParser()
{ }


void
OWLParser::open(const URI& uri)
{
  this->uri = uri;
}




namespace dlvhex {
  namespace dl {


    /**
     * The namespace callback handler for libraptor.
     *
     * @param userData a downcasted Query object for setting the default namespace
     * @param nspace libraptor nspace contains currently parsed namespace
     */
    void
    namespaceHandler(void* userData, raptor_namespace* nspace)
    {
      std::string* nsData = (std::string*) userData;
      
      const char* nsPrefix = (const char*) raptor_namespace_get_prefix(nspace);
      
      if (nsPrefix == 0) // default namespace
	{
	  raptor_uri* ns = raptor_namespace_get_uri(nspace);
	  nsData->assign((const char*) raptor_uri_as_string(ns));
	}
    }


    /// helper struct for parsing concept and role names
    struct TBoxNames
    {
      Ontology::Objects* conceptNames;
      Ontology::Objects* roleNames;
    };


    /**
     * The concept and role name handler for libraptor.
     */
    void
    tboxHandler(void* userData, const raptor_statement* statement)
    {
      const TBoxNames* t = (const TBoxNames*) userData;
      
      std::string subj = (const char*) statement->subject;
      const char* pred = (const char*) statement->predicate;
      const char* obj  = (const char*) statement->object;
      
      if ((OWLParser::rdfType.compare(pred) == 0 &&
	   OWLParser::owlObjectProperty.compare(obj) == 0) ||
	  OWLParser::rdfsSubPropertyOf.compare(pred) == 0 ||
	  OWLParser::owlInverseOf.compare(pred) == 0
	  )
	{
	  t->roleNames->insert(Term(subj));
	}
      else if ((OWLParser::rdfType.compare(pred) == 0 &&
		OWLParser::owlClass.compare(obj) == 0) ||
	       OWLParser::owlSubClassOf.compare(pred) == 0
	       )
	{
	  t->conceptNames->insert(Term(subj));
	}
    }

    
    /**
     * The individual handler for libraptor.
     *
     * @param userData a downcasted Answer object for adding the individuals
     *
     * @param statement libraptor statement contains triples and
     * various other information
     */
    void
    aboxHandler(void* userData, const raptor_statement* statement)
    {
      Ontology::Objects* indvs = (Ontology::Objects*) userData;
      
      std::string subj = (const char*) statement->subject;
      const char* pred = (const char*) statement->predicate;
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
      if ((OWLParser::rdfType.compare(pred) == 0 &&
	   OWLParser::owlThing.compare(obj) == 0) ||
	  (OWLParser::rdfType.compare(pred) == 0 &&
	   OWLParser::rdfNspace  != objNspace &&
	   OWLParser::rdfsNspace != objNspace &&
	   OWLParser::owlNspace  != objNspace
	   )
	  )
	{
	  indvs->insert(Term(subj, true));
	}
    }


    void
    writeBytesHandler(raptor_www* /* w3 */, void* userData,
		      const void* ptr, size_t size, size_t nmemb)
    {
      raptor_iostream_write_bytes((raptor_iostream *) userData, ptr, size, nmemb);
    }
    
    
    void
    errorHandler(void* userData, raptor_locator* locator, const char* message)
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


  } // namespace dl
} // namespace dlvhex


/// Helper struct
struct OWLParser::HandlerFuns
{
  raptor_statement_handler stmHandler;
  raptor_namespace_handler nsHandler;
};


void
OWLParser::parse(void* userData, const HandlerFuns* handler) throw (DLParsingError)
{
  raptor_init();

  raptor_parser* parser = raptor_new_parser("rdfxml");

  std::string error;

  raptor_set_fatal_error_handler(parser, &error, errorHandler);
  raptor_set_error_handler(parser, &error, errorHandler);
  raptor_set_warning_handler(parser, 0, errorHandler);

  raptor_uri* parseURI  = raptor_new_uri((const unsigned char*) uri.getString().c_str());

  if (handler->stmHandler)
    {
      raptor_set_statement_handler(parser, userData, handler->stmHandler);
    }

  if (handler->nsHandler)
    {
      raptor_set_namespace_handler(parser, userData, handler->nsHandler);
    }

  raptor_parse_uri(parser, parseURI, 0);
    
  raptor_free_uri(parseURI);
  raptor_free_parser(parser);

  if (!error.empty())
    {
      throw DLParsingError(error);
    }

  raptor_finish();
}

void
OWLParser::parseIndividuals(Ontology::Objects& indvs) throw (DLParsingError)
{
  HandlerFuns funs = { aboxHandler, 0 };
  parse(&indvs, &funs);
}

void
OWLParser::parseNamespace(std::string& ns) throw (DLParsingError)
{
  HandlerFuns funs = { 0, namespaceHandler };
  parse(&ns, &funs);
}

void
OWLParser::parseNames(Ontology::Objects& concepts, Ontology::Objects& roles)
  throw (DLParsingError)
{
  TBoxNames names = { &concepts, &roles };
  HandlerFuns funs = { tboxHandler, 0 };
  parse(&names, &funs);
}

void
OWLParser::fetchURI(const std::string& file)
  throw (DLParsingError)
{
  raptor_init();

  raptor_www_init();

  raptor_uri* fetchURI = raptor_new_uri((const unsigned char*) uri.getString().c_str());

  raptor_iostream* io = raptor_new_iostream_to_filename(file.c_str());

  raptor_www_init();

  raptor_www* rw3 = raptor_www_new();

  raptor_www_set_write_bytes_handler(rw3, writeBytesHandler, io);

  std::string error;
  raptor_www_set_error_handler(rw3, errorHandler, &error);

  raptor_www_fetch(rw3, fetchURI);

  raptor_www_free(rw3);
  raptor_free_uri(fetchURI);
  raptor_free_iostream(io);

  raptor_www_finish();
  
  raptor_finish();

  if (!error.empty())
    {
      throw DLParsingError(error);
    }
}
