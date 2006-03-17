AUTOMAKE_OPTIONS = foreign

dlvhexlib_LTLIBRARIES = libdlvhexracer.la
libdlvhexracer_la_SOURCES = \
RacerAnswerParser.ypp \
RacerAnswerScanner.lpp \
OWLParser.cpp \
RacerBuilder.cpp \
RacerDirector.cpp \
RacerExtAtom.cpp \
RacerInterface.cpp \
RacerParse.cpp \
RacerRunner.cpp \
RacerQuery.cpp \
TCPStream.cpp \
UserDir.cpp \
RacerAnswerDriver.cpp

# bison flags
AM_YFLAGS = -d

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src \
	$(ACE_CPPFLAGS) \
	$(RAPTOR_CFLAGS) \
	$(DLVHEX_CFLAGS)

libdlvhexracer_la_LDFLAGS = $(ACE_LIBS) $(RAPTOR_LIBS) $(DLVHEX_LIBS)
