# Determine this makefile's path.
# Be sure to place this BEFORE `include` directives, if any.
THIS_FILE := $(lastword $(MAKEFILE_LIST))

build:
	@echo $@  # print target name
	@$(MAKE) -f $(THIS_FILE) build-debug # invoke other target

run: feature2
	@$(MAKE) -f $(THIS_FILE) feature-all

test: build-tests
	@echo $@
	Debug/test/all_tests

ctest: build-tests
	@echo $@
	cd Debug && ctest

clean:
	@echo $@
	rm -rf Debug Release

build-tests:
	@echo $@ # print target name
	mkdir -p Debug
	cd Debug && cmake \
		-D CMAKE_BUILD_TYPE:STRING=Debug \
		-D CMAKE_TOOLCHAIN_FILE:STRING=/home/xstaruch/vcpkg/scripts/buildsystems/vcpkg.cmake \
		-D RUN_TESTS:BOOL=ON \
		..
	cd Debug && \
		make -j 4

build-debug:
	@echo $@ # print target name
	mkdir -p Debug
	cd Debug && cmake \
		-D CMAKE_BUILD_TYPE:STRING=Debug \
		-D CMAKE_TOOLCHAIN_FILE:STRING=/home/xstaruch/vcpkg/scripts/buildsystems/vcpkg.cmake \
		-D RUN_TESTS:BOOL=OFF \
		..
	cd Debug && \
		make -j 4

feature1: build-debug
	Debug/phishsvc \
		--enable-training-data \
		--td-url "http://google.com" \
		--td-class-value 0 \
		--enable-features \
		--feat-ip-address

feature2: build-debug
	Debug/phishsvc \
		--enable-training-data \
		--td-url "http://google.com" \
		--td-class-value 0 \
		--enable-features \
		--feat-ip-address \
		--feat-url-length

feature-all: build-debug
	Debug/phishsvc \
		--enable-training-data \
		--td-url "http://google.com" \
		--td-class-value 0 \
		--enable-features \
		--feat-ip-address \
		--feat-url-length \
		--feat-host-length \
		--feat-path-length \
		--feat-query-length \
		--feat-fragment-length \
		--feat-user-info \
		--feat-domain-count \
		--feat-https-used \
		--feat-extra-https \
		--feat-shortening-service \
		--feat-non-std-port
