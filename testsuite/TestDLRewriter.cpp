/* dlvhex-dlplugin -- Integration of Answer-Set Programming and Description Logics.
 *
 * Copyright (C) 2005, 2006, 2007  Thomas Krennwallner
 * 
 * This file is part of dlvhex-dlplugin.
 *
 * dlvhex-dlplugin is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * dlvhex-dlplugin is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dlvhex-dlplugin; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


/**
 * @file   TestDLRewriter.cpp
 * @author Thomas Krennwallner
 * @date   Thu Apr 27 13:28:39 2006
 * 
 * @brief  Tests the DL Rewriter facility.
 * 
 * 
 */

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
   
   HexDLDriver dlr;
   dlr.setOntology(Ontology::createOntology("file:" + examples + "/tweety_bird.owl"));
   dlr.convert(fs, os);
   
   std::cout << "## " << std::endl << os.str() << std::endl;
 }

 {
   std::fstream fs((examples + "/dl-shop.dlp").c_str(), std::fstream::in);
   CPPUNIT_ASSERT( fs.is_open() );

   std::ostringstream os;
   
   HexDLDriver dlr;
   dlr.setOntology(Ontology::createOntology(shopuri));
   dlr.convert(fs, os);
   
   std::cout << "## " << std::endl << os.str() << std::endl;
 }

 {
   std::istringstream is("p(X,Y) :- &dlCQ[\"my.owl\",a,b,c,d,\"Q1(X),Q2(Y)\"](X, Y), &dlCQ[\"my.owl\",a,b,c,d,\"Q3(Y),Q4(Z)\"](Y,Z).");
   std::ostringstream os;

   HexDLDriver dlr;
   dlr.convert(is, os);

   std::string s = os.str();
   
   std::cout << "## " << std::endl << s << std::endl;

   CPPUNIT_ASSERT("p(X,Y) :- &dlCQ2[\"my.owl\",a,b,c,d,\"Q1(X),Q2(Y)\"](X,Y), &dlCQ2[\"my.owl\",a,b,c,d,\"Q3(Y),Q4(Z)\"](Y,Z)." == s);
 }
}

void
TestDLRewriter::runDLNoRewrite()
{
  std::fstream fs((examples + "/shop.dlp").c_str(), std::fstream::in);
  CPPUNIT_ASSERT( fs.is_open() );
  
  std::ostringstream os;

  HexDLDriver dlr;
  dlr.setOntology(Ontology::createOntology(shop));
  dlr.convert(fs, os);

  std::fstream fs2((examples + "/shop.dlp").c_str(), std::fstream::in);
  CPPUNIT_ASSERT( fs2.is_open() );

  std::stringbuf cmp;
  fs2.get(cmp, 0);

  std::string s = cmp.str();

  std::cerr << s << std::endl;

  // hex programs must stay the same
  //CPPUNIT_ASSERT(s == os.str());
}


// Local Variables:
// mode: C++
// End:
