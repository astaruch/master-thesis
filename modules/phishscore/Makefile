# Determine this makefile's path.
# Be sure to place this BEFORE `include` directives, if any.
THIS_FILE := $(lastword $(MAKEFILE_LIST))
include .env

build:
	@echo $@  # print target name
	@$(MAKE) -f $(THIS_FILE) build-debug # invoke other target

run: build
	@$(MAKE) -f $(THIS_FILE) all-features

check-url: build-debug
	@echo $@
	Debug/phishsvc --check-url 'http://google.com23' \
		--model-checker-port ${THESIS_MODEL_CHECKER_PORT} \
		--html-analysis-port ${THESIS_HTML_ANALYSIS_PORT} \
		--verbose

service: build-debug
	@echo $@
	Debug/phishsvc --server ${THESIS_PHISHSCORE_PORT} \
		--html-analysis-port ${THESIS_HTML_ANALYSIS_PORT} \
		--model-checker-port ${THESIS_MODEL_CHECKER_PORT} \
		--enable-model-checking \
		--verbose

test: build-tests
	@echo $@
	Debug/test/all_tests

test-fail: build-tests
	@echo $@
	Debug/test/all_tests --gtest_break_on_failure

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
		--td-class-value 0 \
		--enable-features \
		--td-output-url \
		${URL_FEATURES}

html-features: build-debug
	Debug/phishsvc \
		--enable-training-data \
		--td-stdin \
		--td-class-value 0 \
		--enable-features \
		--td-output-url \
		--td-output-extra-values \
		${HTML_FEATURES} \
		--td-html-analysis-path ${THESIS_HTML_ANALYSIS_PROG}

host-based-features: build-debug
	Debug/phishsvc \
		--enable-training-data \
		--td-stdin \
		--td-class-value 0 \
		--enable-features \
		--td-output-extra-values \
		--td-output-url \
		${HOST_BASED_FEATURES}

url-host: build-debug
	Debug/phishsvc \
		--enable-training-data \
		--td-stdin \
		--td-class-value 0 \
		--enable-features \
		--td-output-extra-values \
		--td-output-url \
		${URL_FEATURES} \
		${HOST_BASED_FEATURES}

check-model: build-debug
	Debug/phishsvc \
		--enable-model-checking \
		--stdin \
		--mc-html-analysis-path ${THESIS_HTML_ANALYSIS_PROG} \
		--mc-model-checker-path ${THESIS_MODEL_CHECKER_PROG} \

all-features: build-debug
	Debug/phishsvc \
		--enable-training-data \
		--verbose \
		--td-stdin \
		--td-class-value 0 \
		--enable-features \
		--td-output-url \
		--td-html-analysis-path ${THESIS_HTML_ANALYSIS_PROG} \
		${URL_FEATURES} \
		${HTML_FEATURES} \
		${HOST_BASED_FEATURES}

valgrind: build-debug
	valgrind --tool=memcheck --leak-check=yes Debug/phishsvc \
		--enable-training-data \
		--td-url "http://google.com" \
		--td-class-value 0 \
		--enable-features \
		${URL_FEATURES}
