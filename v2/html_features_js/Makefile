# Determine this makefile's path.
# Be sure to place this BEFORE `include` directives, if any.
THIS_FILE := $(lastword $(MAKEFILE_LIST))
include .env

build:
	npm install

clean:
	rm -rf node_modules bin

run:
	node src/index.js ${HTML_FEATURES} --output-lines --url http://seznam.cz

input-tag:
	node src/index.js --url http://google.cz --feat-input-tag --output-lines

pack: build
	pkg src/index.js --targets node12-linux-x64 --output bin/htmlfeatures
