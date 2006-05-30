/* -*- C++ -*- */

/**
 * @file   Registry.h
 * @author Thomas Krennwallner
 * @date   Tue May 30 11:53:17 2006
 * 
 * @brief  
 * 
 * 
 */


#ifndef _REGISTRY_H
#define _REGISTRY_H

#include <string>

namespace dlvhex {
namespace racer {

  class Registry
  {
  private:
    unsigned verbose;
    bool una;
    bool datasubstrate;
    std::string kbName;
    std::string openURL;

  public:
    Registry();

    virtual
    ~Registry();

    unsigned
    getVerbose() const;

    void
    setVerbose(unsigned);

    std::string
    getKBName() const;

    void
    setKBName(const std::string&);

    std::string
    getOpenURL() const;

    void
    setOpenURL(const std::string&);

    bool
    getUNA() const;

    void
    setUNA(bool);

    bool
    getDataSubstrateMirroring() const;

    void
    setDataSubstrateMirroring(bool);
  };

} // namespace racer
} // namespace dlvhex

#endif /* _REGISTRY_H */
