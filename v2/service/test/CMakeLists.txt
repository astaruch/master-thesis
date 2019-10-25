cmake_minimum_required(VERSION 3.10)
project(phishsvc_test LANGUAGES CXX)

find_package(GTest CONFIG REQUIRED)

add_executable(all_tests main.cpp)

target_link_libraries(all_tests
  PRIVATE

  GTest::gtest
  GTest::gtest_main
  GTest::gmock
  GTest::gmock_main
)

add_test(NAME all_tests COMMAND all_tests)
