cmake_minimum_required(VERSION 2.8)

project(matcha CXX)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/matcha)

#include(ext/gtest.cmake)
add_subdirectory(test)

