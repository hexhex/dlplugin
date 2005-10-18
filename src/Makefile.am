AUTOMAKE_OPTIONS = foreign

dlvhexlib_LTLIBRARIES = libdlvhexracer.la
libdlvhexracer_la_SOURCES = \
OWLParser.cpp \
RacerBuilder.cpp \
RacerDirector.cpp \
RacerExtAtom.cpp \
RacerInterface.cpp \
RacerParse.cpp \
RacerRunner.cpp \
RacerQuery.cpp \
TCPStream.cpp \
UserDir.cpp

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(ACE_CPPFLAGS) \
	$(RAPTOR_CFLAGS) \
	$(DLVHEX_CFLAGS)

libdlvhexracer_la_LDFLAGS = $(ACE_LIBS) $(RAPTOR_LIBS) $(DLVHEX_LIBS)
