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

#include <boost/shared_ptr.hpp>

#include "DLError.h"

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
     * Remove #kbName.
     */
    virtual void
    removeKB() throw (DLError)
    { }

    /** 
     * Update the list of open kbs, that is refill #openKBs
     */
    virtual void
    updateOpenKB() throw (DLError)
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

    /** 
     * @param kb check if kb is loaded.
     * @return true if #kb is in #openKBs, false otw.
     */
    virtual bool
    isOpenKB(const std::string& kb)
    {
      return openKBs.find(kb) != openKBs.end();
    }

  };


  /**
   * Does not manage DL-KBs, instead let the managed KBManager do the
   * dirty work. But, isOpenKB() always return false, hence this class
   * useful for forcing the reload of KBs in the DL-reasoner.
   */
  class NullKBManager : public KBManager
  {
  private:
    boost::shared_ptr<KBManager> kbMan;

  public:
    explicit
    NullKBManager(KBManager *k) : KBManager(""), kbMan(k)
    { }

    const std::string&
    getKBName() const
    {
      return kbMan->getKBName();
    }

    const KBSet&
    getOpenKB()
    {
      return kbMan->getOpenKB();
    }

    void
    removeKB() throw (DLError)
    {
      kbMan->removeKB();
    }

    void
    updateOpenKB() throw (DLError)
    {
      kbMan->updateOpenKB();
    }

    /// @return false
    bool
    isOpenKB(const std::string&)
    {
      return false;
    }
  };


} // namespace dl
} // namespace dlvhex

#endif /* _KBMANAGER_H */
