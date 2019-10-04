# #############################
# FROM ubuntu:latest as builder
# LABEL description="Build container"

# RUN apk update && apk add --no-cache \
#     autoconf bash build-base binutils cmake curl file gcc g++ git libgcc libstdc++ libtool linux-headers make musl musl-dev ninja tar unzip wget

# COPY ./src /src
# WORKDIR /src

# # This command compiles your app using GCC, adjust for your source code
# RUN g++ -o myapp main.cpp
# CMD bash


# #############################
# FROM alpine:latest as runtime
# LABEL description="Run container"

# RUN apk update && apk add --no-cache \
#     libstdc++

# COPY --from=builder  /src/myapp /app/myapp
# WORKDIR /app
# CMD ./myapp

FROM ubuntu:18.04 as builder
LABEL description="Build container"

RUN apt-get update \
    && apt-get -y install --no-install-recommends apt-utils dialog 2>&1 \
    #
    # Verify git, process tools, lsb-release (useful for CLI installs) installed
    && apt-get -y install curl git procps lsb-release unzip tar wget \
    #
    # Install C++ tools
    && apt-get -y install build-essential cmake cppcheck valgrind \
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
    && ./vcpkg install POCO

RUN cd /tmp/vcpkg \
    && ./vcpkg install spdlog libpq gtest yaml-cpp

COPY ./src /src
WORKDIR /src

RUN mkdir out \
    && cd out \
    && cmake .. -DCMAKE_TOOLCHAIN_FILE=/tmp/vcpkg/scripts/buildsystems/vcpkg.cmake \
    && make

CMD ["bash"]

# #############################
FROM ubuntu:18.04 as runtime
LABEL description="Run container"

COPY --from=builder  /src/out/phishsvc /app/phishsvc
WORKDIR /app
CMD ./phishsvc
