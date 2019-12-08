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
		--model-checker-port 13000 \
		--html-analysis-port 12000

service: build-debug
	@echo $@
	Debug/phishsvc --mc-input-url 'http://google.com' \
		--html-analysis-port 12000 \
		--model-checker-port 13000 \
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
		--htmlfeatures-bin ${HTMLFEATURES_BIN}

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
		--mc-input-stdin \
		--mc-htmlfeatures-bin ${HTMLFEATURES_BIN} \
		--mc-model-checker-path ${MODEL_CHECKER_PATH} \
		--verbose

all-features: build-debug
	Debug/phishsvc \
		--enable-training-data \
		--td-stdin \
		--td-class-value 0 \
		--enable-features \
		--td-output-url \
		--td-output-extra-values \
		--htmlfeatures-bin ${THESIS_HTML_ANALYSIS_PROG} \
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
