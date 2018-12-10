message("Adding 'safebrowsing-cpp' library as an external project from GitHub.")
cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(safebrowsing NONE)
include(ExternalProject)
ExternalProject_Add(
        safebrowsing
        GIT_REPOSITORY https://github.com/astaruch/safebrowsing-cpp
        CMAKE_ARGS
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
        # Disable install step
        INSTALL_COMMAND ""
)
set(SAFEBROWSING_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/src/safebrowsing/ PARENT_SCOPE)
set(SAFEBROWSING_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/src/safebrowsing-build)
set(SAFEBROWSING_LIB ${SAFEBROWSING_LIB_DIR}/libsafebrowsing.a PARENT_SCOPE)
