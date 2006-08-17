/* -*- C++ -*- */

/**
 * @file   KBManager.h
 * @author Thomas Krennwallner
 * @date   Mon Aug 14 11:41:57 2006
 * 
 * @brief  Base class for dl knowledge base managers.
 * 
 * 
 */

#ifndef _KBMANAGER_H
#define _KBMANAGER_H

#include <string>
#include <vector>

namespace dlvhex {
namespace dl {

  /**
   * @brief Base class for dl knowledge base managers.
   */
  class KBManager
  {
  public:
    typedef std::vector<std::string> KBList;

  protected:
    /// a KB name
    std::string kbName;
    /// a list of open KBs
    KBList openKBs;
    
  public:
    /** 
     * Set #kbName and #openKBs.
     * 
     * @param name 
     * @param kbs 
     */
    explicit
    KBManager(const std::string& name, const KBList& kbs = KBList())
      : kbName(name), openKBs(kbs)
    { }

    virtual
    ~KBManager()
    { }

    /** 
     * @return #kbName
     */
    virtual const std::string&
    getKBName() const
    {
      return kbName;
    }

    /** 
     * @return #openKBs
     */
    virtual const KBList&
    getOpenKB()
    {
      return openKBs;
    }

  };

} // namespace dl
} // namespace dlvhex

#endif /* _KBMANAGER_H */
