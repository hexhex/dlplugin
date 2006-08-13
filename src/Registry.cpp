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
std::string Registry::kbName;
std::string Registry::openURI;


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

std::string
Registry::getKBName()
{
  return Registry::kbName;
}

void
Registry::setKBName(const std::string& k)
{
  Registry::kbName = k;
}

std::string
Registry::getOpenURI()
{
  return Registry::openURI;
}

void
Registry::setOpenURI(const std::string& o)
{
  Registry::openURI = o;
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
