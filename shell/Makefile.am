AM_LFLAGS = -olex.yy.c
AM_CXXFLAGS = -g -I../src -Wall -Werror -Wno-unused-label -Wno-unused-function -DDEBUG

bin_PROGRAMS = uni

uni_LDADD = ../src/libuni.la
uni_SOURCES =	clo.h \
		clo.cxx \
		importer.hh \
		importer.cc \
		main.cc \
		dot_view_handler.hh \
		dot_view_handler.cc \
		my_view_handler.hh \
		my_view_handler.cc
