cmake_minimum_required(VERSION 3.1)
project(plugins)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 14)

include_directories(vendor/tinydir)

option(JSON_BuildTests "Build the unit tests" OFF)
include_directories(vendor/json)
include(cmake/resource.cmake)

add_subdirectory(src)
add_subdirectory(src/dlplugin)
add_subdirectory(example)
