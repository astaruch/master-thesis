import pickle
import argparse
import json
import sys
from sklearn.ensemble import ExtraTreesClassifier
import numpy as np

# hidden imports for pyinstaller
import sklearn.utils._cython_blas
import sklearn.neighbors.typedefs
import sklearn.neighbors.quad_tree
import sklearn.tree._utils


def load_model(pkl_filename='extra_trees_model_15features.pkl'):
    with open(pkl_filename, 'rb') as file:
        pickle_model = pickle.load(file)
        return pickle_model
    raise 'Invalid path %s' % pkl_filename


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
            json.load('args.data_json')
        except json.decoder.JSONDecodeError:
            print("Invalid JSON. Try to encode it properly")
            sys.exit(1)


if __name__ == '__main__':
    main()
