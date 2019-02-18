# Boost::Asio

# cpp-netlib/url - Skyr URL

- link - https://github.com/cpp-netlib/url
- modern C++17 library, active development
- prerequisites for Ubuntu 18.04:
    - C++17 compiler:

            $ sudo apt-get install gcc-8 g++-8
            $ sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 700 --slave /usr/bin/g++ g++ /usr/bin/g++-7
            $ sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 800 --slave /usr/bin/g++ g++ /usr/bin/g++-8
    - ninja

            $ sudo apt-get install ninja-build

    - docs

            $ sudo apt-get install graphviz doxygen python-pip python-sphinx
            $ pip install sphinx_bootstrap_theme breathe recommonmark
            
- usage in CMake: 
    
        target_link_libraries(project PUBLIC
                    skyr-url)
            
- usage in code:
            
       #include <skyr/url.hpp>
    

Building & Install library:

    $ git clone https://github.com/cpp-netlib/url cpp-netlib-url
    $ cd cpp-netlib-url
    $ git submodule update --init --recursive
    $ mkdir _build && cd _build
    $ cmake .. -G "Ninja" -DCMAKE_INSTALL_PREFIX=/usr
    $ ninja
    $ ninja test
    $ sudo ninja install
    $ ninja doc

# cpp-netlib/uri
- link - https://github.com/cpp-netlib/uri
- usage in CMake: 
    
        target_link_libraries(project PUBLIC
                    network-uri)
            
- usage in code:
            
       #include <network/uri.hpp>

Installing prerequisites same as in `skyr-url`

Building & Install library:

    $ git clone https://github.com/cpp-netlib/uri cpp-netlib-uri
    $ cd cpp-netlib-uri
    $ git submodule update --init --recursive
    $ mkdir _build && cd _build
    $ cmake -DUri_WARNINGS_AR_ERRORS=OFF ..
    $ make
    $ make test
    $ sudo make install
    $ make doc


# CUriparser

- link https://github.com/uriparser/uriparser
- C89 - ANSI C library
- usage in CMake: 
    
        find_package(uriparser REQUIRED)
        target_link_libraries(project PUBLIC
            uriparser::uriparser)
            
- usage in code:
            
        #include <uriparser/Uri.h>    


Building & Install library:

    ### GTEST
    $ GTEST_VERSION=1.8.1
    $ GTEST_PREFIX=~/.local/
    $ wget https://github.com/google/googletest/archive/release-${GTEST_VERSION}.tar.gz --no-check-certificate
    $ tar xf release-${GTEST_VERSION}.tar.gz
    $ cd googletest-release-${GTEST_VERSION}/
    $ cmake -DBUILD_SHARED_LIBS=ON -DCVF_VERSION=${GTEST_VERSION} -DCMAKE_INSTALL_PREFIX:PATH=${GTEST_PREFIX} .
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


# Poco

- link: https://github.com/pocoproject/poco/tree/master
- cons: heavy library, long building
- usage in CMake: 
    
        find_package(Poco REQUIRED Foundation)
        target_link_libraries(project PUBLIC
            ${Poco_LIBRARIES})
            
- usage in code:
            
        #include <Poco/URI.h> 

Building & Installing library:

    $ git clone -b master https://github.com/pocoproject/poco.git
    $ cd poco
    $ sudo cmake --install . 
    $ make
    $ sudo make install
    
# C++ REST SDK (Microsoft)

- link: https://github.com/Microsoft/cpprestsdk

Building & Installing library:

    $ sudo apt-get install libcpprest-dev
    
- usage in CMake:

    
    set(cpprestsdk_DIR /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}/cmake/)
    find_package(cpprestsdk REQUIRED)
    target_link_libraries(
            benchmark_url
            PUBLIC
            cpprestsdk::cpprest)
            
            
- usage in code:


    #include <cpprest/base_uri.h>
