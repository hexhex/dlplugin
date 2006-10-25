/* -*- C++ -*- */

/**
 * @file   HexDLRewriterDriver.cpp
 * @author Thomas Krennwallner
 * @date   Tue Jul 25 09:23:09 2006
 * 
 * @brief  Driver class for the Hex DL Rewriters bison/flex parser.
 * 
 * 
 */

#include "HexDLRewriterDriver.h"
#include "HexDLRewriterFlexLexer.h"
#include "Ontology.h"
#include "Registry.h"
#include "DLError.h"

#include <dlvhex/Program.h>
#include <dlvhex/GraphBuilder.h>
#include <dlvhex/DependencyGraph.h>
#include <dlvhex/ComponentFinder.h>
#include <dlvhex/BoostComponentFinder.h>
#include <dlvhex/PrintVisitor.h>

#include <iosfwd>

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/erase.hpp>

using namespace dlvhex::dl;


HexDLRewriterDriver::HexDLRewriterDriver(std::istream& i, std::ostream& o)
  : PluginRewriter(i, o),
    lexer(new HexDLRewriterFlexLexer(this)),
    ontology(),
    doRewriting(true)
{
  lexer->switch_streams(&i, &o);
}


HexDLRewriterDriver::HexDLRewriterDriver(const HexDLRewriterDriver& d)
  : PluginRewriter(*d.input, *d.output),
    lexer(new HexDLRewriterFlexLexer(this)),
    ontology(d.ontology),
    doRewriting(d.doRewriting)
{
  lexer->switch_streams(d.input, d.output);
}


HexDLRewriterDriver&
HexDLRewriterDriver::operator= (const HexDLRewriterDriver& d)
{
  if (this != &d)
    {
      delete lexer;
      lexer = new HexDLRewriterFlexLexer(this);
      ontology = d.ontology;
      setStreams(d.input, d.output);
    }

  return *this;
}


HexDLRewriterDriver::~HexDLRewriterDriver()
{
  delete lexer;
}


HexDLRewriterFlexLexer*
HexDLRewriterDriver::getLexer() const
{
  return lexer;
}


std::ostream&
HexDLRewriterDriver::getOutput() const
{
  return *output;
}


void
HexDLRewriterDriver::setRewriting(bool yesno)
{
  doRewriting = yesno;
}


bool
HexDLRewriterDriver::getRewriting()
{
  return doRewriting;
}


void
HexDLRewriterDriver::setURI(const std::string& s)
{
  try
    {
      this->ontology = Ontology::createOntology(s);
    }
  catch (DLError& e)
    {
      throw PluginError(e.what());
    }
}


Ontology::shared_pointer
HexDLRewriterDriver::getOntology() const
{
  return this->ontology;
}


void
HexDLRewriterDriver::setStreams(std::istream* i, std::ostream* o)
{
  input = i;
  output = o;
  getLexer()->switch_streams(i, o);
}


