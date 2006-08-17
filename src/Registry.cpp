/* -*- C++ -*- */

/**
 * @file   Registry.cpp
 * @author Thomas Krennwallner
 * @date   Tue May 30 12:16:08 2006
 * 
 * @brief  Global state.
 * 
 * 
 */

#include "Registry.h"

#include <string>

using namespace dlvhex::dl;


unsigned Registry::verbose(1);
bool Registry::una(false);
bool Registry::datasubstrate(false);


unsigned
Registry::getVerbose()
{
  return Registry::verbose;
}

void
Registry::setVerbose(unsigned v)
{
  Registry::verbose = v;
}

bool
Registry::getUNA()
{
  return Registry::una;
}

void
Registry::setUNA(bool u)
{
  Registry::una = u;
}

bool
Registry::getDataSubstrateMirroring()
{
  return Registry::datasubstrate;
}

void
Registry::setDataSubstrateMirroring(bool d)
{
  Registry::datasubstrate = d;
}
