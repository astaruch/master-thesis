# Determine this makefile's path.
# Be sure to place this BEFORE `include` directives, if any.
THIS_FILE := $(lastword $(MAKEFILE_LIST))
include .env

build:
	npm install

clean:
	rm -rf node_modules

run:
	node src/index.js --url http://google.cz --output-lines ${HTML_FEATURES}

input-tag:
	node src/index.js --url http://google.cz --feat-input-tag --output-lines
