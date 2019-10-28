cmake_minimum_required(VERSION 3.10)
project(phishsvc_test LANGUAGES CXX)


## ======================================================================================##
## Subdirectories and source files
## ======================================================================================##
file(GLOB TEST_SOURCES *.cpp)
file(GLOB_RECURSE APP_SOURCES ../src/*.cpp)

## Remove main file
get_filename_component(MAIN_FILE . ABSOLUTE)
set(MAIN_FILE "${MAIN_FILE}/../src/main.cpp")
list(REMOVE_ITEM APP_SOURCES "${MAIN_FILE}")
set(SOURCES "${APP_SOURCES};${TEST_SOURCES}")

## ======================================================================================##
## Google test
## ======================================================================================##
find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

## ======================================================================================##
## Executable
## ======================================================================================##
add_executable(all_tests ${SOURCES})
target_include_directories(all_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_compile_features(all_tests PUBLIC cxx_std_17)
add_dependencies(all_tests phishsvc)
target_link_libraries(all_tests
  PRIVATE

  GTest::gtest
  GTest::gtest_main
  GTest::gmock
  GTest::gmock_main
  ${ALL_LIBRARIES}
)

## ======================================================================================##
## CTest
## ======================================================================================##
gtest_discover_tests(all_tests)
