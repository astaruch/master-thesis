import pickle
import argparse

def load_model(pkl_filename='extra_trees_model_15features.pkl'):
    with open(pkl_filename, 'rb') as file:
        pickle_model = pickle.load(file)
        return pickle_model
    raise 'Invalid path %s' % pkl_filename

def main():
    parser = argparse.ArgumentParser(description='Get phishing score based on prediction model')
    parser.add_argument('--values', type=str,
                        help='a string deliminated by a comma contianing values')
    args = parser.parse_args()
    print(args.values)
    model = load_model()
    print(model)
    pass


if __name__ == '__main__':
    main()
