# Phishing URL detection

[![Build Status](https://travis-ci.com/astaruch/master-thesis.svg?branch=master)](https://travis-ci.com/astaruch/master-thesis)

## Prerequisites

For an easier setup & development, we are advising to use Docker. Build steps and all other information are stored in [Dockerfile](v2/service/Dockerfile) and [docker-compose](v2/service/docker-compose.yml).

Shortcuts to build & run are in [Makefile](v2/service/Makefile).

Host machine for development:

- `docker-compose` version >= 3.7 / 1.24.1
- `docker` engine >= 18.06

## Building

For now, there is not prepared database table & data. You need to fill it manualy. Comming soon! In a good case, where the database is running and you have it's IP address, you can fire up the code with following commands:

    cd v2/service
    make build
    make test
    make run

Before first run, you can build & images to dockerhub:

    make prepare-images
    make push-image

## Debugging

For now, easiest debugging is done through a Visual Studio Code and `Remote Development` extension. There is prepared [folder](v2/.vscode/) with tasks and editor settings, to open folder in Docker container, so we can debug step by step actual code and have all the advantages of a IDE.
