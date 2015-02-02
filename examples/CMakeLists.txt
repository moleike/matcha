# test for matcha
file(GLOB sourceFiles "*.cpp")
set(CMAKE_CXX_FLAGS "-std=c++11")

include_directories(${GTEST_INCLUDE_DIR})
message(STATUS "GTEST_INCLUDE_DIR: " ${GTEST_INCLUDE_DIR})

add_executable(example_gtest "example-gtest.cpp")
target_link_libraries(example_gtest ${GTEST_LIBRARY_PATH})

if(Boost_FOUND)
  add_executable(example_boosttest "example-boosttest.cpp")
endif()

