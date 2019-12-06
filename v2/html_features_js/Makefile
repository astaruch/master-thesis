# Determine this makefile's path.
# Be sure to place this BEFORE `include` directives, if any.
THIS_FILE := $(lastword $(MAKEFILE_LIST))
include .env

build:
	npm install

clean:
	rm -rf node_modules bin

service:
	node src/index.js --service
run:
	node src/index.js ${HTML_FEATURES} --output-values-string --include-url --verbose --url http://google.cz

stdin:
	node src/index.js ${HTML_FEATURES} --output-values-string --include-header --include-values --include-url --stdin

json:
	node src/index.js --feat-src-link --feat-ahref-link --feat-favicon-link --output-json --url 'http://google.cz/aaa'

json2:
	node src/index.js --feat-src-link --feat-ahref-link --feat-favicon-link --output-json --url 'https://nab-ib.app/id/MDQ0Nzg5Ode4Mg'


header:
	node src/index.js ${HTML_FEATURES} --output-values-string --include-header --include-values --include-url --print-only-header

pack: build
	pkg src/index.js --targets node12-linux-x64 --output bin/htmlfeatures

