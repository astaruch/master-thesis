# Determine this makefile's path.
# Be sure to place this BEFORE `include` directives, if any.
THIS_FILE := $(lastword $(MAKEFILE_LIST))
include .env

build:
	npm install

clean:
	rm -rf node_modules bin

run:
	node src/index.js ${HTML_FEATURES} --output-lines --url http://google.cz

stdin:
	node src/index.js ${HTML_FEATURES} --output-values-string --stdin

pack: build
	pkg src/index.js --targets node12-linux-x64 --output bin/htmlfeatures

