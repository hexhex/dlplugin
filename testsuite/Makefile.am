noinst_HEADERS = \
                 TestCache.h \
                 TestDLOptimizer.h \
                 TestDLRewriter.h \
                 TestOWLParser.h \
                 TestRacerBuilder.h \
                 TestRacerDirector.h \
                 TestRacerInterface.h \
                 TestRacerNRQL.h \
                 TestRacerParse.h \
                 TestRacerQuery.h \
                 TestRacerStream.h \
                 TestRacerTypes.h \
                 TestSuite.h

EXTRA_DIST = run-tests.sh compare_answersets_plain.py RacerRunner.sh

TESTS = TestSuite run-tests.sh

TESTS_ENVIRONMENT = EXAMPLES=$(CURDIR)/$(top_srcdir)/examples TESTDIR=$(CURDIR)/$(top_srcdir)/examples DLVHEX=dlvhex PARAMETERS="-s -a --keepnsprefix --plugindir=$(CURDIR)/$(top_builddir)/src/"

check_PROGRAMS = TestSuite

TestSuite_SOURCES = \
                    TestCache.cpp \
                    TestDLOptimizer.cpp \
                    TestDLRewriter.cpp \
                    TestOWLParser.cpp \
                    TestRacerBuilder.cpp \
                    TestRacerDirector.cpp \
                    TestRacerInterface.cpp \
                    TestRacerNRQL.cpp \
                    TestRacerParse.cpp \
                    TestRacerQuery.cpp \
                    TestRacerStream.cpp \
                    TestRacerTypes.cpp \
                    TestSuite.cpp


AM_CPPFLAGS = \
	-DDLPLUGIN='"$(top_builddir)/src/.libs/libdlvhexracer.so"' \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src \
	$(BOOST_CPPFLAGS) \
	$(RAPTOR_CFLAGS) \
	$(DLVHEX_CFLAGS)

TestSuite_CXXFLAGS = $(CPPUNIT_CFLAGS)

TestSuite_LDFLAGS = $(CPPUNIT_LIBS) $(DLVHEX_LIBS)

TestSuite_LDADD = $(top_builddir)/src/libdlvhexracer.la
