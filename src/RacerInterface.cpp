/* -*- C++ -*- */

/**
 * @file   RacerInterface.cpp
 * @author Thomas Krennwallner
 * @date   Sun Jul  3 16:58:17 2005
 * 
 * @brief  Import function for registering the callback functions.
 * 
 * 
 */

#include "RacerInterface.h"

#include "RacerRunner.h"
#include "RacerExtAtom.h"
#include "OWLParser.h"
#include "DLRewriter.h"
#include "LogBuf.h"

#include <algorithm>
#include <iterator>

#include <ace/Null_Mutex.h>
#include <ace/Singleton.h>

using namespace dlvhex::racer;

RacerInterface::RacerInterface()
  : stream("localhost", 8088),
    rewriter(new DLRewriter(std::cin, std::cout))
{ }

RacerInterface::~RacerInterface()
{
  delete rewriter;
  // stop RacerPro server
  RacerRunner::instance()->stop();
}


PluginRewriter*
RacerInterface::createRewriter(std::istream& i, std::ostream& o)
{
  rewriter->setStreams(&i, &o);
  return rewriter;
}

void
RacerInterface::getUniverse(std::string& uri, std::list<Term>& uni)
{
  OWLParser p(uri);
  Answer a;
  p.parseIndividuals(a);

  ///@todo this is crap. we need a more flexible query container class
  const std::vector<Tuple>* tv = a.getTuples();
  std::insert_iterator<std::list<Term> > ins = std::inserter(uni, uni.begin());

  for (std::vector<Tuple>::const_iterator it = tv->begin();
       it != tv->end();
       it++)
    {
      std::copy((*it).begin(), (*it).end(), ins);
    }
}

void
RacerInterface::getAtoms(AtomFunctionMap& m)
{
  m["dlC"]          = new RacerConcept(stream, cache, properties);
  m["dlR"]          = new RacerRole(stream, cache, properties);
  m["dlConsistent"] = new RacerConsistent(stream, properties);
  m["dlDR"]         = new RacerDatatypeRole(stream, cache, properties);
}

void
RacerInterface::setOptions(std::vector<std::string>& argv)
{
  /// @todo we want --verbose/--silent in argv...
  const char* c = getenv("DEBUGCACHE");
  if (c != 0)
    {
      properties["verbose"] = std::string(c);

      // use std::clogs rdbuf as output for the LogBuf
      std::streambuf* logrdbuf = std::clog.rdbuf();
      std::clog.rdbuf(new LogBuf(logrdbuf));
    }
  else
    {
      // turn off logging
      std::clog.rdbuf(new LogBuf(0));
    }

  std::vector<std::vector<std::string>::iterator> found;

  for (std::vector<std::string>::iterator it = argv.begin();
       it != argv.end();
       it++)
    {
      std::string::size_type o;

      o = it->find("--ontology=");

      if (o != std::string::npos)
	{
	  std::string uri = it->substr(o + 11); // length of parameter = 11
	  rewriter->setUri(uri);
	  found.push_back(it);

	  properties["ontology"] = uri;
	}

      o = it->find("--nocache");

      if (o != std::string::npos)
	{
	  properties["verbose"] = "0";
	  found.push_back(it);
	}
    }

  // we handled it so we've got to remove it. do this right after the
  // for-loop due to invalidation of the iterator in vector<>::erase()
  for (std::vector<std::vector<std::string>::iterator>::const_iterator it = found.begin();
       it != found.end(); it++)
    {
      argv.erase(*it);
    }
}

void
RacerInterface::startRacer()
{
  if (!RacerRunner::instance()->isRunning())
    {
      RacerRunner::instance()->run();
      stream.setConnection("localhost", RacerRunner::instance()->getPort());
    }
}
 

extern "C" PluginInterface*
PLUGINIMPORTFUNCTION()
{
  /// adapt RacerInterface to a singleton and register it to the
  /// ACE_Object_Manager facility for automatic object deletion at
  /// program exit time
  return TheRacerInterface::instance();
}
