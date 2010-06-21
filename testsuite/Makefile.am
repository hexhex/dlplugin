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

DLVHEX_ABS_TOP_BUILDDIR=`readlink -f $(top_builddir)`
DLVHEX_ABS_TOP_SRCDIR=`readlink -f $(top_srcdir)`
TESTS_ENVIRONMENT = EXAMPLES=$(DLVHEX_ABS_TOP_SRCDIR)/examples DLVHEX=dlvhex PARAMETERS="-s -a --keepnsprefix --plugindir=$(DLVHEX_ABS_TOP_BUILDDIR)/src/"

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

# the order is very important here!
# libtool puts the path to the first library first, so
# if you have an older version of the DLVHEX_LIBS where the CPPUNIT_LIBS are, the wrong one will be used
TestSuite_LDFLAGS = $(DLVHEX_LIBS) $(CPPUNIT_LIBS)

TestSuite_LDADD = $(top_builddir)/src/libdlvhexracer.la
