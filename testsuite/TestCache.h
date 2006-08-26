// -*- C++ -*-

/**
 * @file   TestCache.h
 * @author Thomas Krennwallner
 * @date   Wed Aug 23 17:42:18 2006
 * 
 * @brief  TestCases for the Cache.
 * 
 * 
 */


#ifndef _TESTCACHE_H
#define _TESTCACHE_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestSuite.h"

namespace dlvhex {
namespace dl {

  class CacheStats;
  class BaseCache;

namespace test {


  /**
   * @brief TestCases for Cache. 
   *
   * @test Tests the Caching facility.
   */
  class TestCache : public TestSuite
  {
    CPPUNIT_TEST_SUITE(TestCache);
    CPPUNIT_TEST(runBooleanCache);
    CPPUNIT_TEST(runNonBooleanCache);
    CPPUNIT_TEST_SUITE_END();

    CacheStats* stats;
    BaseCache* cache;

  public: 

    virtual void setUp();
   
    virtual void tearDown();

    void runBooleanCache();    

    void runNonBooleanCache();    
  };

} // namespace test
} // namespace dl
} // namespace dlvhex

#endif /* _TESTCACHE_H */
