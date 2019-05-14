cmake_minimum_required(VERSION 3.1)
set(PHISHAPP phishing_detection)
project(${PHISHAPP})

set(CMAKE_CXX_STANDARD 17)

add_executable(
  ${PHISHAPP}
  main.cpp
  libs/config.cpp libs/config.h
)


### EXTERNAL DEPENDENCIES
add_subdirectory(extern)
add_dependencies(
  ${PHISHAPP}
  yaml-cpp
)

target_include_directories(
  ${PHISHAPP}

  PRIVATE
  ${YAML_CPP_INCLUDE_DIR}

  PUBLIC
  extern/spdlog/include
)

target_link_libraries(
  ${PHISHAPP}

  stdc++fs
  ${YAML_CPP_LIBRARY}
)