void
HexDLRewriterDriver::rewrite()
{
  //
  // parse and rewrite that thing
  //

  Program prog;
  AtomSet edb;
  DLAtomInput dlinput;

  try
    {
      yy::HexDLRewriterParser parser(*this, prog, edb, dlinput);
      lexer->set_debug(Registry::getVerbose() > 2);
      parser.set_debug_level(Registry::getVerbose() > 2);
      parser.parse();
    }
  catch (DLParsingError& e)
    {
      throw PluginError(e.what());
    }

  //
  // build dependency graph
  //

//   GraphBuilder gb;
//   BoostComponentFinder cf;

//   DependencyGraph dg(prog, &gb, &cf);

//   Subgraph* sg = 0;

//   while ((sg = dg.getNextSubgraph()))
//     {
//       sg->dump(std::cerr);

//       std::vector<Component*> leaves;
      
//       sg->getUnsolvedLeaves(leaves);
      
//       for (std::vector<Component*>::const_iterator it = leaves.begin(); it != leaves.end(); ++it)
// 	{
// 	  Program p = (*it)->getBottom();
// 	  std::cerr << "Component found: " << std::endl;
// 	  for (Program::const_iterator pit = p.begin(); pit != p.end(); ++pit)
// 	    {
// 	      std::cerr << *pit << std::endl;
// 	    }
// 	  std::cerr << std::endl;
// 	}
//     }



  //
  // now add parsed rules and facts to the output
  //

  std::copy(edb.begin(), edb.end(), std::ostream_iterator<Atom>(getOutput(), ".\n"));

  //
  // rewrite the rules
  //

  Program rewritten;

  for (Program::const_iterator it = prog.begin(); it != prog.end(); ++it)
    {
      ///@todo does not take unused predicates in \lambda in account

      const Rule* r = *it;

      if (!getRewriting())
	{
	  //
	  // don't rewrite the body
	  //
	  rewritten.addRule(r);
	}
      else
	{
	  //
	  // rewrite the body of each rule
	  //

	  const RuleHead_t& head = r->getHead();
	  const RuleBody_t& body = r->getBody();

	  BodyRewriter br;
	  RuleBody_t body1;

	  for (RuleBody_t::const_iterator it = body.begin(); it != body.end(); ++it)
	    {
	      Literal* l = *it;
	      
	      if (typeid(*l->getAtom()) == typeid(ExternalAtom))
		{
		  ExternalAtom* ea = dynamic_cast<ExternalAtom*>(l->getAtom().get());

		  const std::string& fn = ea->getFunctionName();
		  
		  if (fn == "dlC" || fn == "dlR" || fn == "dlDR")
		    {
		      std::cerr << "found dl" << std::endl;

		      const std::string* n = new std::string(fn);
		      const Tuple* in = new Tuple(ea->getInputTerms());
		      const Tuple* out = new Tuple(ea->getArguments());
		      
		      br.add(new SimpleDLAtomRewriter(n, in, out));

		      ///@todo this burns the readers eyes, but for
		      ///now it prevents memory leaks
		      delete l;
		    }
		  else if (fn.find("dlCQ") == 0)
		    {
		      std::cerr << "found dlcq" << std::endl;

		      const Tuple* in = new Tuple(ea->getInputTerms());
		      const Tuple* out = new Tuple(ea->getArguments());
		      
		      br.add(new CQAtomRewriter(in, out));

		      ///@todo this burns the readers eyes, but for
		      ///now it prevents memory leaks
		      delete l;
		    }
		  else
		    {
		      body1.push_back(l); // non-dl extatom
		    }
		}
	      else
		{
		  body1.push_back(l); // non-extatom
		}
	    }
	  
	  
	  const RuleBody_t* dl_body = br.getBody();
	  
	  body1.insert(body1.end(), dl_body->begin(), dl_body->end());
	  
	  delete dl_body;
	  
	  const Rule* r1 = new Rule(head, body1);
	  
	  rewritten.addRule(r1);
	  
	  ///@todo this burns the readers eyes, but for now it prevents
	  ///memory leaks

	  delete r;
	}
    }

  //
  // and now output the rewritten program
  //

  RawPrintVisitor rpv(getOutput());

  for (Program::const_iterator rit = rewritten.begin(); rit != rewritten.end(); ++rit)
    {
      const Rule* r = *rit;
      r->accept(rpv);
      getOutput() << std::endl;

      ///@todo this burns the readers eyes, but for now it prevents
      ///memory leaks

      // delete unmanaged objects in the program

      // delete each literal in the body
      const RuleBody_t& body = r->getBody();
      for (RuleBody_t::const_iterator bit = body.begin(); bit != body.end(); ++bit)
	{
	  delete *bit;
	}

      delete r;
    }
}


void
HexDLRewriterDriver::error(const yy::location& l,
			   const std::string& m) const
  throw (DLParsingError)
{
  std::ostringstream s;
  s << "Parsing error at " << l << ": " << m;

  throw DLParsingError(s.str());
}
