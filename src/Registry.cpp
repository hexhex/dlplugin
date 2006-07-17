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

using namespace dlvhex::racer;

Registry::Registry()
  : verbose(1),
    una(false),
    datasubstrate(false)
{}


Registry::~Registry()
{}


unsigned
Registry::getVerbose() const
{
  return verbose;
}

void
Registry::setVerbose(unsigned v)
{
  verbose = v;
}

std::string
Registry::getKBName() const
{
  return kbName;
}

void
Registry::setKBName(const std::string& k)
{
  kbName = k;
}

std::string
Registry::getOpenURL() const
{
  return openURL;
}

void
Registry::setOpenURL(const std::string& o)
{
  openURL = o;
}

bool
Registry::getUNA() const
{
  return una;
}

void
Registry::setUNA(bool u)
{
  una = u;
}

bool
Registry::getDataSubstrateMirroring() const
{
  return datasubstrate;
}

void
Registry::setDataSubstrateMirroring(bool d)
{
  datasubstrate = d;
}
