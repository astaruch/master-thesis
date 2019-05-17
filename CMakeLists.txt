cmake_minimum_required(VERSION 3.1)

###
### PROJECT SETTINGS
###
set(PHISHAPP phishing_detection)
project(${PHISHAPP})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")


###
### PROJECT OPTIONS
###
option(PHISHAPP_BUILD_TESTS "Enable testing" ON)
find_package(PostgreSQL REQUIRED)

add_executable(
  ${PHISHAPP}
  main.cpp
  libs/config.cpp libs/config.h
  libs/database.cpp libs/database.h
)

# From http://www.cmake.org/pipermail/cmake/2010-March/035992.html:
# function to collect all the sources from sub-directories
# into a single list
function(add_sources)
  get_property(is_defined GLOBAL PROPERTY SRCS_LIST DEFINED)
  if (NOT is_defined)
    define_property(GLOBAL PROPERTY SRCS_LIST
      BRIEF_DOCS "List of source files"
      FULL_DOCS "List of all source files in the entire project")
  endif ()
  # make absolute paths
  set(SRCS)
  foreach (s IN LISTS ARGN)
    if (NOT IS_ABSOLUTE "${s}")
      get_filename_component(s "${s}" ABSOLUTE)
    endif ()
    list(APPEND SRCS "${s}")
  endforeach ()
  # append to global list
  set_property(GLOBAL APPEND PROPERTY SRCS_LIST "${SRCS}")
endfunction(add_sources)

###
### EXTERNAL DEPENDENCIES
###
add_subdirectory(extern)
add_dependencies(
  ${PHISHAPP}
  yaml-cpp
  taopq
)

target_include_directories(
  ${PHISHAPP}

  PRIVATE
  ${YAML_CPP_INCLUDE_DIR}
  ${PostgreSQL_INCLUDE_DIRS}
  ${TAOPQ_INCLUDE_DIR}

  PUBLIC
  extern/spdlog/include
)

target_link_libraries(
  ${PHISHAPP}

  stdc++fs
  ${YAML_CPP_LIBRARY}
  ${PostgreSQL_LIBRARIES}
  ${TAOPQ_LIBRARY}
)

if (${PHISHAPP_BUILD_TESTS})
  enable_testing()
  add_subdirectory(tests)
endif ()


