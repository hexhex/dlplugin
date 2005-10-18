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

#include "UserDir.h"

namespace dlvhex {
namespace racer {

  /**
   * @brief Starts RACER in another process.
   */
  class RacerRunner : public ACE_Event_Handler
  {
  private:
    /// the singleton
    static RacerRunner* runner;

    std::string command;
    std::string kb;
    pid_t racer;
    ACE_Sig_Handler sighandler;
    UserDir dir;

    /// signal handler
    virtual int
    handle_signal(int signum, siginfo_t* = 0, ucontext_t* = 0);

    /// try to kill an old RACER process
    virtual void
    cleanup();

    /// store RACERs PID in a pid-file
    virtual void
    savePID();

  protected:         
    RacerRunner();

  public:
    /// singleton instance method
    static RacerRunner*
    instance();

    virtual
    ~RacerRunner();

    virtual void
    setKB(const std::string&);

    /// start RACER
    virtual void
    run();

    /// stop running RACER and remove pid-file
    virtual void
    stop();
  };

} // namespace racer
} // namespace dlvhex

#endif /* _RACERERROR_H */
