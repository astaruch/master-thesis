cmake_minimum_required(VERSION 3.10)
project(phishsvc VERSION 0.0.1 LANGUAGES CXX)

## ======================================================================================##
## User options
## ======================================================================================##
option(RUN_TESTS "Build and run tests" FALSE)
if(CMAKE_TOOLCHAIN_FILE)
  message(STATUS "Using cmake toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
else()
  message(FATAL_ERROR "Undefined CMAKE_TOOLCHAIN_FILE var")
endif()


## ======================================================================================##
## Dependencies
## ======================================================================================##

### PostgreSQL - C - https://github.com/microsoft/vcpkg/tree/master/ports/libpq =========##
find_package(PostgreSQL REQUIRED)

### PostgreSQL - C++ - https://github.com/microsoft/vcpkg/tree/master/ports/libpqxx =====##
find_library(LIBPQXX libpqxx_static ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/$<$<CONFIG:Debug>:debug/>lib)
if(LIBPQXX)
  message(STATUS "Found libpqxx: ${LIBPQXX}")
else()
  message(FATAL_ERROR "Can't find LIBPQXX")
endif()

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
file(GLOB_RECURSE SOURCES src/*.cpp src/*.hpp)

## ======================================================================================##
## Libraries
## ======================================================================================##

# Be **EXTREMELY** careful with an order for linking libraries.
# One small change, and whole project will fall apart
# due to unknown and unreadable reasons. God bless C++!
set(ALL_LIBRARIES
  spdlog::spdlog
  # First we need to link pqxx, then pq (otherwise linker will throw lot of errors)
  ${LIBPQXX}
  ${PostgreSQL_LIBRARIES}
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
## Compiler warnings
## ======================================================================================##
add_library(project_warnings INTERFACE)
target_compile_options(project_warnings
  INTERFACE
  -Wall
  -Werror # treat all warnings as errors
  -Wextra # reasonable and standard
  -Wshadow # warn the user if a variable
            # declaration shadows one from a
            # parent context
  -Wnon-virtual-dtor # warn the user if a class
                      # with virtual functions
                      # has a non-virtual
                      # destructor. This helps
                      # catch hard to track down
                      # memory errors
  -Wold-style-cast # warn for c-style casts
  -Wcast-align # warn for potential performance
                # problem casts
  -Wunused # warn on anything being unused
  -Woverloaded-virtual # warn if you overload
                        # (not override) a
                        # virtual function
  -Wpedantic # warn if non-standard C++ is used
  -Wconversion # warn on type conversions that
                # may lose data
  -Wsign-conversion # warn on sign conversions
  -Wnull-dereference # warn if a null
                      # dereference is detected
  -Wdouble-promotion # warn if float is
                      # implicit promoted to
                      # double
  -Wformat=2 # warn on security issues around
              # functions that format output (ie
              # printf)
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  target_compile_options(project_warnings
    INTERFACE -Wmisleading-indentation # warn if
                                      # identation
                                      # implies blocks
                                      # where blocks do
                                      # not exist
              -Wduplicated-cond # warn if if / else chain
                                # has duplicated
                                # conditions
              -Wduplicated-branches # warn if if / else
                                    # branches have
                                    # duplicated code
              -Wlogical-op # warn about logical
                          # operations being used where
                          # bitwise were probably wanted
              -Wuseless-cast # warn if you perform a cast
                            # to the same type
    )
endif()

## ======================================================================================##
## Project options
## ======================================================================================##
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

## ======================================================================================##
## Executable
## ======================================================================================##
add_executable(phishsvc ${SOURCES})

target_link_libraries(phishsvc
  PRIVATE
  ${ALL_LIBRARIES}
  project_warnings
  project_options
)

## ======================================================================================##
## Tests
## ======================================================================================##
if (RUN_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()
