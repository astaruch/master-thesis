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

# hidden imports for pyinstaller
import sklearn.utils._cython_blas
import sklearn.neighbors.typedefs
import sklearn.neighbors.quad_tree
import sklearn.tree._utils


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


def main():
    parser = argparse.ArgumentParser(description='Get phishing score based ' +
                                                 'on prediction model')
    parser.add_argument('--stdin', default=False, action='store_true',
                        help='accept JSON from stdin')
    parser.add_argument('--data-json', type=str,
                        help='accept escaped JSON from cmdline')
    args = parser.parse_args()
    model = load_model()
    model.verbose = False
    if args.stdin:
        for line in sys.stdin:
            fvec = json.loads(line)  # feature values
            # model is expecting array of arrays [(X, 15)]
            # we will pass only one line
            to_check = np.array([[(val) for key, val in sorted(fvec.items())]])
            # since we are checking only one element, we can access it directly
            phishingness = model.predict(to_check)[0]
            print(phishingness)
    elif args.data_json:
        try:
            fvec = json.loads(args.data_json)
            to_check = np.array([[(val) for key, val in sorted(fvec.items())]])
            phishingness = model.predict(to_check)[0]
            print(phishingness)
        except json.decoder.JSONDecodeError:
            print("Invalid JSON. Try to encode it properly")
            sys.exit(1)


if __name__ == '__main__':
    main()
