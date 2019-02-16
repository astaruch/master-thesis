cmake_minimum_required(VERSION 3.6)
project(benchmark_url VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)

#find_package(Boost REQUIRED COMPONENTS system)
find_package(uriparser REQUIRED)

add_executable(
        benchmark_url

        main.cpp

        IBenchmark.h
        CSkyrUrl.hpp

        boost_asio.hpp
        CUriparser.hpp
)


target_include_directories(
        benchmark_url
        PRIVATE

#        ${Boost_INCLUDE_DIR}
)

target_link_libraries(
        benchmark_url
        PUBLIC

#        ${Boost_LIBRARIES}
        skyr-url
        uriparser::uriparser
        network-uri
)