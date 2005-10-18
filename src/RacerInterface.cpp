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

#include "RacerExtAtom.h"
#include "OWLParser.h"

#include <algorithm>
#include <iterator>

#include <ace/OS.h>

using namespace dlvhex::racer;

RacerInterface::RacerInterface()
{
  RacerRunner::instance()->run();

  ACE_Time_Value tv(0, 300000);

  // retry until RACER is up and running
  for (int i = 10; i > 0; i--)
    {
      ACE_OS::sleep(tv);

      if (stream.open("localhost", 8088))
	{
	  std::cerr << "no conn" << std::endl;
	}
      else
	{
	  break;
	}
    }
}

RacerInterface::~RacerInterface()
{
  stream.close();
  RacerRunner::instance()->stop();

  // delete all cached QueryCtx's
  for (RacerCachingDirector::RacerCache::const_iterator it = cache.begin();
       it != cache.end(); it++)
    {
      delete (*it).second;
    }
}


PluginRewriter*
RacerInterface::createRewriter(std::istream&, std::ostream&)
{
  return 0; // nothing implemented yet
}

void
RacerInterface::getUniverse(std::string& uri, std::list<Term>& uni)
{
  OWLParser p(uri);
  Answer a;
  p.parseIndividuals(a);

  ///@todo this is crap. we need a more flexible query container class
  const PluginAtom::TUPLEVECTOR& tv = a.getTuples();
  std::insert_iterator<std::list<Term> > ins = std::inserter(uni, uni.begin());

  for (PluginAtom::TUPLEVECTOR::const_iterator it = tv.begin();
       it != tv.end();
       it++)
    {
      std::copy((*it).begin(), (*it).end(), ins);
    }
}

void
RacerInterface::getAtoms(AtomFunctionMap& m)
{
  m["racerC"] = new RacerConcept(stream, cache);
  (*m["racerC"]).setArities(6,1);

  m["racerR"] = new RacerRole(stream, cache);
  (*m["racerR"]).setArities(6,2);
}


RacerInterface theRacerInterface;

extern "C" PluginInterface*
PLUGINIMPORTFUNCTION()
{
  return &theRacerInterface; // singleton?
}
