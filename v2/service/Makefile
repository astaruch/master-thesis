# Determine this makefile's path.
# Be sure to place this BEFORE `include` directives, if any.
THIS_FILE := $(lastword $(MAKEFILE_LIST))
include .env

build:
	@echo $@  # print target name
	@$(MAKE) -f $(THIS_FILE) build-debug # invoke other target

run: build
	@$(MAKE) -f $(THIS_FILE) all-features

test: build-tests
	@echo $@
	Debug/test/all_tests

ctest: build-tests
	@echo $@
	cd Debug && ctest

clean:
	@echo $@
	rm -rf Debug Release build

build-tests:
	@echo $@ # print target name
	mkdir -p Debug
	cd Debug && cmake \
		-D CMAKE_BUILD_TYPE:STRING=Debug \
		-D CMAKE_TOOLCHAIN_FILE:STRING=${CMAKE_VCPKG_TOOLCHAIN} \
		-D RUN_TESTS:BOOL=ON \
		..
	cd Debug && \
		make -j 4

build-debug:
	@echo $@ # print target name
	mkdir -p Debug
	cd Debug && cmake \
		-D CMAKE_BUILD_TYPE:STRING=Debug \
		-D CMAKE_TOOLCHAIN_FILE:STRING=${CMAKE_VCPKG_TOOLCHAIN} \
		-D RUN_TESTS:BOOL=OFF \
		..
	cd Debug && \
		make -j 4

f1: build-debug
	Debug/phishsvc \
		--enable-training-data \
		--td-url "https://www.google.com" \
		--td-class-value 0 \
		--enable-features \
		--feat-hsts

url-features: build-debug
	Debug/phishsvc \
		--enable-training-data \
		--td-stdin \
		--verbose \
		--td-class-value 0 \
		--enable-features \
		${URL_FEATURES}

html-features: build-debug
	Debug/phishsvc \
		--enable-training-data \
		--td-stdin \
		--verbose \
		--td-class-value 0 \
		--enable-features \
		${HTML_FEATURES} \
		--node-bin ${NODE_BIN} \
		--html-script ${HTML_FEATURES_SCRIPT}

host-based-features: build-debug
	Debug/phishsvc \
		--enable-training-data \
		--verbose \
		--td-stdin \
		--td-class-value 0 \
		--enable-features \
		${HOST_BASED_FEATURES}

all-features: build-debug
	Debug/phishsvc \
		--enable-training-data \
		--td-stdin \
		--verbose \
		--td-class-value 0 \
		--enable-features \
		${URL_FEATURES} \
		${HTML_FEATURES} \
		--node-bin ${NODE_BIN} \
		--html-script ${HTML_FEATURES_SCRIPT} \
		${HOST_BASED_FEATURES}

valgrind: build-debug
	valgrind --tool=memcheck --leak-check=yes Debug/phishsvc \
		--enable-training-data \
		--td-url "http://google.com" \
		--td-class-value 0 \
		--enable-features \
		${URL_FEATURES}
