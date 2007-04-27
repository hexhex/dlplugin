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

using namespace dlvhex::dl::racer;


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


namespace dlvhex {
  namespace dl {
    namespace racer {

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
 
    } // namespace racer
  } // namespace dl
} // namespace dlvhex


void
RacerInterface::getAtoms(AtomFunctionMap& m)
{
  // maybe we should provide the stream and the kbmanager as template
  // parameters similar to GetCache

  m["dlC"]          = new RacerConceptAtom<GetCacheFun>(*stream, *kbManager);
  m["dlR"]          = new RacerRoleAtom<GetCacheFun>(*stream, *kbManager);
  m["dlConsistent"] = new RacerConsistentAtom(*stream, *kbManager);
  m["dlDR"]         = new RacerDatatypeRoleAtom<GetCacheFun>(*stream, *kbManager);

  // register for each arity in range 0 to 32 a dedicated RacerCQAtom
  // external atom with specified arity

  std::ostringstream oss;

  for (unsigned n = 0; n <= 32; ++n)
    {
      oss << "dlCQ" << n;
      m[oss.str()]  = new RacerCQAtom<GetCacheFun>(*stream, *kbManager, n);
      oss.str("");
    }

  // register for each arity in range 0 to 32 a dedicated RacerUCQAtom
  // external atom with specified arity

  for (unsigned n = 0; n <= 32; ++n)
    {
      oss << "dlUCQ" << n;
      m[oss.str()]  = new RacerUCQAtom<GetCacheFun>(*stream, *kbManager, n);
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
      out << " --dlopt=MOD[,MOD]*   Set optimization modifiers, where MOD may be" << std::endl;
      out << "                      -push    ... turn off pushing" << std::endl;
      out << "                      -dlcache ... turn off dl-cache" << std::endl;
      out << " --dldebug=LEVEL      Set debug level to LEVEL." << std::endl;
      return;
    }

  std::vector<std::vector<std::string>::iterator> found;

  const char *ontology     = "--ontology=";
  const char *optimization = "--dlopt=";
  const char *dldebug      = "--dldebug=";

  for (std::vector<std::string>::iterator it = argv.begin();
       it != argv.end();
       ++it)
    {
      std::string::size_type o;

      o = it->find(ontology);

      if (o != std::string::npos)
	{
	  std::string uri = it->substr(o + strlen(ontology)); // get URL
	  dlconverter->setURI(uri);
	  found.push_back(it);
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

	  found.push_back(it);
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

	  found.push_back(it);
	}
    }

  // we handled it so we've got to remove it. do this right after the
  // for-loop due to invalidation of the iterator in vector<>::erase()
  for (std::vector<std::vector<std::string>::iterator>::const_iterator it = found.begin();
       it != found.end(); ++it)
    {
      argv.erase(*it);
    }
}


extern "C" PluginInterface*
PLUGINIMPORTFUNCTION()
{
  RacerInterface::instance()->setVersion(DLPLUGIN_MAJOR,
					 DLPLUGIN_MINOR,
					 DLPLUGIN_MICRO);
  return RacerInterface::instance();
}
