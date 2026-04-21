# Common settings
cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(manual_h2_client
  VERSION 0.1.6
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTING "Build tests" OFF)
option(MH2C_BUILD_SAMPLE "Build sample executable" OFF)
option(MH2C_ENABLE_WERROR "Treat compiler warnings as errors" OFF)

message(STATUS "Project will be installed to ${CMAKE_INSTALL_PREFIX}")

if (NOT CMAKE_CONFIGURATION_TYPES AND
    (NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL ""))
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build type set to ${CMAKE_BUILD_TYPE}")

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})

message(STATUS "Archive output directory: ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
message(STATUS "Library output directory: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

set(INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR} CACHE PATH "Installation directory for libraries")
set(INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR} CACHE PATH "Installation directory for header files")

foreach(prefix LIB INCLUDE)
  file(TO_NATIVE_PATH ${CMAKE_INSTALL_PREFIX}/${INSTALL_${prefix}DIR} _path)
  message(STATUS "Installing ${prefix} components to ${_path}")
  unset(_path)
endforeach()

add_subdirectory(lint)
add_subdirectory(mh2c)
include(CTest)

if (BUILD_TESTING)
  add_subdirectory(test)
endif()

if (MH2C_BUILD_SAMPLE)
  add_subdirectory(sample/h2_get)
  add_subdirectory(sample/h2_post)
  add_subdirectory(sample/h2_post_chunked_data)
  add_subdirectory(sample/h2_custom_headers)
  add_subdirectory(sample/h2_ping)
  add_subdirectory(sample/h2_window_update)
endif()
