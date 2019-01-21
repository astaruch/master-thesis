cmake_minimum_required(VERSION 3.10)

set(URLLIB urllib)

project(${URLLIB})

set(CMAKE_CXX_STANDARD 17)

add_library(
        ${URLLIB}

        STATIC
        urllib.h
        urllib.cpp
)