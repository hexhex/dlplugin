AUTOMAKE_OPTIONS = foreign
MAINTAINERCLEANFILES = \
RacerAnswerParser.hpp \
RacerAnswerParser.output \
location.hh \
position.hh \
stack.hh

dlvhexlib_LTLIBRARIES = libdlvhexracer.la
libdlvhexracer_la_SOURCES = \
OWLParser.cpp \
RacerAnswerParser.ypp \
RacerAnswerScanner.lpp \
RacerAnswerDriver.cpp \
RacerBuilder.cpp \
RacerDirector.cpp \
RacerExtAtom.cpp \
RacerInterface.cpp \
RacerParse.cpp \
RacerRunner.cpp \
RacerQuery.cpp \
TCPStream.cpp \
UserDir.cpp

# don't let automake rename and move the output files of flex/bison,
# this doesn't work for pure C++ parser (yet)
.lpp.cpp:
	$(LEXCOMPILE) -o $@ $<
.ypp.cpp:
	$(YACCCOMPILE) -o $@ $<

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src \
	$(ACE_CPPFLAGS) \
	$(RAPTOR_CFLAGS) \
	$(DLVHEX_CFLAGS)

libdlvhexracer_la_LDFLAGS = $(ACE_LIBS) $(RAPTOR_LIBS) $(DLVHEX_LIBS)
