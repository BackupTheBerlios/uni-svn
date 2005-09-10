AM_CXXFLAGS = -I. -I../src -Wall -Werror -DDEBUG

lib_LTLIBRARIES = libuni-stringify.la \
		libuni-container.la \
		libuni-dlimport.la \
		libuni-import.la

libuni_stringify_la_LIBADD = ../src/libuni.la
libuni_stringify_la_SOURCES = stringify.cc

libuni_container_la_LIBADD = ../src/libuni.la
libuni_container_la_SOURCES = container.cc

libuni_dlimport_la_LIBADD = ../src/libuni.la
libuni_dlimport_la_SOURCES = dlimport.cc

libuni_import_la_LIBADD = ../src/libuni.la
libuni_import_la_SOURCES = import.cc
