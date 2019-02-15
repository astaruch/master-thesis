# Boost::Asio

# Skyr URL

- link - https://github.com/cpp-netlib/url
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
    $ git submodule update --init --recursive
    $ mkdir build && cd build
    $ cmake .. -G "Ninja" -DCMAKE_INSTALL_PREFIX=/usr
    $ ninja
    $ ninja test
    $ sudo ninja install
    $ ninja doc
