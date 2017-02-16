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


AC_CHECK_PROG([PIP_CHECK],[pip],[yes])
AC_CHECK_HEADER([X11/Xlib.h],[X11_CHECK=yes],[X11_CHECK=no])
AC_CHECK_HEADER([python2.7/Python.h],[PYDEV_CHECK=yes],[PYDEC_CHECK=no])
message="Missing package(s) :"
flag=0
if test x"$X11_CHECK" != x"yes" ; then
  message+=" libx11-dev"
  flag=1
fi
if test x"$PYDEV_CHECK" != x"yes" ; then
  message+=" python-dev"
  flag=1
fi
if test x"$PIP_CHECK" != x"yes" ; then
  message+=" python-pip"
  flag=1
fi
if test x"$flag" == x"1" ; then
  AC_MSG_ERROR(["$message"])
fi

AC_CONFIG_COMMANDS_PRE([sudo ./scripts/python_dependencies.sh])
AC_CONFIG_COMMANDS_PRE([sudo ./scripts/setup_python.sh])

CXXFLAGS=-std=c++14

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
