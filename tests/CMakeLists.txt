find_package(Threads REQUIRED)

add_subdirectory(gtest)

file(GLOB test_headers [a-z_]*.h)
file(GLOB test_sources [a-z_]*.cpp)

set(TEST_TARGET run_tests)
add_executable(${TEST_TARGET}

  ${${PHISHAPP}_SOURCE_DIR}/libs/config.cpp
  ${test_sources}
  ${test_headers}
  )

target_include_directories(${TEST_TARGET} PRIVATE
  ${GTEST_INCLUDE_DIRS}
  ${GMOCK_INCLUDE_DIRS}

  ${${PHISHAPP}_SOURCE_DIR}
  ${${PHISHAPP}_SOURCE_DIR}/extern/spdlog/include
  ${YAML_CPP_INCLUDE_DIR}
  )

add_dependencies(${TEST_TARGET} googletest yaml-cpp)

target_link_libraries(${TEST_TARGET}
  ${CMAKE_THREAD_LIBS_INIT}
  ${GOOGLE_LIBRARIES}
  stdc++fs
  ${YAML_CPP_LIBRARY}
  )

enable_testing()

add_test(phishapp_tests ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_TARGET})

