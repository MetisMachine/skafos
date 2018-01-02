.PHONY: test build version

VERSION_FILE=VERSION
VERSION=`cat $(VERSION_FILE)`
BUILD_DIR=_build

all: build

_start_release:
	@echo "Creating release for version $(VERSION)..."

_tag_release:
	@git tag -a $(VERSION) -m "Creating release for version $(VERSION)"
	@git push --tags

_create_version_h:
	@rm -rf src/version.h && \
	printf "#ifndef __CLI_VERSION__\n#define __CLI_VERSION__\n\n#define VERSION \"$(VERSION)\"\n\n#endif\n\n" >> src/version.h

_env_for_staging:
	@cp build_envs/staging_env.h src/cli_env.h

_env_for_prod:
	@cp build_envs/production_env.h src/cli_env.h

build: clean _create_version_h _env_for_staging
	@cd $(BUILD_DIR) \
	&& echo "Building skafos..." \
	&& cmake .. -Wno-dev \
	&& make

build_dist: clean _create_version_h _env_for_prod
	@cd $(BUILD_DIR) \
	&& echo "Building skafos..." \
	&& cmake .. -Wno-dev \
	&& make	\
	&& cpack --config CPackConfig.cmake

clean:
	@rm -rf _build/* 


release: _start_release clean build_dist


