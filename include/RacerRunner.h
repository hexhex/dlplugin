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


namespace dlvhex {
namespace dl {
namespace racer {

  /**
   * @brief Starts RACER in another process.
   */
  class RacerRunnerAdaptee : public ACE_Event_Handler
  {
  private:
    /// RACER command
    std::string command;
    /// the listening port of the RACER server
    unsigned port;
    /// start racer with this kb file
    std::string kb;
    /// RACER process id
    pid_t racer;
    /// signal handler
    ACE_Sig_Handler sighandler;

    /// signal handler
    virtual int
    handle_signal(int signum, siginfo_t* = 0, ucontext_t* = 0);

    /// try to kill a stale RACER process
    virtual void
    cleanup();

    /// store RACERs PID in a pid-file located in dlvhex user directory
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

    virtual bool
    isRunning() const;

    virtual void
    setPort(unsigned port);

    virtual unsigned
    getPort() const;

    /// transparently allow access from ACE_Singleton<>
    friend class ACE_Singleton<RacerRunnerAdaptee, ACE_Null_Mutex>;
  };

  /// adapt RacerRunnerAdaptee to a singleton and register it to the
  /// ACE_Object_Manager facility for automatic object deletion at
  /// program exit time
  typedef ACE_Singleton<RacerRunnerAdaptee, ACE_Null_Mutex> RacerRunner;

} // namespace racer
} // namespace dl
} // namespace dlvhex

#endif /* _RACERERROR_H */
