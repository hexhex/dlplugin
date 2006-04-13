AC_PREREQ([2.59])
AC_INIT([dlvhex-racerplugin],[0.1],[tkren@kr.tuwien.ac.at])
AC_CONFIG_SRCDIR([src/RacerRunner.cpp])
AC_CONFIG_HEADER([src/config.h])



#
# the user didn't supply a path for Racer
#
if test "$RACER" = ""; then
    # we assume "RacerPro"
    RACER=RacerPro
    # and check if and where it exists
    AC_PATH_PROG(RACERPATH, $RACER, notfound)
else
    # the user supplied a 'RACER=...'
    # check if it exists (with search path=/, since the user supplied
    # an entire path!) and set RACERPATH to it!
    AC_CHECK_PROG(RACERPATH, $RACER, $RACER, notfound, [/])
fi

if test "$RACERPATH" = notfound; then
    AC_MSG_ERROR([Racer not found. Download it at http://www.racer-systems.com])
fi

#
# put the racer path into config.h
#
AC_DEFINE_UNQUOTED(RACERPATH,"$RACERPATH",[Path of RACER executable])


AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_LEX
AC_PROG_YACC


# Checks for libraries.

# Checks for header files.
AC_HEADER_DIRENT
AC_CHECK_HEADERS([sys/param.h])

AC_LANG_PUSH([C++])
AC_CHECK_HEADER([FlexLexer.h])
AC_LANG_POP([C++])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST

# Checks for library functions.
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


# Checks for external libraries
AM_PATH_CPPUNIT([1.10.2])
AC_PATH_BOOST
AM_PATH_ACE

# we need 1.4.8 for namespace parsing support
PKG_CHECK_MODULES([RAPTOR], [raptor >= 1.4.8])

PKG_CHECK_MODULES([DLVHEX], [dlvhex >= 0.1])

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
