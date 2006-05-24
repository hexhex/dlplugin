dnl
dnl AM_PATH_ACE(MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
dnl
AC_DEFUN([AM_PATH_ACE],
[

AC_ARG_WITH([ace-include-prefix],
            [  --with-ace-include-prefix=PFX   Prefix where ACE headers are installed (optional)],
            [ACE_CFLAGS="-I$withval"],
            [ACE_CFLAGS="-I/usr/include"])
AC_ARG_WITH([ace-lib-prefix],
            [  --with-ace-lib-prefix=PFX   Prefix where ACE libs are installed (optional)],
            [ACE_LIBS="-L$withval -lACE"],
            [ACE_LIBS="-L/usr/lib -lACE"])


dnl override variables if ACE_ROOT variable is set
AC_MSG_CHECKING([for ACE_ROOT environment])

if test "x$ACE_ROOT" = x ; then
   AC_MSG_RESULT([no])
else
   AC_MSG_RESULT([yes])
   ACE_CPPFLAGS="-I$ACE_ROOT"
   ACE_LIBS="-L$ACE_ROOT/ace -lACE"
fi


no_ace=""

old_CPPFLAGS=$CPPFLAGS
old_LIBS=$LIBS

AC_LANG_PUSH([C++])

CPPFLAGS=$ACE_CFLAGS
LIBS=$ACE_LIBS

AC_CHECK_HEADER([ace/ACE.h], [], [no_ace="yes"], [])
AC_CHECK_LIB([ACE], [main], [], [no_ace="yes"], [])

CPPFLAGS=$old_CPPFLAGS
LIBS=$old_LIBS

AC_LANG_POP([C++])

HAVE_ACE="no"

if test "x$no_ace" = x ; then
   HAVE_ACE="yes"
   ifelse([$2], [], [:], [$2])     
else
   ACE_CPPFLAGS=""
   ACE_LIBS=""
   ifelse([$3], [], [:], [$3])
fi

AC_SUBST([HAVE_ACE])
AC_SUBST([ACE_CFLAGS])
AC_SUBST([ACE_LIBS])
])



