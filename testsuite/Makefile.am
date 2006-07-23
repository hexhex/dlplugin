AUTOMAKE_OPTIONS = foreign

noinst_HEADERS = \
                 TestDLRewriter.h \
                 TestOWLParser.h \
                 TestRacerBuilder.h \
                 TestRacerDirector.h \
                 TestRacerInterface.h \
                 TestRacerNRQL.h \
                 TestRacerParse.h \
                 TestRacerQuery.h \
                 TestRacerRunner.h \
                 TestRacerStream.h \
                 TestRacerTypes.h \
                 TestSuite.h

EXTRA_DIST = run-dlvhex-tests.sh

TESTS = TestSuite run-dlvhex-tests.sh

TESTS_ENVIRONMENT = EXAMPLES=$(top_srcdir)/examples

check_PROGRAMS = TestSuite

TestSuite_SOURCES = \
                    TestDLRewriter.cpp \
                    TestOWLParser.cpp \
                    TestRacerBuilder.cpp \
                    TestRacerDirector.cpp \
                    TestRacerInterface.cpp \
                    TestRacerNRQL.cpp \
                    TestRacerParse.cpp \
                    TestRacerQuery.cpp \
                    TestRacerRunner.cpp \
                    TestRacerStream.cpp \
                    TestRacerTypes.cpp \
                    TestSuite.cpp

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(ACE_CPPFLAGS) \
	$(RAPTOR_CFLAGS) \
	$(DLVHEX_CFLAGS)

TestSuite_CXXFLAGS = $(CPPUNIT_CFLAGS)

TestSuite_LDFLAGS = $(CPPUNIT_LIBS) $(DLVHEX_LIBS)

TestSuite_LDADD = $(top_builddir)/src/libdlvhexracer.la
