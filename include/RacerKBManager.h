/* -*- C++ -*- */

/**
 * @file   RacerKBManager.h
 * @author Thomas Krennwallner
 * @date   Mon Aug 14 11:41:57 2006
 * 
 * @brief  KB Manager class for RACER KBs.
 * 
 * 
 */

#ifndef _RACERKBMANAGER_H
#define _RACERKBMANAGER_H

#include "DLError.h"
#include "KBManager.h"

#include <string>
#include <iosfwd>

namespace dlvhex {
namespace dl {
namespace racer {

  /**
   * @brief KB Manager class for RACER KBs.
   */
  class RacerKBManager : public KBManager
  {
  protected:
    std::iostream& stream;

  public:
    /** 
     * Use @a s to send and retrieve data from RACER. @a name is used
     * as temporary ABox for the (clone-abox) command, if @a name is
     * empty we compute a system-wide unique ABox name.
     * 
     * @param s copy @a s to #stream
     * @param name copy @a name to #kbName, otw. create a unique KB name
     */
    explicit
    RacerKBManager(std::iostream& s, const std::string& name = "");

    /** 
     * Remove ABox #kbName from RACER.
     */
    void
    removeKB() throw (DLError);

    /** 
     * Update the list of open kbs, that is refill #openKBs
     */
    void
    updateOpenKB() throw (DLError);
  };

} // namespace racer
} // namespace dl
} // namespace dlvhex

#endif /* _RACERKBMANAGER_H */
