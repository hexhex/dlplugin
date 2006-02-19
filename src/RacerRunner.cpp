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
#include "RacerError.h"

#include <string>
#include <iostream>
#include <sstream>
#include <csignal>

#include <ace/Process_Manager.h>
#include <ace/Signal.h>

using namespace dlvhex::racer;

#define PIDFILE "racer.pid"


RacerRunnerAdaptee::RacerRunnerAdaptee()
  : command(RACERPATH),
    kb(""),
    racer(ACE_INVALID_PID)
{
  // just instantiate ACE_Process_Manager before we are done
  // instantiating ourselves since RacerRunnerAdaptee and
  // ACE_Process_Manager are both managed by ACE_Object_Manager and it
  // will delete its objects in a LIFO fashion thus destroying the
  // process table of ACE_Process_Manager before we are ready to kill
  // RACER
  ACE_Process_Manager::instance();
}

RacerRunnerAdaptee::~RacerRunnerAdaptee()
{
  stop();
}

void
RacerRunnerAdaptee::setKB(const std::string& kb)
{
  this->kb = kb;
}

void
RacerRunnerAdaptee::cleanup()
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
RacerRunnerAdaptee::savePID()
{
  std::fstream s;
  dir.create(PIDFILE);
  dir.open(s, PIDFILE);
  s << racer;
  s.close();
}

void
RacerRunnerAdaptee::run()
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

      // check whether the spawn succeeded
      ACE_exitcode ec;
      pid_t check = ACE_Process_Manager::instance()->wait(racer,
							  ACE_Time_Value(1),
							  &ec);

      // if ACE_Process_Manager::wait() returns 0, a timeout occurred
      // while waiting for the process -> RACER is running.  Otherwise
      // it returns the pid of RACER and handled its exit status ->
      // RACER is not running.
      if (check == 0)
	{
	  savePID();
	}
      else if (check == racer && ec != 0)
	{
	  racer = ACE_INVALID_PID;

	  std::ostringstream os;
	  os << "Program "
	     << command
	     << " stopped with exit status "
	     << ec;

	  throw RacerError(os.str());
	}
      else if (check == racer && ec == 0)
	{
	  racer = ACE_INVALID_PID;

	  std::ostringstream os;
	  os << "Couldn't start "
	     << command
	     << ", maybe another RACER instance is running"
	     << " and I didn't succeed in stopping it.";

	  throw RacerError(os.str());
	}
      else
	{
	  throw RacerError("Unknown error while starting " + command);
	}
    }
}


int
RacerRunnerAdaptee::handle_signal(int signum, siginfo_t*, ucontext_t*)
{
  stop();

  // remove all signal handlers for signum -> prevents infinite loop
  sighandler.remove_handler(signum);

  return 0;
}


void
RacerRunnerAdaptee::stop()
{
  if (racer != ACE_INVALID_PID)
    {
      ACE_Process_Manager::instance()->terminate(racer, SIGINT);
      ACE_Process_Manager::instance()->wait(racer);
      racer = ACE_INVALID_PID;
      dir.remove(PIDFILE);
    }
}
