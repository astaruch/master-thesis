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
    && apt-get -y install git procps lsb-release \
    #
    # Install C++ tools
    && apt-get -y install build-essential cmake cppcheck valgrind \
    # Clean up
    && apt-get autoremove -y \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/*

COPY ./src /src
WORKDIR /src

# This command compiles your app using GCC, adjust for your source code
RUN g++ -o myapp main.cpp
CMD bash

# #############################
FROM ubuntu:18.04 as runtime
LABEL description="Run container"

COPY --from=builder  /src/myapp /app/myapp
WORKDIR /app
CMD ./myapp
