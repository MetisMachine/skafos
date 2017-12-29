.PHONY: test build version

VERSION_FILE=VERSION
VERSION=`cat $(VERSION_FILE)`
BUILD_DIR=_build

all: build

build: version
	@cd $(BUILD_DIR) \
	&& echo "Building skafos..." \
	&& cmake .. -Wno-dev \
	&& make

version:
	@rm -rf src/version.h && \
	echo "#ifndef __CLI_VERSION__\n#define __CLI_VERSION__\n\n#define VERSION \"$(VERSION)\"\n\n#endif\n\n" >> src/version.h

clean:
	@rm -rf _build/* 

_start_release:
	@echo "Creating release for version $(VERSION)..."

_tag_release:
	@git tag -a $(VERSION) -m "Creating release for version $(VERSION)"
	@git push --tags

release: _start_release clean build _tag_release


