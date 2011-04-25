dnl Copyright (C) 2011  Trevor Woerner

AC_INIT([xannounce], 2.0.1, twoerner@gmail.com, xannounce)
AC_CONFIG_SRCDIR(src/xannounce.c)
AC_CONFIG_AUX_DIR(cfg)
AM_INIT_AUTOMAKE([foreign no-dist-gzip dist-bzip2 1.9])
AM_CONFIG_HEADER(cfg/config.h)

SUBDIRS="src"

dnl **********************************
dnl checks for programs
dnl **********************************
AC_PROG_CC
AC_PROG_CPP

AC_PROG_MAKE_SET

AC_PROG_INSTALL
AC_PROG_LN_S

AC_CHECK_PROG(strip, strip, strip, :)
AC_CHECK_PROG(doc_html, docbook2html, docbook2html, [true],)
AC_CHECK_PROG(doc_pdf,  docbook2pdf,  docbook2pdf,  [true],)
AC_CHECK_PROG(doc_txt,  docbook2txt,  docbook2txt,  [true],)

dnl **********************************
dnl checks for libraries
dnl **********************************
AC_CHECK_LIB(X11, XOpenDisplay,,AC_MSG_ERROR([Can not find X11]),)

dnl **********************************
dnl other stuff
dnl **********************************
AC_SUBST(SUBDIRS)

dnl **********************************
dnl output
dnl **********************************
AC_OUTPUT(Makefile
cfg/Makefile
src/Makefile)
