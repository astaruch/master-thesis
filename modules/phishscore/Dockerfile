FROM ubuntu:bionic-20191029 as dependencies
#    ubuntu:18.04.2

RUN apt-get update \
    && apt-get -y install --no-install-recommends apt-utils dialog 2>&1 \
    # Verify git, process tools, lsb-release (useful for CLI installs) installed
    && apt-get -y install curl git procps lsb-release unzip tar wget \
    # Install C++ tools
    && apt-get -y install build-essential cmake cppcheck valgrind \
    # Install add-apt-repository
    && apt-get -y install software-properties-common \
    # Install additional compilers
    && add-apt-repository ppa:ubuntu-toolchain-r/test \
    && apt-get -y install gcc-8 g++-8 gcc-9 g++-9 \
    && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 700 --slave /usr/bin/g++ g++ /usr/bin/g++-7 \
    && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 800 --slave /usr/bin/g++ g++ /usr/bin/g++-8 \
    && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 900 --slave /usr/bin/g++ g++ /usr/bin/g++-9 \
    # Clean up
    && apt-get autoremove -y \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/*

# Install vcpkg for easier maintainence of C++ libs
RUN cd /tmp \
    && git clone --branch 2019.09 https://github.com/Microsoft/vcpkg.git \
    && cd vcpkg \
    && ./bootstrap-vcpkg.sh \
    && ./vcpkg integrate install \
    && ./vcpkg integrate bash

# Install C++ 3rd party libraries
RUN cd /tmp/vcpkg \
    && ./vcpkg install POCO spdlog libpq gtest yaml-cpp libpqxx cxxopts fmt nlohmann-json
RUN cd /tmp/vcpkg \
    && ./vcpkg install curl

# Install Levehnstein for edit distance libraries
RUN cd /tmp \
    && git clone -b v1.2.4 https://github.com/Martinsos/edlib.git && cd edlib/build \
    && cmake -D CMAKE_BUILD_TYPE=Release .. \
    && make \
    && make install

# Install faup to get URL parts from commandline
RUN cd /tmp \
    && git clone -b v1.5 git://github.com/stricaud/faup.git && cd faup/build \
    && cmake .. \
    && make \
    && make install \
    && echo '/usr/local/lib' | tee -a /etc/ld.so.conf.d/faup.conf \
    && ldconfig

##############################################
FROM dependencies AS builder

COPY ./src /app/src
COPY ./test /app/test
COPY ./CMakeLists.txt /app/CMakeLists.txt
WORKDIR /app
ENV CMAKE_VCPKG_TOOLCHAIN=/tmp/vcpkg/scripts/buildsystems/vcpkg.cmake

RUN mkdir out \
    && cd out \
    && cmake .. -DCMAKE_TOOLCHAIN_FILE:STRING=$CMAKE_VCPKG_TOOLCHAIN \
                -DRUN_TESTS:BOOL=ON \
                -DCMAKE_BUILD_TYPE:STRING=Debug \
    && make -j 4

RUN /app/out/phishsvc --help

RUN /app/out/test/all_tests

CMD ["bash"]

###################################
FROM ubuntu:bionic-20191029  as runtime
#    ubuntu:18.04.2

RUN apt-get update \
    && apt-get -y install software-properties-common \
    && add-apt-repository ppa:ubuntu-toolchain-r/test \
    && apt-get -y install gcc-9 g++-9

# dnsutils => dig
# netbase => getaddrinfo() calls inside whois are failing without netbase
RUN apt-get install -y curl dnsutils whois netbase jq

COPY --from=builder /usr/local/lib/libfaupl.so /usr/local/lib/libfaupl.so
COPY --from=builder /usr/local/bin/faup /usr/local/bin/faup
COPY ./mozilla.tlds /root/.faup/mozilla.tlds
RUN echo '/usr/local/lib' | tee -a /etc/ld.so.conf.d/faup.conf \
    && ldconfig

COPY --from=builder  /app/out/phishsvc /app/phishsvc


CMD ["/app/phishsvc", "--help"]
