AM_CXXFLAGS = -I../src -Wall -Werror -DDEBUG

lib_LTLIBRARIES = libuni-stringify.la \
		libuni-container.la

libuni_stringify_la_LIBADD = ../src/libuni.la
libuni_stringify_la_SOURCES = stringify.cc

libuni_container_la_LIBADD = ../src/libuni.la
libuni_container_la_SOURCES = container.cc
