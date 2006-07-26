/**
 * @file   TestDLRewriter.cpp
 * @author Thomas Krennwallner
 * @date   Thu Apr 27 13:28:39 2006
 * 
 * @brief  
 * 
 * 
 */

#include "HexDLRewriterDriver.h"

#include "TestDLRewriter.h"

#include <fstream>
#include <sstream>

using namespace dlvhex::racer;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestDLRewriter);

void
TestDLRewriter::runDLRewrite()
{
 {
   std::fstream fs((examples + "/tweety_bird.dlp").c_str());
   std::ostringstream os;
   
   HexDLRewriterDriver dr(fs, os);
   dr.setUri("file:" + examples + "/tweety_bird.owl");
   dr.setStreams(&fs, &os);
   dr.rewrite();
   
   std::cout << "## " << std::endl << os.str() << std::endl;
 }

 {
   std::fstream fs((examples + "/dl-shop.dlp").c_str());
   std::ostringstream os;
   
   HexDLRewriterDriver dr(fs, os);
   dr.setUri(shopuri);
   dr.setStreams(&fs, &os);
   dr.rewrite();
   
   std::cout << "## " << std::endl << os.str() << std::endl;
 }

 {
   std::istringstream is("p(X,Y) :- &dlCQ[\"my.owl\",a,b,c,d,\"Q1(X),Q2(Y)\"](X, Y), &dlCQ[\"my.owl\",a,b,c,d,\"Q3(Y),Q4(Z)\"](Y,Z).");
   std::ostringstream os;
   
   HexDLRewriterDriver dr(is, os);
   dr.setStreams(&is, &os);
   dr.rewrite();

   std::string s = os.str();
   
   std::cout << "## " << std::endl << s << std::endl;

   CPPUNIT_ASSERT("p(X,Y) :- &dlCQ2[\"my.owl\",a,b,c,d,\"Q1(X),Q2(Y)\"](X, Y), &dlCQ2[\"my.owl\",a,b,c,d,\"Q3(Y),Q4(Z)\"](Y,Z)." == s);
 }
}

void
TestDLRewriter::runDLNoRewrite()
{
  std::fstream fs((examples + "/shop.dlp").c_str());
  std::ostringstream os;

  HexDLRewriterDriver dr(fs, os);
  dr.setUri(shop);
  dr.setStreams(&fs, &os);
  dr.rewrite();

  std::fstream fs2((examples + "/shop.dlp").c_str());
  std::stringbuf cmp;
  fs2.get(cmp, 0);

  // hex programs must stay the same
  CPPUNIT_ASSERT(cmp.str() == os.str());
}
