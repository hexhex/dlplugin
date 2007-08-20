/* dlvhex-dlplugin -- Integration of Answer-Set Programming and Description Logics.
 *
 * Copyright (C) 2005, 2006, 2007  Thomas Krennwallner
 * 
 * This file is part of dlvhex-dlplugin.
 *
 * dlvhex-dlplugin is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * dlvhex-dlplugin is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dlvhex-dlplugin; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


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

using namespace dlvhex::dl;

//
// default values for the registry
//
unsigned Registry::flags(Registry::UNA);
unsigned Registry::verbose(1);
bool Registry::una(false);
bool Registry::datasubstrate(false);



void
Registry::setFlags(unsigned flags)
{
  Registry::flags = flags;
}

unsigned
Registry::getFlags()
{
  return Registry::flags;
}

 

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


// Local Variables:
// mode: C++
// End:

