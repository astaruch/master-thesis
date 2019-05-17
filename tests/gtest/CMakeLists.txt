message("Adding 'googletest' as an external project cloned from Github.")
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(gtest_builder NONE)
include(ExternalProject)

set(GTEST_FORCE_SHARED_CRT ON)
set(GTEST_DISABLE_PTHREADS ON)
set(GTEST_BUILD_TESTS OFF)

ExternalProject_Add(
  googletest

  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.8.0
  CMAKE_ARGS
  -Dgtest_force_shared_crt=${GTEST_FORCE_SHARED_CRT}
  -Dgtest_disable_pthreads=${GTEST_DISABLE_PTHREADS}
  -Dgtest_build_tests=${GTEST_BUILD_TESTS}
  -DBUILD_GTEST=ON
  -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
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

# Specify include dir
ExternalProject_Get_Property(googletest SOURCE_DIR)
set(GTEST_INCLUDE_DIRS ${SOURCE_DIR}/googletest/include PARENT_SCOPE)
set(GMOCK_INCLUDE_DIRS ${SOURCE_DIR}/googlemock/include PARENT_SCOPE)

# Specify MainTest's link libraries
ExternalProject_Get_Property(googletest BINARY_DIR)

set(GTEST_LIBS_DIR ${BINARY_DIR}/googlemock/gtest)
set(GMOCK_LIBS_DIR ${CMAKE_BINARY_DIR}/lib)
set(GOOGLE_LIBRARIES
  ${CMAKE_BINARY_DIR}/lib/libgtest.a
  ${CMAKE_BINARY_DIR}/lib/libgtest_main.a
  ${CMAKE_BINARY_DIR}/lib/libgmock.a
  PARENT_SCOPE
  )
