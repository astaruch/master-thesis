import pickle
import argparse
import json
import sys
from sklearn.ensemble import ExtraTreesClassifier
import numpy as np

from os.path import dirname
from os.path import join
from os.path import abspath
from os import chdir
from os import environ
import signal
import sys

# hidden imports for pyinstaller
import sklearn.utils._cython_blas
import sklearn.neighbors.typedefs
import sklearn.neighbors.quad_tree
import sklearn.tree._utils

import socketserver


def load_model(filename='extra_trees_model_15features.pkl'):
    # correct path in bundle
    # https://stackoverflow.com/questions/13946650/pyinstaller-2-0-bundle-file-as-onefile
    if hasattr(sys, '_MEIPASS'):
        chdir(sys._MEIPASS)
        filename = join(sys._MEIPASS, filename)
    else:
        filename = join(dirname(abspath(__file__)), filename)
    with open(filename, 'rb') as file:
        pickle_model = pickle.load(file)
        return pickle_model
    raise 'Invalid path %s' % filename


def parse_args():
    parser = argparse.ArgumentParser(description='Get phishing score based ' +
                                                 'on prediction model')
    parser.add_argument('-s', '--service', action='store_true',
                        help='run application as a service')
    parser.add_argument('-p', '--port', type=int, default=0,
                        help='port to bind the service')
    parser.add_argument('--stdin', default=False, action='store_true',
                        help='accept JSON from stdin')
    parser.add_argument('--data-json', type=str,
                        help='accept escaped JSON from cmdline')
    # parser.parse_args()
    return parser.parse_args()


def predict_score(data, model):
    # model is expecting array of arrays [(X, 15)]
    to_check = np.array([[(val) for key, val in sorted(data.items())]])
    if to_check.shape[1] != 15:
        return {
            'error': 'MODEL_PREDICTION',
            'message': 'Input data has wrong number of features: %d. Expected: 15' %
            to_check.shape[1]
        }
    # since we are checking only one element, we can access it directly
    score = model.predict(to_check)[0]
    return {'score': score}


def wrap_handler(model):
    class MyTCPHandler(socketserver.BaseRequestHandler):
        """
        The request handler class for our server.

        It is instantiated once per connection to the server, and must
        override the handle() method to implement communication to the
        client.
        """
        def handle(self):
            print('client connected')
            self.data = self.request.recv(1024).strip()

            print("received: {}".format(self.data.decode('utf-8')))
            fvec = json.loads(self.data)

            response = predict_score(fvec, model)
            print('sending:', json.dumps(response))
            self.request.sendall(json.dumps(response).encode('utf-8'))

            print('client disconnected')
    return MyTCPHandler


def main():
    args = parse_args()
    model = load_model()
    model.verbose = False
    if args.service:
        if (args.port):
            port = int(args.port)
        elif environ.get('THESIS_MODEL_CHECKER_PORT'):
            port = int(environ.get('THESIS_MODEL_CHECKER_PORT'))
        else:
            port = 13000
        print('Starting service for model checking on port %d' % port)
        host = "localhost"
        handler = wrap_handler(model)
        socketserver.TCPServer.allow_reuse_address = True
        with socketserver.TCPServer((host, port), handler) as server:
            server.allow_reuse_address = True
            # Activate the server; this will keep running until you
            # interrupt the program with Ctrl-C
            server.serve_forever()

    elif args.stdin:
        for line in sys.stdin:
            fvec = json.loads(line)  # feature values
            print(json.dumps(predict_score(fvec, model)))
    elif args.data_json:
        try:
            fvec = json.loads(args.data_json)
            print(json.dumps(predict_score(fvec, model)))
        except json.decoder.JSONDecodeError:
            print("Invalid JSON. Try to encode it properly")
            sys.exit(1)


if __name__ == '__main__':
    main()
