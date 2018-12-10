cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
set(PHISHTANK phishtank)
set(PHISHING_APP phishing-app)

project(${PHISHING_APP})


find_package(Threads REQUIRED)
find_package(CURL REQUIRED)


add_subdirectory(third-party)
add_subdirectory(third-party/safebrowsing)

add_library(
        ${PHISHTANK}

        STATIC
        phishtank/phishtank.h
        phishtank/phishtank.cpp
)

add_executable(
        ${PHISHING_APP}

        main.cpp
)

add_dependencies(
        ${PHISHING_APP}

        ${PHISHTANK}
        safebrowsing
)

target_include_directories(
        ${PHISHING_APP}

        PRIVATE
        ${SAFEBROWSING_INCLUDE_DIR}
)

message("${SAFEBROWSING_LIB_DIR}")
message("${SAFEBROWSING_LIB}")


target_link_libraries(
        ${PHISHING_APP}

        PRIVATE
        ${CMAKE_BINARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}${PHISHTANK}${CMAKE_STATIC_LIBRARY_SUFFIX}
        ${CMAKE_THREAD_LIBS_INIT}
        ${SAFEBROWSING_LIB}
        ${CURL_LIBRARIES}
)
