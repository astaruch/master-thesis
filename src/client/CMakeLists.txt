cmake_minimum_required(VERSION 3.1)

project(phish_client)
set(CMAKE_CXX_STANDARD 17)

add_executable(phish_client

  main.cpp

  socket_client.cpp socket_client.h
  )

target_include_directories(phish_client PRIVATE

  PUBLIC
  ${SPDLOG_INCLUDE_DIR}
  )