/**
 * @file   TestDLRewriter.cpp
 * @author Thomas Krennwallner
 * @date   Thu Apr 27 13:28:39 2006
 * 
 * @brief  Tests the DL Rewriter facility.
 * 
 * 
 */

#include "HexDLRewriterDriver.h"

#include "TestDLRewriter.h"

#include <fstream>
#include <sstream>

using namespace dlvhex::dl;
using namespace dlvhex::dl::test;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestDLRewriter);

void
TestDLRewriter::runDLRewrite()
{
 {
   std::fstream fs((examples + "/tweety_bird.dlp").c_str(), std::fstream::in);
   CPPUNIT_ASSERT( fs.is_open() );

   std::ostringstream os;
   
   HexDLRewriterDriver dr(fs, os);
   dr.setUri("file:" + examples + "/tweety_bird.owl");
   dr.setStreams(&fs, &os);
   dr.rewrite();
   
   std::cout << "## " << std::endl << os.str() << std::endl;
 }

 {
   std::fstream fs((examples + "/dl-shop.dlp").c_str(), std::fstream::in);
   CPPUNIT_ASSERT( fs.is_open() );

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

   CPPUNIT_ASSERT("p(X,Y) :- &dlCQ3[\"my.owl\",a,b,c,d,\"Q1(X),Q2(Y),Q3(Y),Q4(Z)\"](X,Y,Z).\n" == s);
 }
}

void
TestDLRewriter::runDLNoRewrite()
{
  std::fstream fs((examples + "/shop.dlp").c_str(), std::fstream::in);
  CPPUNIT_ASSERT( fs.is_open() );
  
  std::ostringstream os;

  HexDLRewriterDriver dr(fs, os);
  dr.setUri(shop);
  dr.setStreams(&fs, &os);
  dr.rewrite();

  std::fstream fs2((examples + "/shop.dlp").c_str(), std::fstream::in);
  CPPUNIT_ASSERT( fs2.is_open() );

  std::stringbuf cmp;
  fs2.get(cmp, 0);

  // hex programs must stay the same
  //CPPUNIT_ASSERT(cmp.str() == os.str());
}
