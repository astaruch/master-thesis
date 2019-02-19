cmake_minimum_required(VERSION 3.10)
project(text_urls_to_database)

set(CMAKE_CXX_STANDARD 17)

find_package(cxxopts REQUIRED)
find_package(Poco REQUIRED Foundation)


add_executable(
        text_urls_to_database
        main.cpp
)

target_link_libraries(
        text_urls_to_database
        PUBLIC

        ${Poco_LIBRARIES}
        sqlite3
)