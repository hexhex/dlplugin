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

namespace dlvhex {
namespace racer {

  /**
   * @brief Starts RACER in another process.
   */
  class RacerRunner : public ACE_Event_Handler
  {
  private:
    std::string command;
    std::string kb;
    pid_t racer;
    ACE_Sig_Handler sighandler;

    /// signal handler
    virtual int
    handle_signal(int signum, siginfo_t* = 0, ucontext_t* = 0);

  public:
    RacerRunner();

    explicit
    RacerRunner(const RacerRunner&);

    explicit
    RacerRunner(const std::string&);

    virtual
    ~RacerRunner();

    virtual void
    setKB(const std::string&);

    virtual void
    run();

    virtual void
    stop();
  };

} // namespace racer
} // namespace dlvhex

#endif /* _RACERERROR_H */
