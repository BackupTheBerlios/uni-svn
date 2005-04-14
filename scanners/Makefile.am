AM_LFLAGS = -olex.yy.c
AM_CXXFLAGS = -g -I../src -Wall -Werror -Wno-unused-label -Wno-unused-function -DDEBUG

lib_LTLIBRARIES = libuni-en-ascii.la \
		  libuni-curly-ascii.la

libuni_en_ascii_la_LIBADD = ../src/libuni.la
libuni_en_ascii_la_SOURCES = en-ascii.ll en-ascii.hh

libuni_curly_ascii_la_LIBADD = ../src/libuni.la
libuni_curly_ascii_la_SOURCES = curly-ascii.ll curly-ascii.hh
