cmake_minimum_required(VERSION 3.10)

project(phishsvc)

find_package(spdlog REQUIRED)

add_executable(phishsvc
  main.cpp
  )

target_link_libraries(phishsvc
  spdlog::spdlog
)
