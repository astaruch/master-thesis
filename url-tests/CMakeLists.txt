cmake_minimum_required(VERSION 3.10)
project(url_tests)

set(CMAKE_CXX_STANDARD 17)

find_package(cxxopts REQUIRED)
find_package(Poco REQUIRED Foundation)

add_executable(
        url_tests
        main.cpp

        UrlTest.cpp UrlTest.h
)

target_link_libraries(
        url_tests
        PUBLIC

        ${Poco_LIBRARIES}
)