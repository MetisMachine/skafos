.PHONY: test build

VERSION_FILE=VERSION
VERSION=`cat $(VERSION_FILE)`
BUILD_DIR=_build

all: clean build

build:
	@cd $(BUILD_DIR) \
	&& echo "Building skafos..." \
	&& cmake .. \
	&& make

clean:
	@rm -rf _build/*

run:
	@./_build/bin/skafos new my_project
