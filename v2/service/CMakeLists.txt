cmake_minimum_required(VERSION 3.10)
project(phishsvc VERSION 0.0.1 LANGUAGES CXX)

## ======================================================================================##
## Dependencies
## ======================================================================================##

### PostgreSQL - https://github.com/microsoft/vcpkg/tree/master/ports/libpqxx ===========##
find_package(PostgreSQL REQUIRED)
set(PQ_INCLUDE_DIR ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/libpq)
set(PQXX_INCLUDE_DIR ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/pqxx)

set(PQXX_LIB ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/liblibpqxx_static.a)
set(PQ_LIB ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/libpq.a)
message(STATUS "Found libpqxx: ${PQXX_LIB}")

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

### OpenSSL - https://github.com/microsoft/vcpkg/tree/master/ports/openssl ==============##
find_package(OpenSSL REQUIRED)

### spdlog - https://github.com/microsoft/vcpkg/tree/master/ports/spdlog ================##
find_package(spdlog CONFIG REQUIRED)

### Poco - https://github.com/microsoft/vcpkg/tree/master/ports/poco ====================##
find_package(Poco CONFIG REQUIRED Foundation Net)

### cxxopts - https://github.com/microsoft/vcpkg/tree/master/ports/cxxopts ==============##
find_package(cxxopts CONFIG REQUIRED)

### fmt - https://github.com/microsoft/vcpkg/tree/master/ports/fmt ======================##
find_package(fmt CONFIG REQUIRED)

### threading ===========================================================================##
find_package(Threads REQUIRED)

## ======================================================================================##
## Subdirectories and source files
## ======================================================================================##
file(GLOB_RECURSE SOURCES src/*.cpp)

## ======================================================================================##
## Libraries
## ======================================================================================##

# Be **EXTREMELY** careful with an order for linking libraries.
# One small change, and whole project will fall apart
# due to unknown and unreadable reasons. God bless C++!
set(ALL_LIBRARIES
  spdlog::spdlog

  # First we need to link pqxx, then pq (otherwise linker will throw lot of errors)
  ${PQXX_LIB}
  ${PQ_LIB}

  # We need manually link openssl **after** linking postgres (otherwise there will be shit tone of weird errors)
  OpenSSL::SSL
  OpenSSL::Crypto

  # Threads come after ssl
  Threads::Threads

  Poco::Foundation
  Poco::Net
  cxxopts::cxxopts
  fmt::fmt
  fmt::fmt-header-only
)

## ======================================================================================##
## Executable
## ======================================================================================##
add_executable(phishsvc ${SOURCES})
target_compile_features(phishsvc PUBLIC cxx_std_14)

target_include_directories(phishsvc
  PRIVATE
  ${PQXX_INCLUDE_DIR}
  ${PQ_INCLUDE_DIR}
)
target_link_libraries(phishsvc PRIVATE ${ALL_LIBRARIES})

## ======================================================================================##
## Tests
## ======================================================================================##
enable_testing()
add_subdirectory(test)
