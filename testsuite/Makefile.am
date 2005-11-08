AUTOMAKE_OPTIONS = foreign

noinst_HEADERS = \
                 TestOWLParser.h \
                 TestRacerBuilder.h \
                 TestRacerDirector.h \
                 TestRacerInterface.h \
                 TestRacerParse.h \
                 TestRacerQuery.h \
                 TestRacerRunner.h \
                 TestRacerStream.h \
                 TestRacerTypes.h


TESTS = TestAll

TESTS_ENVIRONMENT = EXAMPLES=$(top_srcdir)/examples

check_PROGRAMS = $(TESTS)

TestAll_SOURCES = TestAll.cpp

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(ACE_CPPFLAGS) \
	$(RAPTOR_CFLAGS) \
	$(DLVHEX_CFLAGS)

TestAll_CXXFLAGS = $(CPPUNIT_CFLAGS)

# build statically so we don't depend on an installed libdlvhexracer
TestAll_LDFLAGS = -static $(CPPUNIT_LIBS) $(DLVHEX_LIBS)

TestAll_LDADD = $(top_builddir)/src/libdlvhexracer.la