FROM ubuntu:18.04 as thesis-deps

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
    && ./vcpkg install POCO spdlog libpq gtest yaml-cpp libpqxx cxxopts fmt

CMD  ["bash"]

##############################################
FROM thesis-deps AS thesis-builder

COPY ./src /project/src
COPY ./test /project/test
COPY ./CMakeLists.txt /project/CMakeLists.txt
WORKDIR /project

RUN mkdir out \
    && cd out \
    && cmake .. -DCMAKE_TOOLCHAIN_FILE:STRING=/tmp/vcpkg/scripts/buildsystems/vcpkg.cmake \
                -DRUN_TESTS:BOOL=ON \
                -DCMAKE_BUILD_TYPE:STRING=Debug
    && make -j 4

CMD ["bash"]

###################################
FROM ubuntu:18.04 as thesis-runtime

RUN apt-get update \
    && apt-get -y install software-properties-common \
    && add-apt-repository ppa:ubuntu-toolchain-r/test \
    && apt-get -y install gcc-9 g++-9

CMD ["bash"]


##########################################
FROM astaruch/thesis-runtime as thesis-app

COPY --from=thesis-builder  /project/out/phishsvc /app/phishsvc
WORKDIR /app

CMD ["/app/phishsvc", "--help"]


############################################
FROM astaruch/thesis-runtime as thesis-tests

COPY --from=thesis-builder  /project/out/test/all_tests /app/all_tests
WORKDIR /app

CMD ["/app/all_tests"]
