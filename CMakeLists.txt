# Common settings
cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(manual_h2_client
  VERSION 0.1.4
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lint)
add_subdirectory(mh2c)
enable_testing()
add_subdirectory(test)
add_subdirectory(sample/h2_get)
