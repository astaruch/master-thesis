# HTML analysis module

This is a program for analysing HTML content regarding some features, which may indicate that site has signs of a phishing site.

## Installation

This application requires Node.JS (we are using currently stable version v12.10.0, but there shouldn't be any problem with a newer versions). For easier reproduction of the environment, we are using [nvm](https://github.com/nvm-sh/nvm):

    # Install nvm for the current user
    $ curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.35.1/install.sh | bash
    $ source ~/.bashrc
    $ nvm install v12.10.0
    $ node --version
    v12.10.0

    # Install packages for the application
    $ npm install
    $ node src/index.js --help

There are also unit tests for testing that we are computing right values for the features. We can run them with the following command:

    $ npm test

We are comforning the `eslint` rules with `standard` installed. Run the linting with a following command:

    $ npm run lint

There is also possibility to pack the application into single executable binary which can be easily distributed and executed:

    $ make pack
    $ bin/html-analysis --help

## Usage

The application can be used in two ways:

### Command line utility mode

Command line utility accepts input URL either from command line argument or standard input. We also can specify 2 output options: output as a comma separated string (used for `.csv`) or as a JSON. Example:

```bash
$ node src/index.js --url http://google.com --feat-src-link --output-values-string --include-header
src_link,ahref_link
0,0.5
```

### Server mode

Application can also listens on the socket for input url in the JSON format

    $ node src/index.js --server 12000 &  # start server on the background
    $ telnet localhost 12000              # open the client to test the server
    >>>
    { "url": "http://google.com", "features": { "featSrcLink": true, "featAhrefLink": true, "featFaviconLink": true }}
    <<<
    {"src_link":0,"ahref_link":0.7058823529411765,"favicon_link":0}

In case of error, client can receive an error:

    {
        "error": "BAD_INPUT",
        "message": "Unexpected end of JSON input"
    }

All possible options and combinations are provided in the `--help`.
