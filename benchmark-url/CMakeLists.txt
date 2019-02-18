cmake_minimum_required(VERSION 3.6)
project(benchmark_url VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)

find_package(uriparser REQUIRED)
find_package(Poco REQUIRED Foundation)

set(cpprestsdk_DIR /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}/cmake/)
find_package(cpprestsdk REQUIRED)

find_package(folly REQUIRED)
find_package(Threads REQUIRED)

add_executable(
        benchmark_url

        main.cpp

        IBenchmark.h

        CCppRestSdk.hpp
        CNetworkUri.hpp
        CPoco.hpp
        CSkyrUrl.hpp
        CUriparser.hpp
        CFolly.hpp
)


target_include_directories(
        benchmark_url
        PRIVATE

)

target_link_libraries(
        benchmark_url
        PUBLIC

        ${CMAKE_THREAD_LIBS_INIT}
        skyr-url
        uriparser::uriparser
        network-uri
        ${Poco_LIBRARIES}
        cpprestsdk::cpprest
        ${FOLLY_LIBRARIES}
)