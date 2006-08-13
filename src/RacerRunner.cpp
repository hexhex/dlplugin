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
#include "DLError.h"
#include "LogBuf.h"
#include "UserDir.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <csignal>

#include <ace/Process_Manager.h>
#include <ace/Signal.h>
#include <ace/Pipe.h>

using namespace dlvhex::dl::racer;
using dlvhex::util::log;
using dlvhex::util::UserDir;

#define PIDFILE "racer.pid"


RacerRunnerAdaptee::RacerRunnerAdaptee()
  : command(RACERPATH),
    port(8088),
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

bool
RacerRunnerAdaptee::isRunning() const
{
  return racer != ACE_INVALID_PID;
}

void
RacerRunnerAdaptee::setKB(const std::string& kb)
{
  this->kb = kb;
}

void
RacerRunnerAdaptee::setPort(unsigned p)
{
  port = p;
}

unsigned
RacerRunnerAdaptee::getPort() const
{
  return port;
}


void
RacerRunnerAdaptee::cleanup()
{
  std::fstream s;

  UserDir().open(s, PIDFILE);

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
  UserDir dir;
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
//       cleanup();

      // register all "fatal" signals, so we can stop the running
      // RACER process
      sighandler.register_handler(SIGSEGV, this);
      sighandler.register_handler(SIGINT, this);
      sighandler.register_handler(SIGQUIT, this);
      sighandler.register_handler(SIGKILL, this);
      sighandler.register_handler(SIGTERM, this);
      sighandler.register_handler(SIGABRT, this); // assert() raises SIGABRT



      for (unsigned portCount = 0; portCount < 20; ++portCount)
	{
	  std::ostringstream cmdline;

	  port = 8088 + portCount; // use portCount as offset

	  cmdline << command 
		  << " -t 0 "    // turn off timeout
		  << " -http 0 " // turn off http requests
		  << " -p "
		  << port;

	  if (!kb.empty())
	    {
	      cmdline << " -init " << kb;
	    }

	  ACE_Process_Options opt;
	  opt.command_line(cmdline.str().c_str());

	  // redirect RACERs std{in,out,err} to /dev/null
	  ACE_HANDLE h = ACE_OS::open("/dev/null", O_RDWR);
	  //opt.set_handles(h, h, h);

	  ACE_Pipe p;
	  p.open();
	  opt.set_handles(h, p.write_handle(), p.write_handle());

	  // start RACER process
	  racer = ACE_Process_Manager::instance()->spawn(opt);
	  //	  ACE_OS::sleep(ACE_Time_Value(1)); // give RACER time to start

	  char buf[256];
	  int n;
	  std::ostringstream out;
	  const ACE_Time_Value tv(3);

	  do
	    {
	      n = ACE::recv(p.read_handle(), buf, 256, &tv);

	      if (n > 0)
		{
		  std::string b(buf, n);
		  out << b;
		}
	    }
	  while (n > 0);


	  // check whether the spawn succeeded
	  ACE_exitcode ec;
	  pid_t check = ACE_Process_Manager::instance()->wait(racer,
							      ACE_Time_Value(0),
							      &ec);

// 	  std::cerr << "racer: " << racer << ' ' << check << ' ' << ec << std::endl;

	  if (out.str().find("Local socket address already in use") != std::string::npos)
	    {
	      racer = ACE_INVALID_PID;
	      continue;
	    }



	  // if ACE_Process_Manager::wait() returns 0, a timeout occurred
	  // while waiting for the process -> RACER is running.  Otherwise
	  // it returns the pid of RACER and handled its exit status ->
	  // RACER is not running.
	  if (check == 0)
	    {
	      // 	  savePID();
	      log << "Started RACER with pid " << racer << " and port " << port << std::flush;
	      return;
	    }
	  else if (check == racer && ec == 512)
	    {
	      racer = ACE_INVALID_PID;
	      
	      std::ostringstream os;
	      os << "Couldn't execute program "
		 << command;
	      
	      throw DLError(os.str());
	    }
	  else if (check == racer && ec != 0)
	    {
	      racer = ACE_INVALID_PID;
	      
// 	      std::ostringstream os;
// 	      os << "Program "
// 		 << command
// 		 << " stopped with exit status "
// 		 << ec;
	      
// 	      throw DLError(os.str());
	    }
	  else if (check == racer && ec == 0)
	    {
	      racer = ACE_INVALID_PID;
	      
// 	      std::ostringstream os;
// 	      os << "Couldn't start "
// 		 << command
// 		 << ", maybe another RACER instance is running"
// 		 << " and I didn't succeed in stopping it.";

// 	      throw DLError(os.str());
	    }
	  else
	    {
	      throw DLError("Unknown error while starting " + command);
	    }
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
      UserDir().remove(PIDFILE);
    }
}
