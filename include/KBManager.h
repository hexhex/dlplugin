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
#include <set>

namespace dlvhex {
namespace dl {

  /**
   * @brief Base class for dl knowledge base managers.
   */
  class KBManager
  {
  public:
    typedef std::set<std::string> KBSet;

  protected:
    /// a KB name
    std::string kbName;
    /// a list of open KBs
    KBSet openKBs;
    
  public:
    /** 
     * Set #kbName and #openKBs.
     * 
     * @param name 
     * @param kbs 
     */
    explicit
    KBManager(const std::string& name, const KBSet& kbs = KBSet())
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
    virtual const KBSet&
    getOpenKB()
    {
      return openKBs;
    }

    virtual bool
    isOpenKB(const std::string& kb)
    {
      return openKBs.find(kb) != openKBs.end();
    }

  };

} // namespace dl
} // namespace dlvhex

#endif /* _KBMANAGER_H */
