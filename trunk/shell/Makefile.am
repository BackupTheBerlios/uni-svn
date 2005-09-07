AM_LFLAGS = -olex.yy.c
AM_CXXFLAGS = -g -I../src -I../utils -Wall -Werror -DDEBUG

bin_PROGRAMS = uni

uni_LDADD = ../src/libuni.la ../utils/libuni-dlimport.la
uni_SOURCES =	clo.h \
		clo.cxx \
		debugger.hh \
		debugger.cc \
		importer.hh \
		importer.cc \
		main.cc
