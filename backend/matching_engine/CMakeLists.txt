cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)

#include(../cmake/gtest.cmake)
#INIT_TEST(matching_engine)

add_subdirectory(../entities/quote quote)

add_library(matching_engine "matching_engine.cpp")
target_link_libraries(matching_engine quote)

