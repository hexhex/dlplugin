dlvhexlib_LTLIBRARIES = libdlvhexplugin_racer.la
libdlvhexplugin_racer_la_SOURCES = \
Answer.cpp \
AtomSeparator.cpp \
Cache.cpp \
DLQuery.cpp \
LogBuf.cpp \
OWLParser.cpp \
Ontology.cpp \
Query.cpp \
QueryCtx.cpp \
QueryDirector.cpp \
RacerAnswerDriver.cpp \
RacerBuilder.cpp \
RacerInterface.cpp \
RacerKBManager.cpp \
RacerNRQL.cpp \
RacerNRQLBuilder.cpp \
RacerQueryExpr.cpp \
Registry.cpp \
TCPStream.cpp \
URI.cpp \
Default.cpp \
Defaults.cpp \
DFProcessor.cpp \
DFOutputBuilder.cpp \
DLAtom.cpp \
DLAtoms.cpp \
DLRule.cpp \
DLRules.cpp \
Predicate.cpp \
Pred1Dim.cpp \
Pred2Dim.cpp \
Prefix.cpp \
Prefixes.cpp \
MTerm.cpp \
Terms.cpp \
Update.cpp \
Updates.cpp \
DFConverter.cpp \
DLOptimizer.cpp \
HexDLConverter.cpp \
HexDLRewriter.cpp

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src \
	$(BOOST_CPPFLAGS) \
	$(RAPTOR_CFLAGS) \
	$(DLVHEX_CFLAGS)

# -avoid-version: only create libdlvhexplugin_racer.so
# -module: library will be dlopened
# -no-undefined: windows dlls don't allow undefined external symbols
#libdlvhexracer_la_LDFLAGS = -avoid-version -module -no-undefined $(RAPTOR_LIBS)
libdlvhexplugin_racer_la_LDFLAGS = -avoid-version -module $(RAPTOR_LIBS)

#
# adapt path and dependency libraries as needed
#
RAPTOR_PATH = $(HOME)/local/lib
RAPTOR_STATIC_LIBS =  -lxml2 -lxslt -lcurl $(RAPTOR_PATH)/libraptor.a

libdlvhexplugin_racer-static.la: $(libdlvhexplugin_racer_la_OBJECTS)
	$(CXXLINK) -avoid-version -module -rpath $(dlvhexlibdir) $(libdlvhexplugin_racer_la_OBJECTS) $(RAPTOR_STATIC_LIBS)

install-static: libdlvhexplugin_racer-static.la
	$(LIBTOOL) --mode=install $(INSTALL) -s libdlvhexplugin_racer-static.la $(DESTDIR)$(dlvhexlibdir)/libdlvhexplugin_racer-static.la

uninstall-static:
	$(LIBTOOL) --mode=uninstall rm -f $(DESTDIR)$(dlvhexlibdir)/libdlvhexplugin_racer-static.la
