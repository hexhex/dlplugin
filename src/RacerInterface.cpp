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
#include "HexDLRewriterDriver.h"
#include "LogBuf.h"
#include "Registry.h"

#include <algorithm>
#include <iterator>

#include <ace/Null_Mutex.h>
#include <ace/Singleton.h>

using namespace dlvhex::racer;

RacerInterface::RacerInterface()
  : stream("localhost", 8088),
    cache(new Cache),
    rewriter(new HexDLRewriterDriver(std::cin, std::cout))
{ }

RacerInterface::~RacerInterface()
{
  delete rewriter;
  delete cache;
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
  m["dlC"]          = new RacerConceptAtom(stream, *cache);
  m["dlR"]          = new RacerRoleAtom(stream, *cache);
  m["dlConsistent"] = new RacerConsistentAtom(stream);
  m["dlDR"]         = new RacerDatatypeRoleAtom(stream, *cache);

  // register for each arity in range 0 to 32 a dedicated RacerCQAtom
  // external atom with specified arity

  std::ostringstream oss;

  for (unsigned n = 0; n <= 32; n++)
    {
      oss << "dlCQ" << n;
      m[oss.str()]  = new RacerCQAtom(stream, *cache, n);
      oss.str("");
    }
}

void
RacerInterface::setOptions(bool doHelp, std::vector<std::string>& argv, std::ostream& out)
{
  if (doHelp)
    {
      out << "DL-plugin: " << std::endl << std::endl;
      out << " --ontology=URI       Use URI as ontology for dl-atoms." << std::endl;
      out << " --nocache            Turn off dl-caching." << std::endl;
      out << " --debug=LEVEL        Set debug log level to LEVEL." << std::endl;
      return;
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
	  Registry::setVerbose(0);
	  found.push_back(it);
	}

      o = it->find("--debug=");

      if (o != std::string::npos)
	{
	  unsigned level;
	  std::string s = it->substr(o + 8); // length of --debug= is 8
	  std::istringstream iss(s);
	  iss >> level;

	  Registry::setVerbose(level);

	  if (level > 1)
	    {
	      // get rid of null logger
	      delete log.rdbuf();
	      // use std::cerr as output for the LogBuf
	      log.rdbuf(new LogBuf(&std::cerr));
	    }

	  delete cache;
	  cache = new DebugCache;

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
