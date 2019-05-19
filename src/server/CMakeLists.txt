cmake_minimum_required(VERSION 3.1)

project(phish_server)
set(CMAKE_CXX_STANDARD 17)

find_package(PostgreSQL 9.6 REQUIRED)
find_package(Poco REQUIRED Foundation Net)

add_executable(phish_server

  main.cpp
  ${PHISHAPP_LIBS_SOURCES}

  url_test.cpp url_test.h
  socket_server.cpp socket_server.h
  )

add_dependencies(phish_server
  yaml-cpp
  taopq
  )

target_include_directories(phish_server PRIVATE

  ${${PHISHAPP}_SOURCE_DIR}
  ${YAML_CPP_INCLUDE_DIR}
  ${PostgreSQL_INCLUDE_DIRS}
  ${TAOPQ_INCLUDE_DIR}

  PUBLIC
  ${SPDLOG_INCLUDE_DIR}
  )

target_link_libraries(phish_server

  stdc++fs
  ${YAML_CPP_LIBRARY}
  ${PostgreSQL_LIBRARIES}
  ${TAOPQ_LIBRARY}
  ${Poco_LIBRARIES}

)