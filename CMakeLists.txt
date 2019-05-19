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
option(PHISHAPP_BUILD_CLIENT "Build client application" ON)
option(PHISHAPP_BUILD_SERVER "Build server application" ON)

set(PHISHAPP_LIBS_DIR ${${PHISHAPP}_SOURCE_DIR}/libs)
set(PHISHAPP_LIBS_SOURCES
  ${PHISHAPP_LIBS_DIR}/config.h
  ${PHISHAPP_LIBS_DIR}/config.cpp
  ${PHISHAPP_LIBS_DIR}/database.h
  ${PHISHAPP_LIBS_DIR}/database.cpp
  )

set(SPDLOG_INCLUDE_DIR ${${PHISHAPP}_SOURCE_DIR}/extern/spdlog/include)

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

add_subdirectory(extern)
add_subdirectory(src)

if (${PHISHAPP_BUILD_TESTS})
  enable_testing()
  add_subdirectory(tests)
endif ()


