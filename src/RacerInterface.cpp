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

#include <ace/Null_Mutex.h>
#include <ace/Singleton.h>

using namespace dlvhex::racer;

RacerInterface::RacerInterface()
  : stream("localhost", 8088)
{ }

RacerInterface::~RacerInterface()
{
  RacerRunner::instance()->stop();

  // delete all cached QueryCtx's
  for (RacerCachingDirector::RacerCache::const_iterator it = cache.begin();
       it != cache.end(); it++)
    {
      delete *it;
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
  m["dlC"] = new RacerConcept(stream, cache);
  m["dlR"] = new RacerRole(stream, cache);
  m["dlConsistent"] = new RacerConsistent(stream);
}


extern "C" PluginInterface*
PLUGINIMPORTFUNCTION()
{
  /// adapt RacerInterface to a singleton and register it to the
  /// ACE_Object_Manager facility for automatic object deletion at
  /// program exit time
  return ACE_Singleton<RacerInterface, ACE_Null_Mutex>::instance();
}
