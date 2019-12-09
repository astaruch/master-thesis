import argparse
import json
import pickle
import socketserver
import sys
import numpy as np

from os.path import dirname
from os.path import join
from os.path import abspath
from os import chdir
from os import environ

# Hidden imports for pyinstaller. Don't delete
from sklearn.ensemble import ExtraTreesClassifier
import sklearn.utils._cython_blas
import sklearn.neighbors.typedefs
import sklearn.neighbors.quad_tree
import sklearn.tree._utils


def parse_args():
    parser = argparse.ArgumentParser(description='Get phishing score from ' +
                                                 'pretrained model')
    parser.add_argument('--server', type=int,
                        help='run as server application with listening socket on given port')
    parser.add_argument('--data-json', type=str,
                        help='accept escaped JSON from cmdline')
    parser.add_argument('-', '--stdin', dest='stdin', action='store_true',
                        help='accept JSON from standard input')
    return parser.parse_args()


def load_model(filename='pretrained_model.pkl'):
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


def predict_score(data, model):
    """
    The 'data' is parsed JSON into dict. We're sorting values by columns
    so we have correct order of the feature values. The prediction model
    is expecting array of np arrays with 15 features a.k.a shape (X, 15).

    Expected input for the model: [[1., 1., ..., 0., 0.5]]
    """
    model_input = np.array([[(val) for key, val in sorted(data.items())]])
    if model_input.shape[1] != 15:
        return
        {
            'error': 'PREDICTION_MODEL',
            'message': 'Input data has a wrong number of features.' +
                       'Got: %s. Expected: 15' % model_input.shape[1]
        }

    # https://scikit-learn.org/stable/modules/generated/sklearn.ensemble.ExtraTreesClassifier.html#sklearn.ensemble.ExtraTreesClassifier.predict_proba
    # class_proba predict class probabilities for X (all records).
    # Our classes are [0: benign, 1: phishing]
    # Since we are checking only one record, we are taking the first
    # element of the returning array which may looks like: [[0.03, 0.97]]
    class_proba = model.predict_proba(model_input)
    score = class_proba[0]  # only one queried element
    score = class_proba.item(1)  # probability for phishing class
    score = int(100 * score)
    return {'score': score}


def wrap_handler(model):
    class MyTCPHandler(socketserver.BaseRequestHandler):
        """
        The request handler class for our server.
        It is instantiated once per connection to the server.
        """
        def handle(self):
            print('Client connected')
            self.data = self.request.recv(1024).strip()

            print("Received: {}".format(self.data.decode('utf-8')))
            fvec = json.loads(self.data)

            response = predict_score(fvec, model)
            print('Sending:', json.dumps(response))
            self.request.sendall(json.dumps(response).encode('utf-8'))

            print('Client disconnected')
    return MyTCPHandler


def main():
    args = parse_args()
    model = load_model()
    model.verbose = False
    if args.server:
        port = int(args.server)
        print('Starting server for prediction model on port %d' % port)
        host = '0.0.0.0'
        handler = wrap_handler(model)
        socketserver.TCPServer.allow_reuse_address = True
        with socketserver.TCPServer((host, port), handler) as server:
            server.serve_forever()
    elif args.stdin:
        for line in sys.stdin:
            fvec = json.loads(line)
            print(json.dumps(predict_score(fvec, model)))
    elif args.data_json:
        try:
            fvec = json.loads(args.data_json)
            print(json.dumps(predict_score(fvec, model)))
        except json.decoder.JSONDecodeError as err:
            print(json.dumps({
                'error': 'PREDICTION_MODEL_JSON',
                'message': err
            }))
            sys.exit(1)


if __name__ == '__main__':
    main()
