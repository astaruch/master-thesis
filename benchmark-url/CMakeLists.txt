cmake_minimum_required(VERSION 3.6)
project(benchmark_url)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(
        benchmark_url

        main.cpp

        IBenchmark.h
        SkyrUrl.hpp

        boost_asio.hpp
)


target_include_directories(
        benchmark_url
        PRIVATE

        ${Boost_INCLUDE_DIR}

)

target_link_libraries(
        benchmark_url

        ${Boost_LIBRARIES}
        skyr-url
)