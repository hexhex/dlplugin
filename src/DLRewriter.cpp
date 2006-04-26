/* -*- C++ -*- */

/**
 * @file   DLRewriter.cpp
 * @author Thomas Krennwallner
 * @date   Tue Apr 25 13:48:38 2006
 * 
 * @brief  
 * 
 * 
 */

#include "DLRewriter.h"
#include "OWLParser.h"

#include <algorithm>
#include <iterator>

#include <ace/Pipe.h>
#include <ace/Process_Manager.h>

using namespace dlvhex::racer;

DLRewriter::DLRewriter(std::istream& i, std::ostream& o)
  : PluginRewriter(i,o)
{ }

void
DLRewriter::setUri(const std::string& s)
{
  this->uri = s;
}

void
DLRewriter::setStreams(std::istream* i, std::ostream* o)
{
  in = i;
  out = o;
}

void
DLRewriter::rewrite()
{
  std::set<Term> concepts;
  std::set<Term> roles;

  OWLParser p(uri);

  p.parseNames(concepts, roles);
  
  
  std::string cmdline("dl-rewriter " + uri);

  for (std::set<Term>::const_iterator it = concepts.begin();
       it != concepts.end();
       it++)
    {
      cmdline += " ";
      cmdline += it->getUnquotedString();
    }

  cmdline += " --";

  for (std::set<Term>::const_iterator it = roles.begin();
       it != roles.end();
       it++)
    {
      cmdline += " ";
      cmdline += it->getUnquotedString();
    }

  ACE_Process_Options opt;
  opt.command_line(cmdline.c_str());

  ACE_Pipe pipe;
  pipe.open();
  opt.set_handles(pipe.read_handle(), pipe.write_handle());

  // start RACER process
  int rewriter = ACE_Process_Manager::instance()->spawn(opt);

  // move input to pipe

  // write pipe to output

  // check whether the spawn succeeded
  pid_t check = ACE_Process_Manager::instance()->terminate(rewriter);
}
