# Phishing URL detection

[![Build Status](https://travis-ci.com/astaruch/master-thesis.svg?branch=master)](https://travis-ci.com/astaruch/master-thesis)

## Prerequisites
> database setup

    $ sudo apt-get install wget ca-certificates
    $ wget --quiet -O - https://www.postgresql.org/media/keys/ACCC4CF8.asc | sudo apt-key add -
    $ sudo sh -c 'echo "deb http://apt.postgresql.org/pub/repos/apt/ `lsb_release -cs`-pgdg main" >> /etc/apt/sources.list.d/pgdg.list'
    $ sudo apt-get update
    $ sudo apt-get install postgresql-9.6 postgresql-contrib-9.6 postgresql-server-dev-9.6
    

Run a database and store connection string in `config.yaml`

> conn_string: postgres://postgres@localhost:5432/phishing-detection
    
