# Boost::Asio

# cpp-netlib/url - Skyr URL

- link - https://github.com/cpp-netlib/url
- modern C++17 library, active development
- prerequisites for Ubuntu 18.04:
    - C++17 compiler:

            $ sudo apt-get install gcc-8 g++8
            $ sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 700 --slave /usr/bin/g++ g++ /usr/bin/g++-7
            $ sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 800 --slave /usr/bin/g++ g++ /usr/bin/g++-8
    - ninja

            $ sudo apt-get install ninja-build

    - docs

            $ sudo apt-get install graphviz doxygen python-pip python-sphinx
            $ pip install sphinx_bootstrap_theme breathe recommonmark

Building & Install library:

    $ git clone https://github.com/cpp-netlib/url
    $ cd url
    $ git submodule update --init --recursive
    $ mkdir build && cd build
    $ cmake .. -G "Ninja" -DCMAKE_INSTALL_PREFIX=/usr
    $ ninja
    $ ninja test
    $ sudo ninja install
    $ ninja doc

# cpp-netlib/uri
- link - https://github.com/cpp-netlib/uri

Installing prerequisites same as in `skyr-url`

Building & Install library:

    $ git clone https://github.com/cpp-netlib/uri
    $ cd uri
    $ git submodule update --init --recursive
    $ mkdir _build & cd _build
    $ cmake -DUri_WARNINGS_AR_ERRORS=OFF ..
    $ make
    $ make test
    $ sudo make install
    $ make doc


# CUriparser

- link https://github.com/uriparser/uriparser
- C89 - ANSI C library

Building & Install library:

    ### GTEST
    $ GTEST_VERSION=1.8.1
    $ GTEST_PREFIX=~/.local/
    $  wget https://github.com/google/googletest/archive/release-${GTEST_VERSION}.tar.gz --no-check-certificate
    $ tar xf release-${GTEST_VERSION}.tar.gz
    $ cd googletest-release-${GTEST_VERSION}/
    $  cmake -DBUILD_SHARED_LIBS=ON -DCVF_VERSION=${GTEST_VERSION} -DCMAKE_INSTALL_PREFIX:PATH=${GTEST_PREFIX} .
    $ make
    $ make install

    ### URIPARSER
    $ git clone https://github.com/uriparser/uriparser.git
    $ cd uriparser && mkdir build && cd build
    $ cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=${GTEST_PREFIX} ..
    $ cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=/usr ..
    $ make
    $ make test
    $ sudo make install


