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

## REMARKS

In case that you have problem with tests regarding DNSSEC, check [this article](https://moss.sh/name-resolution-issue-systemd-resolved/). Ubuntu 18.04 serverhave by default `systemd-resolved` which may casue a problems. If the command `dig . DNSKEY +short` displays nothing, you don't have configured the DNS correctly.

    $ dig . DNSKEY +short
    > no output
    $ sudo systemctl stop systemd-resolved.service
    $ sudo systemctl disable systemd-resolved.service
    $ sudo apt-get update && apt-get install -y unbound resolvconf
    $ sudo reboot
    $ dig . DNSKEY +short
    256 3 8 AwEAAbPwrxwtOMENWvblQbUFwBllR7ZtXsu9rg/LdyklKs9gU2GQTeOc 59XjhuAPZ4WrT09z6YPL+vzIIJqnG3Hiru7hFUQ4pH0qsLNxrsuZrZYm XAKoVa9SXL1Ap0LygwrIugEk1G4v7Rk/Alt1jLUIE+ZymGtSEhIuGQdX rEmj3ffzXY13H42X4Ja3vJTn/WIQOXY7vwHXGDypSh9j0Tt0hknF1yVJ CrIpfkhFWihMKNdMzMprD4bV+PDLRA5YSn3OPIeUnRn9qBUCN11LXQKb +W3Jg+m/5xQRQJzJ/qXgDh1+aN+Mc9AstP29Y/ZLFmF6cKtL2zoUMN5I 5QymeSkJJzc=
    257 3 8 AwEAAaz/tAm8yTn4Mfeh5eyI96WSVexTBAvkMgJzkKTOiW1vkIbzxeF3 +/4RgWOq7HrxRixHlFlExOLAJr5emLvN7SWXgnLh4+B5xQlNVz8Og8kv ArMtNROxVQuCaSnIDdD5LKyWbRd2n9WGe2R8PzgCmr3EgVLrjyBxWezF 0jLHwVN8efS3rCj/EWgvIWgb9tarpVUDK/b58Da+sqqls3eNbuv7pr+e oZG+SrDK6nWeL3c6H5Apxz7LjVc1uTIdsIXxuOLYA4/ilBmSVIzuDWfd RUfhHdY6+cn8HFRm+2hM8AnXGXws9555KrUB5qihylGa8subX2Nn6UwN R1AkUTV74bU=

## Levenshtein distance

    git clone -b v1.2.4 https://github.com/Martinsos/edlib.git && cd edlib/build
    cmake -D CMAKE_BUILD_TYPE=Release ..
    make
    sudo make install

Then find a library`libedlib_static.a` in CMakeListst (usually in /usr/local/lib) and header files (usually in /usr/local/include).

## SLD from commandline

To parse an URL from commandline, we are using program `faup`

    git clone -b v1.5 git://github.com/stricaud/faup.git && cd faup/build \
    cmake ..
    make
    sudo make install
    echo '/usr/local/lib' | sudo tee -a /etc/ld.so.conf.d/faup.conf
    ## or /usr/local/lib64 if you are using RPM based system
    sudo ldconfig
    $ faup -t domain_without_tld sites.google.com
    google
