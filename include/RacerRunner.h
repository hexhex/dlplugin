/* -*- C++ -*- */

/**
 * @file   RacerRunner.h
 * @author Thomas Krennwallner
 * @date   Thu Jul 21 09:15:37 2005
 * 
 * @brief  Run RACER in another process.
 * 
 * 
 */

#ifndef _RACERRUNNER_H
#define _RACERRUNNER_H

#include <string>

#include <ace/Event_Handler.h>
#include <ace/Signal.h>
#include <ace/Null_Mutex.h>
#include <ace/Singleton.h>

#include "UserDir.h"

namespace dlvhex {
namespace racer {

  /**
   * @brief Starts RACER in another process.
   */
  class RacerRunnerAdaptee : public ACE_Event_Handler
  {
  private:
    std::string command;
    std::string kb;
    pid_t racer;
    ACE_Sig_Handler sighandler;
    UserDir dir;

    /// signal handler
    virtual int
    handle_signal(int signum, siginfo_t* = 0, ucontext_t* = 0);

    /// try to kill a stale RACER process
    virtual void
    cleanup();

    /// store RACERs PID in a pid-file
    virtual void
    savePID();

    /// Ctor
    RacerRunnerAdaptee();

    /// Dtor
    virtual
    ~RacerRunnerAdaptee();

  public:
    virtual void
    setKB(const std::string&);

    /// start RACER
    virtual void
    run();

    /// stop running RACER and remove pid-file
    virtual void
    stop();

    /// transparently allow access from ACE_Singleton<>
    friend class ACE_Singleton<RacerRunnerAdaptee, ACE_Null_Mutex>;
  };

  /// adapt RacerRunnerAdaptee to a singleton and register it to the
  /// ACE_Object_Manager facility for automatic object deletion at
  /// program exit time
  typedef ACE_Singleton<RacerRunnerAdaptee, ACE_Null_Mutex> RacerRunner;

} // namespace racer
} // namespace dlvhex

#endif /* _RACERERROR_H */
