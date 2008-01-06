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
#include "Ontology.h"
#include "DLOptimizer.h"
#include "HexDLDriver.h"
#include "LogBuf.h"
#include "Registry.h"
#include "TCPStream.h"
#include "Cache.h"
#include "DLError.h"
#include "RacerBuilder.h"
#include "RacerAnswerDriver.h"
#include "Answer.h"
#include "RacerKBManager.h"

#include <iosfwd>
#include <algorithm>
#include <iterator>

#include <boost/tokenizer.hpp>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

namespace dlvhex {
  namespace dl {
    namespace racer {


RacerInterface::RacerInterface()
  : stream(new dlvhex::util::TCPIOStream("localhost", 8088)),
    stats(new CacheStats),
    cache(new Cache(*stats)),
    dlconverter(new HexDLDriver),
    dloptimizer(new DLOptimizer),
    kbManager(new RacerKBManager(*stream))
{ }


RacerInterface::RacerInterface(const RacerInterface&)
  : PluginInterface(),
    stream(0),
    stats(0),
    cache(0),
    dlconverter(0),
    dloptimizer(0),
    kbManager(0)
{ /* ignore */ }


RacerInterface&
RacerInterface::operator= (const RacerInterface&)
{
  return *this; // ignore
}


RacerInterface::~RacerInterface()
{
  try
    {
      if (Registry::getVerbose() > 2)
	{
	  std::cerr << *stats;
	}

      if (stream->isOpen())
	{
	  kbManager->removeKB(); // remove temporary abox
	}
    }
  catch (std::exception& e)
    {
      // we ignore this error since dtors must not throw exceptions
      std::cerr << "Couldn't remove temporary KB: " << e.what() << std::endl;
    }
  catch (...)
    {
      // we ignore this error since dtors must not throw exceptions
      std::cerr << "~RacerInterface: Caught unknown exception." << std::endl;
    }

  delete kbManager;
  delete dloptimizer;
  delete dlconverter;
  delete cache;
  delete stats;
  delete stream;
}


RacerInterface*
RacerInterface::instance()
{
  static RacerInterface ri;
  return &ri;
}


PluginConverter*
RacerInterface::createConverter()
{
  return dlconverter;
}


PluginOptimizer*
RacerInterface::createOptimizer()
{
  return dloptimizer;
}


/** 
 * We cannot rely on dlvhex calling RacerInterface::setOptions
 * before RacerInterface::getAtoms so we provide this class as
 * template parameter for the caching external atoms in order to
 * get the current cache.
 */
struct GetCacheFun
{
  dlvhex::dl::BaseCache&
  operator() () const
  {
    return *(RacerInterface::instance()->getCache());
  }
};

/** 
 * We cannot rely on dlvhex calling RacerInterface::setOptions
 * before RacerInterface::getAtoms so we provide this class as
 * template parameter for the external atoms in order to get the
 * current KBManager.
 */
struct GetKBManagerFun
{
  dlvhex::dl::KBManager&
  operator() () const
  {
    return *(RacerInterface::instance()->getKBManager());
  }
};


void
RacerInterface::getAtoms(AtomFunctionMap& m)
{
  // maybe we should provide the stream and the kbmanager as template
  // parameters similar to GetCache

  boost::shared_ptr<PluginAtom> dlC(new RacerConceptAtom<GetKBManagerFun,GetCacheFun>(*stream));
  boost::shared_ptr<PluginAtom> dlR(new RacerRoleAtom<GetKBManagerFun,GetCacheFun>(*stream));
  boost::shared_ptr<PluginAtom> dlConsistent(new RacerConsistentAtom<GetKBManagerFun>(*stream));
  boost::shared_ptr<PluginAtom> dlDR(new RacerDatatypeRoleAtom<GetKBManagerFun,GetCacheFun>(*stream));

  m["dlC"]          = dlC;
  m["dlR"]          = dlR;
  m["dlConsistent"] = dlConsistent;
  m["dlDR"]         = dlDR;

  // register for each arity in range 0 to 32 a dedicated RacerCQAtom
  // external atom with specified arity

  std::ostringstream oss;

  for (unsigned n = 0; n <= 32; ++n)
    {
      boost::shared_ptr<PluginAtom> dlCQ(new RacerCQAtom<GetKBManagerFun,GetCacheFun>(*stream, n));
      oss << "dlCQ" << n;
      m[oss.str()] = dlCQ;
      oss.str("");
    }

  // register for each arity in range 0 to 32 a dedicated RacerUCQAtom
  // external atom with specified arity

  for (unsigned n = 0; n <= 32; ++n)
    {
      boost::shared_ptr<PluginAtom> dlUCQ(new RacerUCQAtom<GetKBManagerFun,GetCacheFun>(*stream, n));
      oss << "dlUCQ" << n;
      m[oss.str()] = dlUCQ;
      oss.str("");
    }
}

void
RacerInterface::setOptions(bool doHelp, std::vector<std::string>& argv, std::ostream& out)
{
  if (doHelp)
    {
      out << "DL-plugin: " << std::endl << std::endl;
      out << " --ontology=URI        Use URI as ontology for dl-atoms." << std::endl;
      out << " --kb-reload           Force reloading of previously loaded ontologies." << std::endl;
      out << " --dlsetup=ARG[,ARG]*  Set DL-reasoner options, where ARG may be" << std::endl;
      out << "                       -una ... turn off unique name assumption (default: on)" << std::endl;
      out << " --dlopt=MOD[,MOD]*    Set optimization modifiers, where MOD may be" << std::endl;
      out << "                       -push    ... turn off pushing" << std::endl;
      out << "                       -dlcache ... turn off dl-cache" << std::endl;
      out << " --dldebug=LEVEL       Set debug level to LEVEL." << std::endl;
      return;
    }

  const char *ontology     = "--ontology=";
  const char *reload       = "--kb-reload";
  const char *setup        = "--dlsetup=";
  const char *optimization = "--dlopt=";
  const char *dldebug      = "--dldebug=";

  std::vector<std::string>::iterator it = argv.begin();

  while (it != argv.end())
    {
      std::string::size_type o;

      o = it->find(ontology);

      if (o != std::string::npos)
	{
	  std::string uri = it->substr(o + strlen(ontology)); // get URL
	  dlconverter->setURI(uri);

	  it = argv.erase(it);
	  continue;
	}

      o = it->find(reload);

      if (o != std::string::npos)
	{
	  ///@todo we ignore the fact that we could have multiple
	  ///occurrences of --kb-reload on the command line
	  KBManager *tmp = new NullKBManager(kbManager);
	  kbManager = tmp;

	  it = argv.erase(it);
	  continue;
	}

      o = it->find(setup);

      if (o != std::string::npos) // dispatch setup arguments
	{
	  std::string args = it->substr(o + strlen(setup)); // get list of ARG

	  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	  boost::char_separator<char> sep(",");
	  tokenizer tokens(args, sep);
	  for (tokenizer::iterator tok_iter = tokens.begin();
	       tok_iter != tokens.end(); ++tok_iter)
	    {
	      if (*tok_iter == "-una") // turn UNA off
		{
		  unsigned flags = Registry::getFlags();
		  Registry::setFlags(flags & ~Registry::UNA); // remove UNA flag
		}
	    }

	  it = argv.erase(it);
	  continue;
	}

      o = it->find(optimization);

      if (o != std::string::npos) // dispatch optimization modifiers
	{
	  std::string mods = it->substr(o + strlen(optimization)); // get list of MOD

	  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	  boost::char_separator<char> sep(",");
	  tokenizer tokens(mods, sep);
	  for (tokenizer::iterator tok_iter = tokens.begin();
	       tok_iter != tokens.end(); ++tok_iter)
	    {
	      if (*tok_iter == "-push") // no rewriting at all
		{
		  dloptimizer->setRewriting(false);
		}
	      else if (*tok_iter == "-dlcache") // no caching at all
		{
		  delete cache;
		  cache = new NullCache(*stats);
		}
	    }

	  it = argv.erase(it);
	  continue;
	}

      o = it->find(dldebug);

      if (o != std::string::npos)
	{
	  unsigned level;
	  std::string s = it->substr(o + strlen(dldebug)); // get LEVEL
	  std::istringstream iss(s);
	  iss >> level;

	  Registry::setVerbose(level);

	  if (Registry::getVerbose() > 2)
	    {
	      // get rid of null logger
	      delete dlvhex::util::log.rdbuf();
	      // use std::cerr as output for the LogBuf
	      dlvhex::util::log.rdbuf(new dlvhex::util::LogBuf(&std::cerr));

	      // use a verbose cache
	      delete cache;
	      cache = new DebugCache(*stats);
	    }

	  it = argv.erase(it);
	  continue;
	}

      ++it; // nothing found, check next position
    }
}


    } // namespace racer
  } // namespace dl
} // namespace dlvhex


extern "C" DLVHEX_NAMESPACE PluginInterface*
PLUGINIMPORTFUNCTION()
{
  dlvhex::dl::racer::RacerInterface::instance()->setVersion(DLPLUGIN_MAJOR,
							    DLPLUGIN_MINOR,
							    DLPLUGIN_MICRO);
  return dlvhex::dl::racer::RacerInterface::instance();
}


// Local Variables:
// mode: C++
// End:
