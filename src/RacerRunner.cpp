/* -*- C++ -*- */

/**
 * @file   RacerRunner.cpp
 * @author Thomas Krennwallner
 * @date   Thu Jul 21 09:15:37 2005
 * 
 * @brief  Run RACER in another process.
 * 
 *
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include "RacerRunner.h"

#include <string>
#include <iostream>
#include <csignal>

#include <ace/Process_Manager.h>
#include <ace/Signal.h>

using namespace dlvhex::racer;

#define PIDFILE "racer.pid"

RacerRunner* RacerRunner::runner = 0;

RacerRunner::RacerRunner()
  : command(RACERPATH),
    kb(""),
    racer(ACE_INVALID_PID)
{ }

RacerRunner::~RacerRunner()
{
  stop();
}

RacerRunner*
RacerRunner::instance()
{
  if (RacerRunner::runner == 0)
    {
      RacerRunner::runner = new RacerRunner;
    }

  return RacerRunner::runner;
}

void
RacerRunner::setKB(const std::string& kb)
{
  this->kb = kb;
}

void
RacerRunner::cleanup()
{
  std::fstream s;

  dir.open(s, PIDFILE);

  if (s.is_open())
    {
      s >> racer;
      s.close();

      // Found stale RACER process -> try to kill it
      stop();
    }
}

void
RacerRunner::savePID()
{
  std::fstream s;
  dir.create(PIDFILE);
  dir.open(s, PIDFILE);
  s << racer;
  s.close();
}

void
RacerRunner::run()
{
  if (racer == ACE_INVALID_PID)
    {
      cleanup();

      // register all "fatal" signals s.t. RACER will always be killed
      sighandler.register_handler(SIGSEGV, this);
      sighandler.register_handler(SIGINT, this);
      sighandler.register_handler(SIGQUIT, this);
      sighandler.register_handler(SIGKILL, this);
      sighandler.register_handler(SIGTERM, this);

      std::string cmdline(command);

      cmdline += " -http 0 "; // turn off http requests

      if (!kb.empty())
	{
	  cmdline += " -init " + kb;
	}

      ACE_Process_Options opt;
      opt.command_line(cmdline.c_str());

      // redirect RACERs std{in,out,err} to /dev/null
      ACE_HANDLE h = ACE_OS::open("/dev/null", O_RDWR);
      opt.set_handles(h, h, h);

      // start RACER process
      racer = ACE_Process_Manager::instance()->spawn(opt);

      if (racer == ACE_INVALID_PID)
	{
	  // throw exception?
	}
      else
	{
	  savePID();
	}
    }
}


int
RacerRunner::handle_signal(int signum, siginfo_t*, ucontext_t*)
{
  stop();

  // remove all signal handlers for signum -> prevents infinite loop
  sighandler.remove_handler(signum);

  return 0;
}


void
RacerRunner::stop()
{
  if (racer != ACE_INVALID_PID)
    {
      ACE_Process_Manager::instance()->terminate(racer, SIGINT);
      ACE_Process_Manager::instance()->wait(racer);
      racer = ACE_INVALID_PID;
      dir.remove(PIDFILE);
    }
}
