#!/bin/sh

# This script is shortcut to running a Docker container
# with commandline arguments. Default entry (without args) is
# '/app/phishsvc --help' (this is defined in a Dockerfile).
# If we pass any arguments to this
# script, we can change the default entry '--help' to anything else, e.g.:
#
# $ ./run.sh --help
#   => docker run --rm --name thesis-phish-app thesis-app --help
#   => /app/phishsvc --help
# $ ./run.sh --config conf.ini --database postgres -d
#   => docker run --rm --name thesis-phish-app thesis-app --config conf.ini --database postgres -d
#   => /app/phishsvc --config conf.ini --database postgres -d
docker run --rm --name thesis-phish-app thesis-app "$@"
