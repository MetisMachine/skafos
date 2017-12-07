AUTOMAKE_OPTIONS = gnu subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}
AM_CXXFLAGS=-std=c++11

MAINTAINERCLEANFILES = \
    aclocal.m4 \
    config.h.in \
    config.log \
    config.status \
    configure \
    configure.files \
    stamp-h.in \
    depcomp \
    install-sh \
    missing


CLEANFILES = \
    %.o \
    %.bak \
    %~ \
    doxyfile.stamp

SUBDIRS = include . tests examples docs

noinst_PROGRAMS = bin/command

dist_noinst_SCRIPTS = autogen.sh

nodist_bin_command_SOURCES = \
    src/main.cpp


bin_command_CPPFLAGS = \
    $(bin_command_CFLAGS) \
    -I$(top_srcdir)/include \
    -Wall -pedantic -Wextra
