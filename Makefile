.PHONY: test build version

VERSION_FILE=VERSION
VERSION=`cat $(VERSION_FILE)`
BUILD_DIR=_build

all: clean build

build: version
	@cd $(BUILD_DIR) \
	&& echo "Building skafos..." \
	&& cmake .. -Wno-dev \
	&& make

version:
	@rm -rf src/version.h && \
	echo "#define VERSION \"$(VERSION)\"" >> src/version.h

clean:
	@rm -rf _build/* 

run:
	@./_build/bin/skafos new my_project
