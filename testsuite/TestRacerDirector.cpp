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
 * @file   TestRacerDirector.cpp
 * @author Thomas Krennwallner
 * @date   Wed Feb  8 21:13:26 2006
 * 
 * @brief  TestCases for QueryDirector.
 * 
 * 
 */

#include "TestRacerDirector.h"
#include "RacerBuilder.h"
#include "RacerAnswerDriver.h"
#include "QueryDirector.h"
#include "TCPStream.h"
#include "QueryCtx.h"
#include "Query.h"
#include "RacerKBManager.h"
#include "Answer.h"

#include <iostream>
#include <string>
#include <functional>
#include <iterator>


using namespace dlvhex::util;
using namespace dlvhex::dl;
using namespace dlvhex::dl::racer;
using namespace dlvhex::dl::test;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestRacerDirector);


void
TestRacerDirector::runRacerPlusConceptTest()
{
  TCPIOStream rsIO("localhost", 8088);
      
  QueryDirector<RacerStateBuilder, RacerAnswerDriver> pcd(rsIO);
  
  AtomSet pc;
  AtomPtr ap(new Atom("plusC(\"Part\",\"foo\")"));
  pc.insert(ap);
  AtomSet ints(pc);
  
  Tuple in;
  in.push_back(Term(shop));
  in.push_back(Term("plusC"));
  in.push_back(Term(""));
  in.push_back(Term(""));
  in.push_back(Term(""));
  in.push_back(Term(""));

  Tuple out;
  RacerKBManager kb(rsIO, "DEFAULT");

  QueryCtx::shared_pointer q(new QueryCtx(PluginAtom::Query(ints, in, out),kb));
  
  CPPUNIT_ASSERT_NO_THROW( q = pcd.query(q) );
}
    
void
TestRacerDirector::runRacerAllIndividualsTest()
{
  TCPIOStream rsIO("localhost", 8088);
  
  QueryDirector<RacerFunAdapterBuilder<RacerAllIndividualsCmd>,RacerAnswerDriver> aiq(rsIO);
  RacerKBManager kb(rsIO, "DEFAULT");
  DLQuery::shared_pointer dlq(new DLQuery(Ontology::shared_pointer(),AtomSet(),Tuple()));
  QueryCtx::shared_pointer q(new QueryCtx(new Query(kb,dlq,
						    Term(""),Term(""),Term(""),Term(""),
						    AtomSet()),
					  new Answer(0))
			     );
  
  CPPUNIT_ASSERT_NO_THROW( q = aiq.query(q) );
  CPPUNIT_ASSERT(q->getAnswer().getTuples()->size() > 0);
  
  boost::shared_ptr<std::vector<Tuple> > tv = q->getAnswer().getTuples();
  output(*tv);
}


// Local Variables:
// mode: C++
// End:
