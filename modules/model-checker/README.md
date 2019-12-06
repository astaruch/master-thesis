# Prediction model checker module

This is an application for computing phishing probability. The application uses pretrained model for predefined features. The module is returning JSON with phishing score in a following format:

    { "score": <integer from range [0-100]> }

In case of error, response is in following format:

    {
        "error": "PREDICTION_MODEL[_METHOD]",
        "message": string
    }

## Installation

This application requires:

- Python (>= 3.6)
- numpy (>=1.17.4)
- scipy (>=1.3.3)
- scikit-learn (>=0.21.3)

For convenienence, here are the installation steps for quick start:

    $ sudo apt install python3 python3-pip
    $ pip3 install --user virtualenv==16.1 # https://github.com/pyinstaller/pyinstaller/issues/4064
    $ virtualenv --python=python3 venv
    $ source venv/bin/activate
    (venv) $ pip install -r requirements.txt

We have also prepared option for packing the application into single executable:

    (venv) $ make pack

## Information

The application is expecting JSON with following features values:

1. host_length
1. spec_keywords
1. path_length
1. url_length
1. spec_chars_query
1. spec_chars_path
1. gtld
1. ahref_link
1. favicon_link
1. src_link
1. asn
1. similar_domain
1. dnssec
1. dns_updated
1. dns_created

It's on the user that JSON is valid. Example of a JSON:

    {"ahref_link":1.0,"asn":1.0,"dns_created":1.0,"dns_updated":1.0,"dnssec":1.0,"favicon_link":1.0,"gtld":1.0,"host_length":0.037037037037037035,"path_length":0.0,"similar_domain":0.0,"spec_chars_path":0.0,"spec_chars_query":0.0,"spec_keywords":0.0,"src_link":1.0,"url_length":0.027210884353741496}

The application has three modes of accepting input:

- listening on a socket
- standard input
- command line argument (quotation marks has to be properly escaped)

## Usage

    (venv) $ python model.py --help
    usage: model.py [-h] [--server SERVER] [--data-json DATA_JSON] [-]

    Get phishing score from pretrained model

    optional arguments:
    -h, --help            show this help message and exit
    --server SERVER       run as server application with listening socket on
                            given port
    --data-json DATA_JSON
                            accept escaped JSON from cmdline
    -, --stdin            accept JSON from standard input

## Example

Start the server:

    (venv) $ python model_checker.py --server 13000

Start the client:

    $ telnet localhost 13000
    Trying 127.0.0.1...
    Connected to localhost.
    Escape character is '^]'.
    {"ahref_link":1.0,"asn":1.0,"dns_created":1.0,"dns_updated":1.0,"dnssec":1.0,"favicon_link":1.0,"gtld":1.0,"host_length":0.037037037037037035,"path_length":0.0,"similar_domain":0.0,"spec_chars_path":0.0,"spec_chars_query":0.0,"spec_keywords":0.0,"src_link":1.0,"url_length":0.027210884353741496}

    {"score": 97}

