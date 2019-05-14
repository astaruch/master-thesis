message("Adding 'yaml-cpp' as an external project cloned from Github.")
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(yaml-cpp NONE)
include(ExternalProject)

ExternalProject_Add(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG 012269756149ae99745b6dafefd415843d7420bb
        CMAKE_ARGS
        -DYAML_CPP_BUILD_TESTS=OFF
        -DYAML_CPP_INSTALL=OFF
        -DYAML_CPP_BUILD_CONTRIB=OFF
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
        -DCMAKE_CXX_FLAGS_DEBUG=${CMAKE_CXX_FLAGS_DEBUG}
        -DCMAKE_CXX_FLAGS_RELEASE=${CMAKE_CXX_FLAGS_RELEASE}
        -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}/bin
        -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}/lib
        -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}/lib
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
        # Disable install step
        INSTALL_COMMAND ""
)

set(YAML_CPP_INCLUDE_DIR ${yaml-cpp_BINARY_DIR}/src/yaml-cpp/include PARENT_SCOPE)
set(YAML_CPP_LIBRARY ${CMAKE_BINARY_DIR}/lib/libyaml-cpp.a PARENT_SCOPE)

