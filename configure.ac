#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])

AC_INIT([simLoco_cpp], [0.0.1])
AC_CONFIG_SRCDIR([src/main_drive.cc])
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX

# Checks for libraries.
PKG_CHECK_MODULES([CLUTTERMM], [cluttermm-1.0])
PKG_CHECK_MODULES([CLUTTER_GTKMM], [clutter-gtkmm-1.0])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_OUTPUT(Makefile src/Makefile)

