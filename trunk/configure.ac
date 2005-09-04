#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([uni], [0.3], [chentianran@mail.berlios.de], [uni])
AC_CONFIG_AUX_DIR([./config])
AC_CONFIG_SRCDIR([src/term.hh])
AM_INIT_AUTOMAKE

AC_PREREQ(2.57)

AC_LIBTOOL_DLOPEN

AC_PROG_CXX
AM_PROG_LEX
AC_PROG_YACC
AC_PROG_LIBTOOL

AC_LANG(C++)

AC_CHECK_LIB(curses,main,,[AC_MSG_ERROR([Curses not found.])])
AC_CHECK_LIB(readline,main,,[AC_MSG_ERROR([GNU Readline not found.])])
AC_CHECK_LIB(dl,dlopen,,[AC_MSG_ERROR([libdl not found.])])

AC_HEADER_STDBOOL
AC_C_CONST

AC_CONFIG_FILES([
Makefile
src/Makefile
shell/Makefile
filter/Makefile
scanners/Makefile
utils/Makefile
doc/Makefile
])

AC_OUTPUT
