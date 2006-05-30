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
#include "Registry.h"

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
  std::set<Term> indvs;
  p.parseIndividuals(indvs);
  std::insert_iterator<std::list<Term> > ins = std::inserter(uni, uni.begin());
  std::copy(indvs.begin(), indvs.end(), ins);
}

void
RacerInterface::getAtoms(AtomFunctionMap& m)
{
  m["dlC"]          = new RacerConcept(stream, cache, registry);
  m["dlR"]          = new RacerRole(stream, cache, registry);
  m["dlConsistent"] = new RacerConsistent(stream, registry);
  m["dlDR"]         = new RacerDatatypeRole(stream, cache, registry);
}

void
RacerInterface::setOptions(std::vector<std::string>& argv)
{
  // just in case we want verbose logging
  std::streambuf* logrdbuf = std::clog.rdbuf();
  // turn off logging
  std::clog.rdbuf(new LogBuf(0));

  /// @todo we want --verbose/--silent in argv...
  const char* c = getenv("DEBUGCACHE");
  if (c != 0)
    {
      unsigned level;
      std::string s(c);
      std::istringstream iss(s);
      iss >> level;

      registry.setVerbose(level);

      if (level > 1)
	{
	  // get rid of null logger
	  delete std::clog.rdbuf();
	  // use std::clogs original rdbuf as output for the LogBuf
	  std::clog.rdbuf(new LogBuf(logrdbuf));
	}
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
	}

      o = it->find("--nocache");

      if (o != std::string::npos)
	{
	  registry.setVerbose(0);
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
