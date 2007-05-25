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
 * @file   TestCache.cpp
 * @author Thomas Krennwallner
 * @date   Wed Aug 23 17:43:47 2006
 * 
 * @brief  Testcases for Cache.
 * 
 * 
 */


#include "Cache.h"
#include "KBManager.h"
#include "Answer.h"

#include "TestCache.h"


using namespace dlvhex::dl;
using namespace dlvhex::dl::test;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestCache);

void
TestCache::setUp()
{
  stats = new CacheStats;
  cache = new DebugCache(*stats);
}

void
TestCache::tearDown()
{
  std::cerr << *stats << std::endl;

  delete cache;
  delete stats;
}

void
TestCache::runBooleanCache()
{
  AtomSet ints;
  KBManager kb("DEFAULT");

  //
  // first, we simulate a positive answer
  //

  AtomPtr ap(new Atom("pc(q1,i1)"));
  ints.insert(ap);

  Tuple out(1,Term("i1"));
  DLQuery::shared_pointer dlq1(new DLQuery(Ontology::createOntology(shop),
					  Term("q1"),
					  out
					  )
			      );
  Query* q1 = new Query(kb,dlq1,
		       Term("pc"),Term(""),Term(""),Term(""),ints
		       );
  Answer* a1 = new Answer(q1);
  a1->setAnswer(true);
  a1->setTuples(std::vector<Tuple>(1,Tuple()));

  QueryCtx::shared_pointer qctx1(new QueryCtx(q1,a1));

  CPPUNIT_ASSERT(stats->hits() == 0);
  CPPUNIT_ASSERT(stats->miss() == 0);

  CPPUNIT_ASSERT(cache->cacheHit(qctx1) == QueryCtx::shared_pointer());

  CPPUNIT_ASSERT(stats->hits() == 0);
  CPPUNIT_ASSERT(stats->miss() == 1);

  cache->insert(qctx1);

  CPPUNIT_ASSERT(cache->cacheHit(qctx1) == qctx1);

  CPPUNIT_ASSERT(stats->hits() == 1);
  CPPUNIT_ASSERT(stats->miss() == 1);
  CPPUNIT_ASSERT(stats->qctxno() == 1);
  CPPUNIT_ASSERT(stats->dlqno() == 1);

  // we don't insert duplicates
  cache->insert(qctx1);

  CPPUNIT_ASSERT(stats->qctxno() == 1);
  CPPUNIT_ASSERT(stats->dlqno() == 1);

  //
  // and now we have a negative answer
  //

  ints.clear();

  Query* q2 = new Query(kb, dlq1,
			Term(""),Term(""),Term(""),Term(""),ints
			);
  Answer* a2 = new Answer(q2);
  a2->setAnswer(false);

  QueryCtx::shared_pointer qctx2(new QueryCtx(q2,a2));

  CPPUNIT_ASSERT(cache->cacheHit(qctx2) == QueryCtx::shared_pointer());

  CPPUNIT_ASSERT(stats->hits() == 1);
  CPPUNIT_ASSERT(stats->miss() == 2);

  cache->insert(qctx2);

  CPPUNIT_ASSERT(cache->cacheHit(qctx2) == qctx2);
  CPPUNIT_ASSERT(stats->hits() == 2);
  CPPUNIT_ASSERT(stats->miss() == 2);
  CPPUNIT_ASSERT(stats->qctxno() == 2);
  CPPUNIT_ASSERT(stats->dlqno() == 1);

  // no dups
  cache->insert(qctx2);
  CPPUNIT_ASSERT(stats->hits() == 2);
  CPPUNIT_ASSERT(stats->miss() == 2);
  CPPUNIT_ASSERT(stats->qctxno() == 2);
  CPPUNIT_ASSERT(stats->dlqno() == 1);

  
  //
  //
  //

  ints.clear();

  ap = AtomPtr(new Atom("pc(q1,i1)"));
  ints.insert(ap);
  ap = AtomPtr(new Atom("pc(q1,i2)"));
  ints.insert(ap);

  out.clear();
  out.push_back(Term("i2"));
  DLQuery::shared_pointer dlq3(new DLQuery(Ontology::createOntology(shop),
					   Term("q1"),
					   out
					  )
			      );

  Query* q3 = new Query(kb, dlq3,
			Term("pc"),Term(""),Term(""),Term(""),ints
			);
  Answer* a3 = new Answer(q3);
  a3->setAnswer(true);

  QueryCtx::shared_pointer qctx3(new QueryCtx(q3,a3));

  cache->insert(qctx3);


}


void
TestCache::runNonBooleanCache()
{
  //
  // and now for something completely different
  //

  AtomSet ints;
  AtomPtr ap;

  KBManager kb("DEFAULT");

  Tuple out(1,Term("X"));

  DLQuery::shared_pointer dlq1(new DLQuery(Ontology::createOntology(shop),
					   Term("q1"),
					   out
					  )
			      );
  Query* q1 = new Query(kb, dlq1,
			Term(""),Term(""),Term(""),Term(""),ints
			);
  Answer* a1 = new Answer(q1);


  QueryCtx::shared_pointer qctx1(new QueryCtx(q1,a1));

  cache->insert(qctx1);

  CPPUNIT_ASSERT(cache->cacheHit(qctx1) == qctx1);
  CPPUNIT_ASSERT(stats->hits() == 1);
  CPPUNIT_ASSERT(stats->miss() == 0);
  CPPUNIT_ASSERT(stats->qctxno() == 1);
  CPPUNIT_ASSERT(stats->dlqno() == 1);

  ap = AtomPtr(new Atom("pc(q1,i1)"));
  ints.insert(ap);
  ap = AtomPtr(new Atom("pc(q1,i2)"));
  ints.insert(ap);
  ap = AtomPtr(new Atom("pc(q1,i3)"));
  ints.insert(ap);

  Query* q2 = new Query(kb, dlq1,
			Term("pc"),Term(""),Term(""),Term(""),ints
			);
  Answer* a2 = new Answer(q2);

  QueryCtx::shared_pointer qctx2(new QueryCtx(q2,a2));

  cache->insert(qctx2);

  CPPUNIT_ASSERT(cache->cacheHit(qctx2) == qctx2);
  CPPUNIT_ASSERT(stats->hits() == 2);
  CPPUNIT_ASSERT(stats->miss() == 0);
  CPPUNIT_ASSERT(stats->qctxno() == 2);
  CPPUNIT_ASSERT(stats->dlqno() == 1);

  //
  // and now we try to get qctx2 from a fresh query
  // 

  Query* q3 = new Query(kb, dlq1,
			Term("pc"),Term(""),Term(""),Term(""),ints
			);
  Answer* a3 = new Answer(q3);

  QueryCtx::shared_pointer qctx3(new QueryCtx(q3,a3));

  CPPUNIT_ASSERT(cache->cacheHit(qctx3) == qctx2);
  CPPUNIT_ASSERT(stats->hits() == 3);
  CPPUNIT_ASSERT(stats->miss() == 0);
  CPPUNIT_ASSERT(stats->qctxno() == 2);
  CPPUNIT_ASSERT(stats->dlqno() == 1);

}


// Local Variables:
// mode: C++
// End:
