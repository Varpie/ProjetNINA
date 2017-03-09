AC_PREREQ([2.69])
AC_INIT([nina], [1.0], [])
AM_INIT_AUTOMAKE([1.15 foreign])

AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL

AC_CHECK_PROG([PIP_CHECK],[pip],[yes])
AC_CHECK_HEADER([X11/Xlib.h],[X11_CHECK=yes],[X11_CHECK=no])
AC_CHECK_HEADER([X11/extensions/XInput.h],[XI_CHECK=yes],[XI_CHECK=no])
AC_CHECK_HEADER([python2.7/Python.h],[PYDEV_CHECK=yes],[PYDEC_CHECK=no])
AC_CHECK_PROG([PY2_CHECK],[python2.7],[yes])

message="Missing package(s) :"
flag=0
if test x"$PY2_CHECK" != x"yes" ; then
  message+=" python2.7"
  flag=1
fi
if test x"$X11_CHECK" != x"yes" ; then
  message+=" libx11-dev"
  flag=1
fi
if test x"$XI_CHECK" != x"yes" ; then
  message+=" libxi-dev"
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
AC_CONFIG_COMMANDS_PRE([./scripts/python_dependencies.sh])

CURRENT_DIR=$PWD
AC_SUBST(CURRENT_DIR)

AM_PATH_PYTHON([2.7])
CXXFLAGS=-std=c++14
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
