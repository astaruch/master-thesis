cmake_minimum_required(VERSION 3.6)
project(benchmark_url VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)

find_package(uriparser REQUIRED)
find_package(Poco REQUIRED Foundation)

set(cpprestsdk_DIR /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}/cmake/)
find_package(cpprestsdk REQUIRED)

find_package(folly REQUIRED)
find_package(Threads REQUIRED)


# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)
# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED Core)

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
        CQt.hpp
)


target_include_directories(
        benchmark_url
        PRIVATE

        ${Qt5Core_INCLUDE_DIRS}

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
        Qt5::Core
)