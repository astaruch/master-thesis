# Module for updating Phishtank URLs

This program has 2 functions:

1. Populate database with predownloaded data from Phishtank
2. Run script for querying Phishtank database and update our database

## Installation

Node.js application (>=12.10.0).

    $ npm install
    $ node src/index.js --help
    Phishing URL fetcher

    Usage: index.js [options]

    Options:
    --version         Show version number                                [boolean]
    --help, -h        Show help                                          [boolean]
    --init, -i        Initialize Postgre database with prepared csv files
    --csv-folder, -p  Path containing Phishtank csv files
    --update, -u      Query the phishtank site and update database

## Requirements

The application is expecting that database is runinng. You need to setup at least these following environmental variables (or use it from the root `.env` file which is provided):

    TYPEORM_CONNECTION=postgres
    TYPEORM_HOST=0.0.0.0
    TYPEORM_USERNAME=thesis-user
    TYPEORM_PASSWORD=thesis-password
    TYPEORM_DATABASE=phishing-app
    TYPEORM_PORT=11000

Change accordingly to your application. The application is using package `dotenv` so the defaults are correctly loaded.

On each run, app is running migrations, so on the first start, it will create the needed tables.

## Populating the database with previously downloaded data

We can use previously downloaded data from the phishtank. We can set the path to the folder that contains all the `.csv` files:

> `pino-pretty` is for formatting logging messages

    node src/index.js -i -p <CSV_DIRECTORY> | pino-pretty

## Update database with current Phishtank URLs

The other mode is, that we can download a live snapshot and update it in the database. For this, we need to register register on [Phishtank](https://www.phishtank.com/) and set the environmental variable `PHISHTANK_API_KEY`. Then we can run:

    node src/index.js --update | pino-pretty

Again, we are expecting that database is running and all `TYPEORM_` variables are set.
