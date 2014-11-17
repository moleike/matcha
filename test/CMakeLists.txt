# test for matcha
file(GLOB sourceFiles "*.cpp")
set(CMAKE_CXX_FLAGS "-std=c++11")
include_directories(${GTEST_INCLUDE_DIR})
message(STATUS "GTEST_INCLUDE_DIR: " ${GTEST_INCLUDE_DIR})

add_executable(example_test ${sourceFiles})
target_link_libraries(example_test ${GTEST_LIBRARY_PATH})
