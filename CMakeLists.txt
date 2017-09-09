cmake_minimum_required(VERSION 3.1)
project(plugins)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(VENDOR_PATH ${CMAKE_CURRENT_BINARY_DIR}/sol2)

set(CMAKE_CXX_STANDARD 14)

include_directories(vendor/tinydir)

option(JSON_BuildTests "Build the unit tests" OFF)
include_directories(vendor/json)

include(cmake/resource.cmake)
#include(cmake/sol2.cmake)

add_subdirectory(vendor/spdlog)
include_directories(vendor/spdlog/include)

add_subdirectory(vendor/fmt)
include_directories(vendor/fmt)


add_subdirectory(src/utils)
include_directories(src/utils/include)
add_subdirectory(src)
add_subdirectory(src/dlplugin)
add_subdirectory(example)
