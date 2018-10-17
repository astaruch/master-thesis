import urllib.request
from datetime import datetime


def main():
    key = 'cdc400e08a81d1e231cfd3ea919291c8395f7c0289421671dff35be88b3b7558'
    url = 'http://data.phishtank.com/data/' + key + '/online-valid.csv.bz2'
    print(url)
    today = datetime.now().strftime('%Y-%m-%d')
    extension = '.csv.bz2'
    filename = today + extension
    print(filename)
    urllib.request.urlretrieve(url, filename)


if __name__ == '__main__':
    main()
