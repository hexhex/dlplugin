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
#include "HexDLDriver.h"

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
   
   HexDLDriver* dlr = new HexDLDriver(fs, os);
   HexDLRewriterDriver dr(dlr, fs, os);
   dlr->setURI("file:" + examples + "/tweety_bird.owl");
   dr.rewrite();
   
   std::cout << "## " << std::endl << os.str() << std::endl;
   delete dlr;
 }

 {
   std::fstream fs((examples + "/dl-shop.dlp").c_str(), std::fstream::in);
   CPPUNIT_ASSERT( fs.is_open() );

   std::ostringstream os;
   
   HexDLDriver* dlr = new HexDLDriver(fs, os);
   HexDLRewriterDriver dr(dlr, fs, os);
   dlr->setURI(shopuri);
   dr.rewrite();
   
   std::cout << "## " << std::endl << os.str() << std::endl;
   delete dlr;
 }

 {
   std::istringstream is("p(X,Y) :- &dlCQ[\"my.owl\",a,b,c,d,\"Q1(X),Q2(Y)\"](X, Y), &dlCQ[\"my.owl\",a,b,c,d,\"Q3(Y),Q4(Z)\"](Y,Z).");
   std::ostringstream os;

   HexDLDriver* dlr = new HexDLDriver(is, os);
   HexDLRewriterDriver dr(dlr, is, os);
   dr.setStreams(&is, &os);
   dr.rewrite();

   std::string s = os.str();
   
   std::cout << "## " << std::endl << s << std::endl;

   CPPUNIT_ASSERT("p(X,Y) :- &dlCQ3[\"my.owl\",a,b,c,d,\"Q1(X),Q2(Y),Q3(Y),Q4(Z)\"](X,Y,Z).\n" == s);
   delete dlr;
 }
}

void
TestDLRewriter::runDLNoRewrite()
{
  std::fstream fs((examples + "/shop.dlp").c_str(), std::fstream::in);
  CPPUNIT_ASSERT( fs.is_open() );
  
  std::ostringstream os;

  HexDLDriver* dlr = new HexDLDriver(fs, os);
  HexDLRewriterDriver dr(dlr, fs, os);
  dlr->setURI(shop);
  dr.rewrite();

  std::fstream fs2((examples + "/shop.dlp").c_str(), std::fstream::in);
  CPPUNIT_ASSERT( fs2.is_open() );

  std::stringbuf cmp;
  fs2.get(cmp, 0);

  delete dlr;

  // hex programs must stay the same
  //CPPUNIT_ASSERT(cmp.str() == os.str());
}
