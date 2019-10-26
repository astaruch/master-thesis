# Determine this makefile's path.
# Be sure to place this BEFORE `include` directives, if any.
THIS_FILE := $(lastword $(MAKEFILE_LIST))

build:
	@echo $@  # print target name
	@$(MAKE) -f $(THIS_FILE) build-debug # invoke other target

build-debug:
	@echo $@ # print target name
	mkdir -p Debug
	cd Debug && \
		cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=/home/xstaruch/vcpkg/scripts/buildsystems/vcpkg.cmake
	cd Debug && \
		make -j 4

clean:
	@echo $@
	rm -rf Debug Release

test: build-debug
	@echo $@
	Debug/test/all_tests

f-ip: build-debug
	Debug/phishsvc \
		--enable-training-data \
		--td-url "http://google.com" \
		--td-class-value 0 \
		--enable-features \
		--feat-ip-address

