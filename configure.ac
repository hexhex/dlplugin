AC_PREREQ([2.59])
AC_INIT([dlvhex-racerplugin],[0.3.0],[tkren@kr.tuwien.ac.at])
AC_CONFIG_SRCDIR([src/RacerInterface.cpp])
AC_CONFIG_HEADER([src/config.h])

MAJOR=`echo $PACKAGE_VERSION | sed "s,^\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\),\1,"`
MINOR=`echo $PACKAGE_VERSION | sed "s,^\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\),\2,"`
MICRO=`echo $PACKAGE_VERSION | sed "s,^\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\),\3,"`

AC_DEFINE_UNQUOTED([DLPLUGIN_MAJOR], [$MAJOR], [Major version number of the dl-plugin])
AC_DEFINE_UNQUOTED([DLPLUGIN_MINOR], [$MINOR], [Minor version number of the dl-plugin])
AC_DEFINE_UNQUOTED([DLPLUGIN_MICRO], [$MICRO], [Micro version number of the dl-plugin])

#
# the user didn't supply a path for Racer
#
if test "$RACER" = ""; then
    # we assume "RacerPro"
    RACER=RacerPro
    # and check if and where it exists
    AC_PATH_PROG([RACERPATH], [$RACER], [notfound])
else
    # the user supplied a 'RACER=...'
    # check if it exists (with search path=/, since the user supplied
    # an entire path!) and set RACERPATH to it!
    AC_CHECK_PROG([RACERPATH], [$RACER], [$RACER], [notfound], [/])
fi

if test "$RACERPATH" = notfound; then
    AC_MSG_ERROR([Racer not found. Download it at http://www.racer-systems.com/])
fi


AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_LEX
AC_PROG_YACC
AC_PROG_LN_S


# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([sys/select.h sys/socket.h sys/stat.h sys/time.h sys/types.h unistd.h])
AC_HEADER_STAT
AC_HEADER_STDC
AC_HEADER_TIME

#
# search for FlexLexer.h so we can add it at "make dist" time
#
if test -f $top_builddir/src/FlexLexer.h ; then : ; else

   AC_LANG_PUSH([C++])
   AC_CHECK_HEADER([FlexLexer.h])
   AC_LANG_POP([C++])

   AC_MSG_CHECKING([for FlexLexer.h path])

   include_paths="/usr/include /usr/local/include" # TODO: get default include paths from compiler?
   for f in "$CPPFLAGS $CXXFLAGS $CFLAGS" ; do
   case $f in
   -I*) include_paths="$include_paths $f" ;;
   esac
   done

   # get rid of -I compiler flags
   paths=`echo $include_paths | sed s,-I,,g`

   # get the FIRST occurence of FlexLexer.h
   FLEXLEXER_H=`find $paths -name FlexLexer.h 2>/dev/null | head -n1`

   AC_MSG_RESULT([$FLEXLEXER_H])

   if test -z "$FLEXLEXER_H" ; then AC_MSG_ERROR([Could not find FlexLexer.h]) fi

   # link FlexLexer.h to the src directory at the end of the configuration phase
   AC_SUBST([FLEXLEXER_H])

fi




# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST

# Checks for libtool library functions.
AC_LIBTOOL_WIN32_DLL # enable win32 dll-building support
AC_LIBTOOL_DLOPEN # we build a module library
AC_DISABLE_STATIC # we don't need a static library
AC_PROG_LIBTOOL

# doxygen stuff
#
# these are the defaults, can be overridden by --enable-doxygen-...
DX_HTML_FEATURE([ON])
DX_CHM_FEATURE([OFF])
DX_CHI_FEATURE([OFF])
DX_MAN_FEATURE([OFF])
DX_RTF_FEATURE([OFF])
DX_XML_FEATURE([OFF])
DX_PDF_FEATURE([OFF])
DX_PS_FEATURE([ON])
DX_INIT_DOXYGEN([dlvhex-racerplugin], [doxygen.cfg], [documentation])

#
# Checks for external libraries
#

AM_PATH_CPPUNIT([1.10.2])
AC_PATH_BOOST

# in case we couldn't find ACE.pc, fall back to the old scheme
#PKG_CHECK_MODULES([ACE], [ACE >= 5.5.1], [:],
#			 [echo $ACE_PKG_ERROR
#			 AM_PATH_ACE
#			 if test "$HAVE_ACE" != "yes" ; then
#			    AC_MSG_ERROR([ACE not found. Download it at http://www.cs.wustl.edu/~schmidt/ACE.html])
#			 fi])

# we need 1.4.8 for namespace parsing support
PKG_CHECK_MODULES([RAPTOR], [raptor >= 1.4.8])

# we want dlvhex
PKG_CHECK_MODULES([DLVHEX], [dlvhex >= 1.2])


AC_ARG_ENABLE(userinstall, 
             [  --enable-userinstall    Install plugin into home directory],
             [dlvhexlibdir=$HOME/`pkg-config dlvhex --variable=userplugindir`],
             [dlvhexlibdir=`pkg-config dlvhex --variable=sysplugindir`]
             )
AC_SUBST([dlvhexlibdir])


#
# set system-wide plugin dir
#
CPPFLAGS="$CPPFLAGS -DPLUGIN_DIR='\"\$(dlvhexlibdir)\"'"

AC_CONFIG_FILES([
	Makefile
	examples/Makefile
	include/Makefile
	src/Makefile
	testsuite/Makefile
])

AC_OUTPUT
