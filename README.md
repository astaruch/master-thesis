# Phishing URL detection

[![Build Status](https://travis-ci.com/astaruch/master-thesis.svg?branch=master)](https://travis-ci.com/astaruch/master-thesis)

## Prerequisites

For an easier setup & development, we are advising to use Docker. Build steps and all other information are stored in respective Dockerfile for each module.

For local development you need to check each Dockerfile for instructions, but at least you need to have installed following programs:

- CMake, GCC-9
- Python3
- Node.JS
- Postgres

Easy development:
- `docker-compose` version >= 3.7 / 1.24.1
- `docker` engine >= 18.06

## Running

You need to create an empty folder and set it to the `THESIS_DB_DOCKER_VOLUME`. Also you need following environmental variables:

    # Define following environmental variables which are specific to your paths
    # CMAKE_VCPKG_TOOLCHAIN=
    # THESIS_DB_DOCKER_VOLUME=
    # PHISHTANK_API_KEY=
    # GOOGLE_SAFEBROWSING_API_KEY=

Then run `docker-compose build`

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

