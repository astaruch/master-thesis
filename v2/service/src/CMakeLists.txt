cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)

project(phishsvc)

find_package(PostgreSQL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(spdlog REQUIRED)
find_package(Threads REQUIRED)
find_package(Poco REQUIRED Foundation Net)

set(PQ_INCLUDE_DIR ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/libpq)
set(PQXX_INCLUDE_DIR ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/pqxx)

set(PQXX_LIB ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/liblibpqxx_static.a)
set(PQ_LIB ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/libpq.a)
message(STATUS "Found libpqxx: ${PQXX_LIB}")

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

add_executable(phishsvc
  main.cpp
  )

target_include_directories(phishsvc
  PRIVATE
  ${PQXX_INCLUDE_DIR}
  ${PQ_INCLUDE_DIR}
  )

# Be **EXTREMELY** careful with an order for linking libraries.
# One small change, and whole project will fall apart
# due to unknown and unreadable reasons. God bless C++!
target_link_libraries(phishsvc
  PRIVATE

  spdlog::spdlog

  # First we need to link pqxx, then pq (otherwise linker will throw lot of errors)
  ${PQXX_LIB}
  ${PQ_LIB}

  # We need manually link openssl **after** linking postgres (otherwise there will be shit tone of weird errors)
  OpenSSL::SSL
  OpenSSL::Crypto

  # Threads come as last (or at least after ssl)
  Threads::Threads

  Poco::Foundation
  Poco::Net
)
