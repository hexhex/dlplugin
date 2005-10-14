AC_PREREQ([2.59])
AC_INIT([dlvhex-racerplugin],[0.1],[tkren@kr.tuwien.ac.at])
AC_CONFIG_SRCDIR([src/RacerRunner.cpp])
AC_CONFIG_HEADER([src/config.h])

if test "$RACERPATH" = ""; then
    RACERPATH=RacerPro
fi
    
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

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST

# Checks for library functions.
AC_PROG_LIBTOOL

# Checks for external libraries
AM_PATH_CPPUNIT([1.10.2])

AM_PATH_ACE

PKG_CHECK_MODULES([RAPTOR], [raptor >= 1.4.7])

PKG_CHECK_MODULES([DLVHEX], [dlvhex >= 0.1])
dlvhexlibdir=`pkg-config dlvhex --variable=pkglibdir`
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
