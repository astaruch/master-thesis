cmake_minimum_required(VERSION 3.1)
project(phishing_app)

set(CMAKE_CXX_STANDARD 17)

set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)

add_executable(
        phishing_app

        main.cpp
        json.hpp
)

include_directories(
        ${CURL_INCLUDE_DIR}
)

target_link_libraries(
        phishing_app
        ${CURL_LIBRARIES}
)
