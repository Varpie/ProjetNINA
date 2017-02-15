AC_PREREQ([2.69])
AC_INIT([nina], [0.1], [])
AM_INIT_AUTOMAKE

AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB


AC_CHECK_PROG(PIP_CHECK,pip,yes)
if test x"$PIP_CHECK" != x"yes" ; then
    AC_MSG_ERROR([Please install python-pip])
fi
AC_CHECK_HEADER([X11/Xlib.h],[],[AC_MSG_ERROR([Please install libx11-dev])])
AC_CHECK_HEADER([python2.7/Python.h],[],[AC_MSG_ERROR([Please install python-dev])])

AC_CONFIG_COMMANDS_PRE([./python_dependencies.sh])

CXXFLAGS=-std=c++14

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
