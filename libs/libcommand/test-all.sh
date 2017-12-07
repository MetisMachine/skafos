#!/bin/sh

if [ ! -f configure ] ; then
    ./autogen.sh
fi

CXXFLAGS='--coverage -O0' ./configure

make check
cd tests; make cov